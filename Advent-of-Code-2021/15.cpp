#include <iostream>
#include <fstream>
#include <sstream>

#include <cmath>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <queue>
#include <set>

#include "include/tensor.hpp"

template<class T, class C = std::vector<T>, class P = std::less<typename C::value_type> >
struct heapq : std::priority_queue<T, C, P> {
    typename C::iterator begin() { return std::priority_queue<T, C, P>::c.begin(); }
    typename C::iterator end() { return std::priority_queue<T, C, P>::c.end(); }
};

class Point {
public:
    int x = 0, y = 0;
    Point() {}
    Point(int px, int py) : x(px), y(py) {}
    int flatten(int x_boundary) const { return x + y * x_boundary; }
    Point operator+(const Point& rval) const { return Point(rval.x + this->x, rval.y + this->y); }
    bool operator==(const Point& rval) const { return this->x == rval.x && this->y == rval.y; }
    bool operator<(const Point& rval) const { return this->x + this->y < rval.x + rval.y; }
};

class Node {
public:
    Point pos;
    Node* parent = nullptr;
    int cost = 0;
    Node() {}
    Node(Point node_position, Node* node_parent, int node_cost) : pos(node_position), parent(node_parent), cost(node_cost) {}
    bool operator==(const Node& rval) const { return this->pos == rval.pos; }
    bool operator<(const Node& rval) const { return this->cost < rval.cost; }
    bool operator>(const Node& rval) const { return this->cost > rval.cost; }
};

void read_data(std::vector<std::vector<int>>& data) {
    std::ifstream infile("data/15_data.txt");
    std::string line;

    while(std::getline(infile, line)) {
        std::vector<int> row;
        for (auto& c : line) {
            row.push_back(int(c) - 48);
        }
        data.push_back(row);
    }
}

int safest_path(const tensor::Tensor<int>& danger_map, const Point& start, const Point& end) {
    unsigned dims[3];
    danger_map.shape(dims);

    std::vector<Point> directions = {Point(1, 0), Point(0, 1), Point(-1, 0), Point(0, -1)};
    heapq<Node, std::vector<Node>, std::greater<Node>> open_list;
    open_list.push(Node(start, nullptr, 0));
    std::set<int> closed_list;

    Node current_node;
    while (open_list.size() > 0) {
        current_node = open_list.top();
        open_list.pop();
        closed_list.insert(current_node.pos.flatten(dims[1]));
        if (current_node.pos == end) {
            break;
        }
        for (auto& dir : directions) {
            Point new_pos = current_node.pos + dir;
            if (new_pos.x < 0 || new_pos.x >= dims[1] || new_pos.y < 0 || new_pos.y >= dims[0]) continue;
            if (closed_list.find(new_pos.flatten(dims[1])) != closed_list.end()) continue;
            Node new_node(new_pos, &current_node, danger_map(new_pos.y, new_pos.x, 0) + current_node.cost);
            if (std::find(open_list.begin(), open_list.end(), new_node) != open_list.end()) continue;
            open_list.push(new_node);
        }
    }
    return current_node.cost;
}


int main() {
    std::vector<std::vector<int>> data;
    read_data(data);
    
    int width = data[0].size(), height = data.size();
    tensor::Tensor<int> danger_map(height, width, 1);
    for (int row = 0; row < data.size(); row++) {
        for (int col = 0; col < data[0].size(); col++) {
            danger_map(row, col, 0) = data[row][col];
        }
    }
    
    Point start(0, 0), end(width - 1, height - 1);
    int shortest_path_pt1 = safest_path(danger_map, start, end);
    std::cout << shortest_path_pt1 << std::endl;

    tensor::Tensor<int> danger_map_expanded(5 * height, 5 * width, 1);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            int abs_dist = i + j;
            for (int r = 0; r < height; r++) {
                for (int c = 0; c < width; c++) {
                    danger_map_expanded(i * height + r, j * width + c, 0) = 1 + (danger_map(r, c, 0) + abs_dist - 1) % 9;
                }
            }
        }
    }
    end = Point(5 * width - 1, 5 * height - 1);
    int shortest_path_pt2 = safest_path(danger_map_expanded, start, end);
    std::cout << shortest_path_pt2 << std::endl;

    return 0;
}