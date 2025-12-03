const std = @import("std");

const Range = struct { start: u64, stop: u64 };

pub fn read_ranges(
    allocator: std.mem.Allocator,
    path: []const u8,
) ![]Range {
    const file = try std.fs.cwd().openFile(path, .{});
    defer file.close();

    var ranges: std.ArrayList(Range) = .empty;
    defer ranges.deinit(allocator);

    var file_buffer: [4096]u8 = undefined;
    var reader = file.reader(&file_buffer);

    while (try reader.interface.takeDelimiter(',')) |line| {
        var split = std.mem.splitScalar(u8, line, '-');

        const start = try std.fmt.parseUnsigned(u64, split.next().?, 10);
        const stop = try std.fmt.parseUnsigned(u64, split.next().?, 10);
        try ranges.append(allocator, .{ .start = start, .stop = stop });
    }

    return ranges.toOwnedSlice(allocator);
}

pub fn part1(ranges: []Range) !void {
    var total: u64 = 0;

    for (ranges) |range| {
        for (range.start..range.stop + 1) |val| {
            var buffer: [64]u8 = undefined;
            const string_val = try std.fmt.bufPrint(&buffer, "{d}", .{val});

            if (@mod(string_val.len, 2) == 1) {
                continue;
            }

            if (std.mem.eql(u8, string_val[0..(string_val.len / 2)], string_val[(string_val.len / 2)..])) {
                total += @as(u64, val);
            }
        }
    }

    std.debug.print("Part 1: {d}\n", .{total});
}

pub fn part2(ranges: []Range) !void {
    var total: u64 = 0;

    for (ranges) |range| {
        for (range.start..range.stop + 1) |val| {
            var buffer: [64]u8 = undefined;
            const string_val = try std.fmt.bufPrint(&buffer, "{d}", .{val});

            var valid = true;

            var split_len: usize = 1;
            while (split_len <= string_val.len / 2) {
                if (@mod(string_val.len, split_len) == 0) {
                    const first_slice = string_val[0..split_len];

                    valid = false;

                    for (1..string_val.len / split_len) |i| {
                        const second_slice = string_val[i * split_len .. (i + 1) * split_len];

                        if (!std.mem.eql(u8, first_slice, second_slice)) {
                            valid = true;
                            break;
                        }
                    }
                    if (!valid) {
                        break;
                    }
                }
                split_len += 1;
            }

            if (!valid) {
                total += @as(u64, val);
            }
        }
    }

    std.debug.print("Part 2: {d}\n", .{total});
}

pub fn main() !void {
    const allocator = std.heap.smp_allocator;

    const ranges = try read_ranges(allocator, "data/02.txt");

    try part1(ranges);
    try part2(ranges);
}
