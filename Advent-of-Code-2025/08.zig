const std = @import("std");

const Circuit = struct {
    list: std.ArrayList(u64),

    pub fn init(allocator: std.mem.Allocator) !*Circuit {
        const self = try allocator.create(Circuit);
        self.* = .{
            .list = .empty,
        };
        return self;
    }

    pub fn deinit(self: *Circuit, allocator: std.mem.Allocator) void {
        self.list.deinit(allocator);
        allocator.destroy(self);
    }
};
const Box = struct { x: f64, y: f64, z: f64, circuit: ?*Circuit };
const Distance = struct { i: u64, j: u64, distance: f64 };

pub fn distance_less_than(_: void, d1: Distance, d2: Distance) bool {
    return d1.distance < d2.distance;
}

pub fn circuit_greater_than(_: void, c1: *Circuit, c2: *Circuit) bool {
    return c1.list.items.len > c2.list.items.len;
}

pub fn read_boxes(
    allocator: std.mem.Allocator,
    path: []const u8,
) ![]Box {
    const file = try std.fs.cwd().openFile(path, .{});
    defer file.close();

    var points: std.ArrayList(Box) = .empty;
    defer points.deinit(allocator);

    var file_buffer: [4096]u8 = undefined;
    var reader = file.reader(&file_buffer);

    while (try reader.interface.takeDelimiter('\n')) |line| {
        const trimmed = std.mem.trim(u8, line, " \r\n");
        var split = std.mem.splitScalar(u8, trimmed, ',');

        const x: f64 = @floatFromInt(try std.fmt.parseUnsigned(u64, split.next().?, 10));
        const y: f64 = @floatFromInt(try std.fmt.parseUnsigned(u64, split.next().?, 10));
        const z: f64 = @floatFromInt(try std.fmt.parseUnsigned(u64, split.next().?, 10));
        try points.append(allocator, .{ .x = x, .y = y, .z = z, .circuit = null });
    }

    return points.toOwnedSlice(allocator);
}

pub fn get_distances(allocator: std.mem.Allocator, boxes: []Box) ![]Distance {
    var distances: std.ArrayList(Distance) = .empty;
    defer distances.deinit(allocator);

    for (0.., boxes[0 .. boxes.len - 1]) |i, bi| {
        for (i + 1.., boxes[i + 1 ..]) |j, bj| {
            const distance = std.math.sqrt(std.math.pow(f64, bi.x - bj.x, 2) +
                std.math.pow(f64, bi.y - bj.y, 2) +
                std.math.pow(f64, bi.z - bj.z, 2));
            try distances.append(allocator, .{ .i = i, .j = j, .distance = distance });
        }
    }

    std.mem.sort(Distance, distances.items, {}, comptime distance_less_than);

    return try distances.toOwnedSlice(allocator);
}

pub fn get_unique_circuits(allocator: std.mem.Allocator, boxes: []Box) ![]*Circuit {
    var unique_circuits: std.ArrayList(*Circuit) = .empty;
    defer unique_circuits.deinit(allocator);

    for (boxes) |box| {
        if (box.circuit == null) continue;

        var contains = false;
        for (unique_circuits.items) |c| {
            if (box.circuit.? == c) {
                contains = true;
                break;
            }
        }
        if (!contains) {
            try unique_circuits.append(allocator, box.circuit.?);
        }
    }

    std.mem.sort(*Circuit, unique_circuits.items, {}, comptime circuit_greater_than);

    return try unique_circuits.toOwnedSlice(allocator);
}

pub fn part1(allocator: std.mem.Allocator, boxes: []Box) !void {
    var distances = try get_distances(allocator, boxes);

    for (distances[0..1000]) |distance| {
        const i = distance.i;
        const j = distance.j;

        var b1 = &boxes[i];
        var b2 = &boxes[j];

        if (b1.circuit == null and b2.circuit == null) {
            var c = try Circuit.init(allocator);
            try c.list.append(allocator, i);
            try c.list.append(allocator, j);

            b1.circuit = c;
            b2.circuit = c;
        } else if (b1.circuit == null) {
            try b2.circuit.?.list.append(allocator, i);
            b1.circuit = b2.circuit;
        } else if (b2.circuit == null) {
            try b1.circuit.?.list.append(allocator, j);
            b2.circuit = b1.circuit;
        } else {
            if (b1.circuit != b2.circuit) {
                const c1 = b1.circuit.?;
                const c2 = b2.circuit.?;

                try c1.list.appendSlice(allocator, c2.list.items);

                for (c2.list.items) |idx| {
                    boxes[idx].circuit = c1;
                }

                c2.deinit(allocator);
            }
        }
    }

    const unique_circuits = try get_unique_circuits(allocator, boxes);

    var result: u64 = 1;
    for (0..3) |i| {
        result *= unique_circuits[i].list.items.len;
    }

    for (unique_circuits) |circuit| {
        circuit.deinit(allocator);
    }

    for (boxes) |*box| {
        box.circuit = null;
    }

    std.debug.print("Part 1: {d}\n", .{result});
}

pub fn part2(allocator: std.mem.Allocator, boxes: []Box) !void {
    const distances = try get_distances(allocator, boxes);

    for (distances) |distance| {
        const i = distance.i;
        const j = distance.j;

        var b1 = &boxes[i];
        var b2 = &boxes[j];

        if (b1.circuit == null and b2.circuit == null) {
            var c = try Circuit.init(allocator);
            try c.list.append(allocator, i);
            try c.list.append(allocator, j);

            b1.circuit = c;
            b2.circuit = c;
        } else if (b1.circuit == null) {
            try b2.circuit.?.list.append(allocator, i);
            b1.circuit = b2.circuit;
        } else if (b2.circuit == null) {
            try b1.circuit.?.list.append(allocator, j);
            b2.circuit = b1.circuit;
        } else {
            if (b1.circuit != b2.circuit) {
                const c1 = b1.circuit.?;
                const c2 = b2.circuit.?;

                try c1.list.appendSlice(allocator, c2.list.items);

                for (c2.list.items) |idx| {
                    boxes[idx].circuit = c1;
                }

                c2.deinit(allocator);
            }
        }

        if (boxes[0].circuit != null and boxes[0].circuit.?.list.items.len == boxes.len) {
            std.debug.print("Part 2: {d}\n", .{b1.x * b2.x});
            break;
        }
    }

    const unique_circuits = try get_unique_circuits(allocator, boxes);

    for (unique_circuits) |circuit| {
        circuit.deinit(allocator);
    }

    for (boxes) |*box| {
        box.circuit = null;
    }
}

pub fn main() !void {
    const allocator = std.heap.smp_allocator;

    const boxes = try read_boxes(allocator, "data/08.txt");

    try part1(allocator, boxes);
    try part2(allocator, boxes);
}
