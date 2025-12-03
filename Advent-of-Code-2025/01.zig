const std = @import("std");

const Instruction = struct { direction: i32, length: u32 };

pub fn read_instructions(
    allocator: std.mem.Allocator,
    path: []const u8,
) ![]Instruction {
    const file = try std.fs.cwd().openFile(path, .{});
    defer file.close();

    var instructions: std.ArrayList(Instruction) = .empty;
    defer instructions.deinit(allocator);

    var file_buffer: [4096]u8 = undefined;
    var reader = file.reader(&file_buffer);

    while (try reader.interface.takeDelimiter('\n')) |line| {
        var direction: i32 = 0;
        if (line[0] == 'L') {
            direction = -1;
        } else {
            direction = 1;
        }

        const trimmed = std.mem.trim(u8, line[1..], " \t\r\n");
        const number = try std.fmt.parseUnsigned(u32, trimmed, 10);

        try instructions.append(allocator, .{ .direction = direction, .length = number });
    }

    return instructions.toOwnedSlice(allocator);
}

pub fn part1(instructions: []Instruction) void {
    var position: i32 = 50;
    var counter: u32 = 0;

    for (instructions) |instruction| {
        position += instruction.direction * @as(i32, @intCast(instruction.length));
        position = @mod(position, 100);

        if (position == 0) {
            counter += 1;
        }
    }

    std.debug.print("Part 1: {d}\n", .{counter});
}

pub fn part2(instructions: []Instruction) void {
    var position: i32 = 50;
    var counter: u32 = 0;

    for (instructions) |instruction| {
        const started_at_zero = position == 0;

        position += instruction.direction * @as(i32, @intCast(instruction.length));

        if (position >= 100 or position <= 0) {
            counter += @abs(position) / 100 + @intFromBool(position <= 0 and !started_at_zero);
        }
        position = @mod(position, 100);
    }

    std.debug.print("Part 2: {d}\n", .{counter});
}

pub fn main() !void {
    const allocator = std.heap.smp_allocator;

    const instructions = try read_instructions(allocator, "data/01.txt");

    part1(instructions);
    part2(instructions);
}
