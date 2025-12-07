const std = @import("std");

const TableAligned = struct { numbers: [][][]u8, signs: []u8 };

pub fn contains(string: []const u8, char: u8) bool {
    for (string) |val| {
        if (val == char) return true;
    }
    return false;
}

pub fn read_table_aligned(
    allocator: std.mem.Allocator,
    path: []const u8,
) !TableAligned {
    var n_number_lines: u64 = 0;

    var signs_list: std.ArrayList(u8) = .empty;
    defer signs_list.deinit(allocator);

    var signs_indices_list: std.ArrayList(u64) = .empty;
    defer signs_indices_list.deinit(allocator);

    {
        const file = try std.fs.cwd().openFile(path, .{});
        defer file.close();

        var file_buffer: [4096]u8 = undefined;
        var reader = file.reader(&file_buffer);

        while (try reader.interface.takeDelimiter('\n')) |line| {
            if (!contains(line, '*')) {
                n_number_lines += 1;
                continue;
            } else {
                for (0.., line) |i, char| {
                    if (char == '+' or char == '*') {
                        try signs_list.append(allocator, char);
                        try signs_indices_list.append(allocator, i);
                    }
                }
            }
        }
    }

    const signs = try signs_list.toOwnedSlice(allocator);
    const signs_indices = try signs_indices_list.toOwnedSlice(allocator);

    {
        const file = try std.fs.cwd().openFile(path, .{});
        defer file.close();

        var file_buffer: [4096]u8 = undefined;
        var reader = file.reader(&file_buffer);

        var numbers: std.ArrayList([][]const u8) = .empty;
        defer numbers.deinit(allocator);

        var line_i: u64 = 0;
        while (try reader.interface.takeDelimiter('\n')) |line| {
            if (line_i == n_number_lines) {
                return .{ .numbers = try numbers.toOwnedSlice(allocator), .signs = signs };
            }

            const trimmed = std.mem.trim(u8, line, "\r\n");

            var numbers_line: std.ArrayList([]const u8) = .empty;
            defer numbers_line.deinit(allocator);

            for (0..signs_indices.len - 1) |i| {
                const number = trimmed[signs_indices[i] .. signs_indices[i + 1] - 1];
                const buf = try allocator.alloc(u8, number.len);
                std.mem.copyForwards(u8, buf, number);

                try numbers_line.append(allocator, buf);
            }
            const number = trimmed[signs_indices[signs_indices.len - 1]..];
            const buf = try allocator.alloc(u8, number.len);
            std.mem.copyForwards(u8, buf, number);

            try numbers_line.append(allocator, buf);
            try numbers.append(allocator, try numbers_line.toOwnedSlice(allocator));

            line_i += 1;
        }
    }

    unreachable;
}

pub fn aligned_operation(numbers: [][]const u8, operator: u8) !u64 {
    var total: u64 = 0;

    if (operator == '*') total = 1;

    for (0..numbers[0].len) |i| {
        var cum_number: u64 = 0;

        var j: u64 = 0;
        for (0..numbers.len) |number_i| {
            const number = numbers[numbers.len - 1 - number_i];
            if (number[i] != ' ') {
                const pow_coeff = try std.math.powi(u64, 10, j);

                cum_number += (@as(u64, number[i] - 48)) * pow_coeff;
                j += 1;
            }
        }

        if (operator == '+') {
            total += cum_number;
        } else if (operator == '*') {
            total *= cum_number;
        } else unreachable;
    }

    return total;
}

pub fn part1(table: TableAligned) !void {
    var total_result: u64 = 0;
    for (0..table.signs.len) |col| {
        var cum_result: u64 = @intFromBool(table.signs[col] == '*');
        for (0..table.numbers.len) |row| {
            const value = try std.fmt.parseUnsigned(u64, std.mem.trim(u8, table.numbers[row][col], " "), 10);
            if (table.signs[col] == '+') {
                cum_result += value;
            } else if (table.signs[col] == '*') {
                cum_result *= value;
            } else unreachable;
        }

        total_result += cum_result;
    }

    std.debug.print("Part 1: {d}\n", .{total_result});
}

pub fn part2(allocator: std.mem.Allocator, table: TableAligned) !void {
    var total_result: u64 = 0;
    for (0..table.signs.len) |col| {
        var numbers_col: std.ArrayList([]const u8) = .empty;
        defer numbers_col.deinit(allocator);

        for (0..table.numbers.len) |row| {
            try numbers_col.append(allocator, table.numbers[row][col]);
        }

        total_result += try aligned_operation(try numbers_col.toOwnedSlice(allocator), table.signs[col]);
    }

    std.debug.print("Part 2: {d}\n", .{total_result});
}

pub fn main() !void {
    const allocator = std.heap.smp_allocator;

    const table = try read_table_aligned(allocator, "data/06.txt");

    try part1(table);
    try part2(allocator, table);
}
