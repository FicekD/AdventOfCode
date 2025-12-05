const std = @import("std");

const Range = struct {
    start: u64,
    stop: u64,
    pub fn is_in(self: *const Range, val: u64) bool {
        return (val >= self.start and val <= self.stop);
    }
};
const RangesAndItems = struct { ranges: []Range, items: []u64 };

pub fn range_start_less_than(_: void, r1: Range, r2: Range) bool {
    return r1.start < r2.start;
}

pub fn read_list(
    allocator: std.mem.Allocator,
    path: []const u8,
) !RangesAndItems {
    const file = try std.fs.cwd().openFile(path, .{});
    defer file.close();

    var ranges: std.ArrayList(Range) = .empty;
    defer ranges.deinit(allocator);

    var file_buffer: [4096]u8 = undefined;
    var reader = file.reader(&file_buffer);

    while (try reader.interface.takeDelimiter('\n')) |line| {
        if (line.len == 1) {
            break;
        }

        var split = std.mem.splitScalar(u8, line[0 .. line.len - 1], '-');

        const start = try std.fmt.parseUnsigned(u64, split.next().?, 10);
        const stop = try std.fmt.parseUnsigned(u64, split.next().?, 10);
        try ranges.append(allocator, .{ .start = start, .stop = stop });
    }

    var numbers: std.ArrayList(u64) = .empty;
    defer numbers.deinit(allocator);

    while (try reader.interface.takeDelimiter('\n')) |line| {
        const trimmed = std.mem.trim(u8, line, " \t\r\n");
        const number = try std.fmt.parseUnsigned(u64, trimmed, 10);

        try numbers.append(allocator, number);
    }

    return .{ .ranges = try ranges.toOwnedSlice(allocator), .items = try numbers.toOwnedSlice(allocator) };
}

pub fn part1(ranges_and_items: RangesAndItems) !void {
    var total_fresh: u64 = 0;
    for (ranges_and_items.items) |item| {
        var fresh = false;
        for (ranges_and_items.ranges) |range| {
            if (range.is_in(item)) {
                fresh = true;
                break;
            }
        }
        total_fresh += @intFromBool(fresh);
    }
    std.debug.print("Part 1: {d}\n", .{total_fresh});
}

pub fn part2(allocator: std.mem.Allocator, ranges_and_items: RangesAndItems) !void {
    std.mem.sort(Range, ranges_and_items.ranges, {}, comptime range_start_less_than);

    var merged_ranges: std.ArrayList(Range) = .empty;
    defer merged_ranges.deinit(allocator);

    var current = ranges_and_items.ranges[0];
    for (ranges_and_items.ranges[1..]) |range| {
        if (range.start <= current.stop) {
            current.stop = @max(current.stop, range.stop);
        } else {
            try merged_ranges.append(allocator, current);
            current = range;
        }
    }
    try merged_ranges.append(allocator, current);

    var total_fresh: u64 = 0;

    for (try merged_ranges.toOwnedSlice(allocator)) |range| {
        total_fresh += range.stop - range.start + 1;
    }

    std.debug.print("Part 2: {d}\n", .{total_fresh});
}

pub fn main() !void {
    const allocator = std.heap.smp_allocator;

    const ranges_and_items = try read_list(allocator, "data/05.txt");

    try part1(ranges_and_items);
    try part2(allocator, ranges_and_items);
}
