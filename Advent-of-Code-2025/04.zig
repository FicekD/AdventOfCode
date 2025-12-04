const std = @import("std");

const Status = struct { total: u64, index: u64 };

pub fn read_map(
    allocator: std.mem.Allocator,
    path: []const u8,
) ![][]u8 {
    const file = try std.fs.cwd().openFile(path, .{});
    defer file.close();

    var batteries: std.ArrayList([]u8) = .empty;
    defer batteries.deinit(allocator);

    var file_buffer: [4096]u8 = undefined;
    var reader = file.reader(&file_buffer);

    while (try reader.interface.takeDelimiter('\n')) |line| {
        var battery_line: std.ArrayList(u8) = .empty;
        for (line) |char| {
            if (char == 10 or char == 13) {
                continue;
            }
            try battery_line.append(allocator, @intFromBool(char == '@'));
        }
        try batteries.append(allocator, try battery_line.toOwnedSlice(allocator));
    }

    return batteries.toOwnedSlice(allocator);
}

pub fn part1(map: [][]u8) !void {
    var total_accessible: u32 = 0;

    for (0.., map) |row, line| {
        for (0.., line) |col, value| {
            if (value == 0) {
                continue;
            }

            var n_invalid: usize = 0;
            for (0..3) |d_row| {
                var row_offseted: i32 = @intCast(row + d_row);
                row_offseted = row_offseted - 1;
                if (row_offseted < 0 or row_offseted >= map.len) {
                    continue;
                }
                for (0..3) |d_col| {
                    if (d_row == 1 and d_col == 1) continue;
                    var col_offseted: i32 = @intCast(col + d_col);
                    col_offseted = col_offseted - 1;
                    if (col_offseted < 0 or col_offseted >= map.len) {
                        continue;
                    }

                    n_invalid += map[@intCast(row_offseted)][@intCast(col_offseted)];
                }
            }

            if (n_invalid < 4) {
                total_accessible += 1;
            }
        }
    }

    std.debug.print("Part 1: {d}\n", .{total_accessible});
}

pub fn part2(map: [][]u8) !void {
    var total_removed: u32 = 0;

    var updated = true;
    while (updated) {
        updated = false;

        for (0.., map) |row, line| {
            for (0.., line) |col, value| {
                if (value == 0) {
                    continue;
                }

                var n_invalid: usize = 0;
                for (0..3) |d_row| {
                    var row_offseted: i32 = @intCast(row + d_row);
                    row_offseted = row_offseted - 1;
                    if (row_offseted < 0 or row_offseted >= map.len) {
                        continue;
                    }
                    for (0..3) |d_col| {
                        if (d_row == 1 and d_col == 1) continue;
                        var col_offseted: i32 = @intCast(col + d_col);
                        col_offseted = col_offseted - 1;
                        if (col_offseted < 0 or col_offseted >= map.len) {
                            continue;
                        }

                        n_invalid += map[@intCast(row_offseted)][@intCast(col_offseted)];
                    }
                }

                if (n_invalid < 4) {
                    total_removed += 1;
                    map[row][col] = 0;
                    updated = true;
                }
            }
        }
    }

    std.debug.print("Part 2: {d}\n", .{total_removed});
}

pub fn main() !void {
    const allocator = std.heap.smp_allocator;

    const map = try read_map(allocator, "data/04.txt");

    try part1(map);
    try part2(map);
}
