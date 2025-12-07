const std = @import("std");

const Position = struct { position: u64, count: u64 };

pub fn read_map(
    allocator: std.mem.Allocator,
    path: []const u8,
) ![][]u8 {
    const file = try std.fs.cwd().openFile(path, .{});
    defer file.close();

    var map: std.ArrayList([]u8) = .empty;
    defer map.deinit(allocator);

    var file_buffer: [4096]u8 = undefined;
    var reader = file.reader(&file_buffer);

    while (try reader.interface.takeDelimiter('\n')) |line| {
        const trimmed = std.mem.trim(u8, line, " \r\n");
        const buf = try allocator.alloc(u8, trimmed.len);
        std.mem.copyForwards(u8, buf, trimmed);
        try map.append(allocator, buf);
    }

    return map.toOwnedSlice(allocator);
}

pub fn add_if_not_in(allocator: std.mem.Allocator, list: *std.ArrayList(u64), value: u64) !bool {
    for (list.items) |item| {
        if (item == value) return false;
    }
    try list.append(allocator, value);
    return true;
}

pub fn add_position_if_not_in(allocator: std.mem.Allocator, list: *std.ArrayList(Position), position: Position) !void {
    for (0..list.items.len) |i| {
        if (list.items[i].position == position.position) {
            list.items[i].count += position.count;
            return;
        }
    }
    try list.append(allocator, position);
}

pub fn part1(allocator: std.mem.Allocator, map: [][]u8) !void {
    var beam_positions: std.ArrayList(u64) = .empty;
    defer beam_positions.deinit(allocator);

    for (0..map[0].len) |col| {
        if (map[0][col] == 'S') {
            try beam_positions.append(allocator, col);
        }
    }

    var total_splits: u64 = 0;

    for (1..map.len) |row| {
        var new_beam_positions: std.ArrayList(u64) = .empty;
        defer new_beam_positions.deinit(allocator);

        for (beam_positions.items) |position| {
            if (map[row][position] == '^') {
                _ = try add_if_not_in(allocator, &new_beam_positions, position - 1);
                _ = try add_if_not_in(allocator, &new_beam_positions, position + 1);
                total_splits += 1;
            } else {
                _ = try add_if_not_in(allocator, &new_beam_positions, position);
            }
        }

        beam_positions.clearRetainingCapacity();
        try beam_positions.appendSlice(allocator, new_beam_positions.items);
    }

    std.debug.print("Part 1: {d}\n", .{total_splits});
}

pub fn part2(allocator: std.mem.Allocator, map: [][]u8) !void {
    var beam_positions: std.ArrayList(Position) = .empty;
    defer beam_positions.deinit(allocator);

    for (0..map[0].len) |col| {
        if (map[0][col] == 'S') {
            try beam_positions.append(allocator, .{ .position = col, .count = 1 });
        }
    }

    for (1..map.len) |row| {
        var new_beam_positions: std.ArrayList(Position) = .empty;
        defer new_beam_positions.deinit(allocator);

        for (beam_positions.items) |position| {
            if (map[row][position.position] == '^') {
                try add_position_if_not_in(allocator, &new_beam_positions, .{ .position = position.position - 1, .count = position.count });
                try add_position_if_not_in(allocator, &new_beam_positions, .{ .position = position.position + 1, .count = position.count });
            } else {
                try add_position_if_not_in(allocator, &new_beam_positions, position);
            }
        }

        beam_positions.clearRetainingCapacity();
        try beam_positions.appendSlice(allocator, new_beam_positions.items);
    }

    var timelines: u64 = 0;
    for (beam_positions.items) |position| {
        timelines += position.count;
    }

    std.debug.print("Part 2: {d}\n", .{timelines});
}

pub fn main() !void {
    const allocator = std.heap.smp_allocator;

    const map = try read_map(allocator, "data/07.txt");

    try part1(allocator, map);
    try part2(allocator, map);
}
