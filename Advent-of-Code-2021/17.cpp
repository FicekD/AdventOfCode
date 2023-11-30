#include <iostream>
#include <fstream>
#include <sstream>

#include <cmath>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <regex>

enum ProbeState {
    UNDER,
    INSIDE,
    OVER
};

class Point {
public:
    int x = 0;
    int y = 0;
    Point() : x(0), y(0) {}
    Point(const Point& point) : x(point.x), y(point.y) {}
    Point(int x_pos, int y_pos) : x(x_pos), y(y_pos) {}
    void operator+=(const Point& point) {
        x += point.x;
        y += point.y;
    }
};

class Interval {
public:
    Point pt1 = Point();
    Point pt2 = Point();
    Interval() : pt1(Point()), pt2(Point()) {}
    Interval(const Interval& interval) : pt1(interval.pt1), pt2(interval.pt2) {}
    Interval(const Point& x1y1, const Point& x2y2) : pt1(x1y1), pt2(x2y2) {}
    ProbeState check_point(const Point& point) const {
        if (point.x >= pt1.x && point.y >= pt1.y && point.x <= pt2.x && point.y <= pt2.y) return INSIDE;
        else if (point.x >= pt2.x || point.y <= pt1.y) return OVER;
        else return UNDER;
    }
};

Interval read_data() {
    std::ifstream infile("data/17_data.txt");
    std::string line;
    std::getline(infile, line);

    std::regex re("target area: x=(-?\\d+)\\.\\.(-?\\d+), y=(-?\\d+)\\.\\.(-?\\d+)");
    std::smatch m;
    std::regex_match(line, m, re);
    return Interval(Point(std::stoi(m[1]), std::stoi(m[3])), Point(std::stoi(m[2]), std::stoi(m[4])));
}

int simulate_velocity(const Point& v0, const Interval& target_interval) {
    Point point(0, 0);
    Point velocity(v0);
    ProbeState state = UNDER;
    bool state_crossed_interval = false;
    int highest_position = 0;
    while (state != OVER) {
        point += velocity;
        if (point.y > highest_position) highest_position = point.y;
        if (velocity.x != 0) velocity.x += velocity.x < 0 ? 1 : -1;
        velocity.y -= 1;
        state = target_interval.check_point(point);
        if (state == INSIDE) state_crossed_interval = true;
        // std::cout << point.x << 'x' << point.y << ' ' << state << std::endl;
    }
    return state_crossed_interval ? highest_position : 0;
}

int explore_velocity_space(const Point& min_velocity, const Point& max_velocity, const Interval& interval) {
    int max_height = 0;
    for (int v_x = min_velocity.x; v_x <= max_velocity.x; v_x++) {
        for (int v_y = min_velocity.y; v_y >= max_velocity.y; v_y--) {
            // std::cout << v_x << ' ' << v_y << std::endl;
            int height = simulate_velocity(Point(v_x, v_y), interval);
            if (height > max_height) max_height = height;
        }
    }
    return max_height;
}

int main() {
    Interval target_interval = read_data();

    int highest_point = explore_velocity_space(Point(0, 10), Point(target_interval.pt2.x, target_interval.pt1.y), target_interval);
    std::cout << highest_point << std::endl;

    return 0;
}