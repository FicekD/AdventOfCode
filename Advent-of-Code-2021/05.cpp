#include <iostream>
#include <fstream>
#include <sstream>

#include <cmath>
#include <string>
#include <array>
#include <vector>

#include "include/tensor.hpp"


void read_data(std::vector<std::pair<int, int>>& points_1, std::vector<std::pair<int, int>>& points_2) {
    std::ifstream infile("data/05_data.txt");
    std::string line;

    while(std::getline(infile, line)) {
        std::pair<int, int> pt1, pt2;
        char delim;
        std::istringstream iss(line);
        iss >> pt1.first >> delim >> pt1.second >> delim >> delim >> pt2.first >> delim >> pt2.second;
        points_1.push_back(pt1);
        points_2.push_back(pt2);
    }
}


void find_biggest_first_second(std::vector<std::pair<int, int>> data, int* biggest_vals) {
    biggest_vals[0] = 0;
    biggest_vals[1] = 0;
    for (auto& val : data) {
        if (val.first > biggest_vals[0]) biggest_vals[0] = val.first;
        if (val.second > biggest_vals[1]) biggest_vals[1] = val.second;
    }
}


int main() {
    std::vector<std::pair<int, int>> pt1, pt2;
    read_data(pt1, pt2);

    int dims_1[2];
    find_biggest_first_second(pt1, dims_1);
    int dims_2[2];
    find_biggest_first_second(pt2, dims_2);

    tensor::Tensor<int> data((dims_1[1] > dims_2[1] ? dims_1[1] : dims_2[1])+1, (dims_1[0] > dims_2[0] ? dims_1[0] : dims_2[0])+1, 1);

    for (int i = 0; i < pt1.size(); i++) {
        // uncommented -> 1st task, commented -> 2nd task
        // if ((pt1[i].first != pt2[i].first) && (pt1[i].second != pt2[i].second)) continue;
        bool x_upwards = pt2[i].first > pt1[i].first, y_upwards = pt2[i].second > pt1[i].second;
        for (int x = pt1[i].first, y = pt1[i].second;;) {
            data(y, x, 0)++;
            if ((x == pt2[i].first) && (y == pt2[i].second)) break;
            if (x != pt2[i].first) x_upwards ? x++ : x--;
            if (y != pt2[i].second) y_upwards ? y++ : y--;
        }
    }
    int sum = 0;
    for (int i = 0; i < data.size(); i++) if (data(i) > 1) sum++;

    std::cout << sum << std::endl;

    return 0;
}