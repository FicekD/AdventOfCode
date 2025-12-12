const std = @import("std");

const Point = struct { x: i64, y: i64 };
const Area = struct { idx0: u64, idx1: u64, area: u64 };

pub fn area_greater_than(_: void, a1: Area, a2: Area) bool {
    return a1.area > a2.area;
}

pub fn read_points(
    allocator: std.mem.Allocator,
    path: []const u8,
) ![]Point {
    const file = try std.fs.cwd().openFile(path, .{});
    defer file.close();

    var points: std.ArrayList(Point) = .empty;
    defer points.deinit(allocator);

    var file_buffer: [4096]u8 = undefined;
    var reader = file.reader(&file_buffer);

    while (try reader.interface.takeDelimiter('\n')) |line| {
        const trimmed = std.mem.trim(u8, line, " \r\n");
        var split = std.mem.splitScalar(u8, trimmed, ',');

        const x = try std.fmt.parseUnsigned(i64, split.next().?, 10);
        const y = try std.fmt.parseUnsigned(i64, split.next().?, 10);

        try points.append(allocator, .{ .x = x, .y = y });
    }

    return points.toOwnedSlice(allocator);
}

pub fn part1(points: []Point) !void {
    var max_area: u64 = 0;

    for (0.., points[0 .. points.len - 1]) |i, p1| {
        for (points[i..]) |p2| {
            const area = @abs(p1.x - p2.x + 1) * @abs(p1.y - p2.y + 1);

            if (area > max_area) {
                max_area = area;
            }
        }
    }

    std.debug.print("Part 1: {d}\n", .{max_area});
}

pub fn part2(allocator: std.mem.Allocator, points: []Point) !void {
    var areas: std.ArrayList(Area) = .empty;
    defer areas.deinit(allocator);

    for (0..points.len - 1) |i| {
        for (i + 1..points.len) |j| {
            const area = (@abs(points[i].x - points[j].x) + 1) * (@abs(points[i].y - points[j].y) + 1);
            try areas.append(allocator, .{ .idx0 = i, .idx1 = j, .area = area });
        }
    }

    std.mem.sort(Area, areas.items, {}, area_greater_than);

    var max_area: u64 = 0;

    for (areas.items) |a| {
        const pi = points[a.idx0];
        const pj = points[a.idx1];
        const area = a.area;

        if (area < max_area) continue;

        var p1: Point = undefined;
        var p2: Point = undefined;
        var p3: Point = undefined;
        var p4: Point = undefined;

        // p1---------p2
        // |          |
        // |          |
        // p3---------p4

        if (pi.x <= pj.x) {
            if (pi.y <= pj.y) {
                p1 = pi;
                p2 = .{ .x = pj.x, .y = pi.y };
                p3 = .{ .x = pi.x, .y = pj.y };
                p4 = pj;
            } else {
                p1 = .{ .x = pi.x, .y = pj.y };
                p2 = pj;
                p3 = pi;
                p4 = .{ .x = pj.x, .y = pi.y };
            }
        } else {
            if (pi.y <= pj.y) {
                p1 = .{ .x = pj.x, .y = pi.y };
                p2 = pi;
                p3 = pj;
                p4 = .{ .x = pi.x, .y = pj.y };
            } else {
                p1 = pj;
                p2 = .{ .x = pi.x, .y = pj.y };
                p3 = .{ .x = pj.x, .y = pi.y };
                p4 = pi;
            }
        }

        var valid = true;

        for (0..points.len) |idx0| {
            const idx1 = @mod(idx0 + 1, points.len);

            var edge_start: Point = undefined;
            var edge_end: Point = undefined;

            if (points[idx0].y < points[idx1].y) {
                edge_start = points[idx0];
                edge_end = points[idx1];
            } else {
                edge_start = points[idx1];
                edge_end = points[idx0];
            }

            if (edge_start.y == edge_end.y) {} else {
                if (edge_start.x > p1.x and edge_start.x < p2.x) {
                    if (edge_start.y == p1.y) {
                        valid = false;
                        break;
                    } else if (edge_end.y == p1.y) {
                        //
                    } else if (edge_start.y < p1.y and edge_end.y > p1.y) {
                        valid = false;
                        break;
                    }

                    if (edge_start.y == p3.y) {
                        //
                    } else if (edge_end.y == p3.y) {
                        valid = false;
                        break;
                    } else if (edge_start.y < p3.y and edge_end.y > p3.y) {
                        valid = false;
                        break;
                    }
                }
            }
        }
        if (!valid) continue;

        for (0..points.len) |idx0| {
            const idx1 = @mod(idx0 + 1, points.len);

            var edge_start: Point = undefined;
            var edge_end: Point = undefined;

            if (points[idx0].x < points[idx1].x) {
                edge_start = points[idx0];
                edge_end = points[idx1];
            } else {
                edge_start = points[idx1];
                edge_end = points[idx0];
            }

            if (edge_start.x == edge_end.x) {} else {
                if (edge_start.y > p1.y and edge_start.y < p3.y) {
                    if (edge_start.x == p1.x) {
                        valid = false;
                        break;
                    } else if (edge_end.x == p1.x) {
                        //
                    } else if (edge_start.x < p1.x and edge_end.x > p1.x) {
                        valid = false;
                        break;
                    }

                    if (edge_start.x == p2.x) {
                        //
                    } else if (edge_end.x == p2.x) {
                        valid = false;
                        break;
                    } else if (edge_start.x < p2.x and edge_end.x > p2.x) {
                        valid = false;
                        break;
                    }
                }
            }
        }
        if (!valid) continue;

        if (valid) {
            max_area = area;
        }
    }

    std.debug.print("Part 2: {d}\n", .{max_area});
}

pub fn main() !void {
    const allocator = std.heap.smp_allocator;

    const points = try read_points(allocator, "data/09.txt");

    try part1(points);
    try part2(allocator, points);
}
