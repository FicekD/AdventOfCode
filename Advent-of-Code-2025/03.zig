const std = @import("std");

const Status = struct { total: u64, index: u64 };

pub fn read_batteries(
    allocator: std.mem.Allocator,
    path: []const u8,
) ![][]u64 {
    const file = try std.fs.cwd().openFile(path, .{});
    defer file.close();

    var batteries: std.ArrayList([]u64) = .empty;
    defer batteries.deinit(allocator);

    var file_buffer: [4096]u8 = undefined;
    var reader = file.reader(&file_buffer);

    while (try reader.interface.takeDelimiter('\n')) |line| {
        var battery_line: std.ArrayList(u64) = .empty;
        for (line) |char| {
            if (char == 10 or char == 13) {
                continue;
            }
            try battery_line.append(allocator, char - 48);
        }
        try batteries.append(allocator, try battery_line.toOwnedSlice(allocator));
    }

    return batteries.toOwnedSlice(allocator);
}

pub fn find_statuses(allocator: std.mem.Allocator, line: []u64, len: usize, status: Status) ![]const Status {
    if (len == 0) {
        return &[_]Status{status};
    }

    var max_digit: u64 = 0;
    for (line[status.index .. line.len - (len - 1)]) |v| {
        if (v > max_digit) max_digit = v;
    }

    var new_statuses: std.ArrayList(Status) = .empty;
    defer new_statuses.deinit(allocator);

    for (0.., line[status.index .. line.len - (len - 1)]) |i, v| {
        if (v == max_digit) {
            const new_status: Status = .{ .total = status.total + try std.math.powi(u64, 10, len - 1) * max_digit, .index = status.index + i + 1 };

            const found = try find_statuses(allocator, line, len - 1, new_status);

            for (found) |f| {
                try new_statuses.append(allocator, .{ .total = f.total, .index = f.index });
            }
        }
    }

    return new_statuses.toOwnedSlice(allocator);
}

pub fn max_status(statuses: []const Status) u64 {
    var max_total: u64 = 0;
    for (statuses) |v| {
        if (v.total > max_total) max_total = v.total;
    }
    return max_total;
}

pub fn part1(allocator: std.mem.Allocator, batteries: [][]u64) !void {
    var total: u64 = 0;

    for (batteries) |line| {
        const status: Status = .{ .total = 0, .index = 0 };
        const statuses = try find_statuses(allocator, line, 2, status);
        total += max_status(statuses);
    }

    std.debug.print("Part 1: {d}\n", .{total});
}

pub fn part2(allocator: std.mem.Allocator, batteries: [][]u64) !void {
    var total: u64 = 0;

    for (batteries) |line| {
        const status: Status = .{ .total = 0, .index = 0 };
        const statuses = try find_statuses(allocator, line, 12, status);
        total += max_status(statuses);
    }

    std.debug.print("Part 2: {d}\n", .{total});
}

pub fn main() !void {
    const allocator = std.heap.smp_allocator;

    const batteries = try read_batteries(allocator, "data/03.txt");

    try part1(allocator, batteries);
    try part2(allocator, batteries);
}
