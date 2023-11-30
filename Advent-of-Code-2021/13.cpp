#include <iostream>
#include <fstream>
#include <sstream>

#include <cmath>
#include <string>
#include <vector>
#include <array>
#include <algorithm>

#include "include/tensor.hpp"


void read_data(std::vector<std::pair<int, int>>& dots, std::vector<std::pair<int, int>>& folds) {
    std::ifstream infile("data/13_data.txt");
    std::string line;

    while(std::getline(infile, line)) {
        if (line.length() == 0) break;
        std::istringstream iss_in(line);
        int x, y;
        char delim;
        iss_in >> x >> delim >> y;
        dots.push_back(std::pair<int, int>(x, y));
    }
    std::string prefix("fold along ");
    while(std::getline(infile, line)) {
        line.erase(0, prefix.length());
        char axis, delim;
        int val;
        std::istringstream iss_in(line);
        iss_in >> axis >> delim >> val;
        folds.push_back(std::pair<int, int>(axis == 'x' ? 1 : 0, val));
    }
}


void split_tensors(const tensor::Tensor<int>& source, tensor::Tensor<int>& dest1, tensor::Tensor<int>& dest2, int axis, int skip_idx) {
    for (int col = 0; col < source.y_size(); col++) {
        if (axis == 1 && col == skip_idx) continue;
        for (int row = 0; row < source.x_size(); row++) {
            if (axis == 0 && row == skip_idx) continue;
            if (col >= dest1.y_size()) dest2(row, col - dest1.y_size() - int(axis == 1), 0) = source(row, col, 0);
            else if (row >= dest1.x_size()) dest2(row - dest1.x_size() - int(axis == 0), col, 0) = source(row, col, 0);
            else dest1(row, col, 0) = source(row, col, 0);
        }
    }
}


int main() {
    std::vector<std::pair<int, int>> dots, folds;
    read_data(dots, folds);

    int max_x = std::max_element(dots.begin(), dots.end(), [](const std::pair<int, int>& pair_l, const std::pair<int, int>& pair_r) { return pair_l.first < pair_r.first; })->first;
    int max_y = std::max_element(dots.begin(), dots.end(), [](const std::pair<int, int>& pair_l, const std::pair<int, int>& pair_r) { return pair_l.second < pair_r.second; })->second;
    tensor::Tensor<int> tensor_map(max_y+1, max_x+1, 1);
    for (auto& dot : dots) {
        tensor_map(dot.second, dot.first, 0) = 1;
    }
    // std::cout << tensor_map << std::endl;
    
    for (auto& fold : folds) {
        std::cout << fold.first << ':' << fold.second << std::endl;
        if (fold.first == 0) {
            tensor::Tensor<int> folded(fold.second, tensor_map.y_size(), 1);
            for (int row = 0; row < folded.x_size(); row++) {
                for (int col = 0; col < folded.y_size(); col++) {
                    // std::cout << row << ':' << col << std::endl;
                    folded(row, col, 0) = tensor_map(row, col, 0) + tensor_map(tensor_map.x_size() - 1 - row, col, 0);
                }
            }
            // for (int col = 0; col < folded.y_size(); col++) folded(fold.second - 1, col, 0) = tensor_map(fold.second - 1, col, 0);
            tensor_map = folded;
        } else if (fold.first == 1) {
            tensor::Tensor<int> folded(tensor_map.x_size(), fold.second, 1);
            for (int row = 0; row < folded.x_size(); row++) {
                for (int col = 0; col < folded.y_size(); col++) {
                    folded(row, col, 0) = tensor_map(row, col, 0) + tensor_map(row, tensor_map.y_size() - 1 - col, 0);
                }
            }
            // for (int row = 0; row < folded.x_size(); row++) folded(row, fold.second - 1, 0) = tensor_map(row, fold.second - 1, 0);
            tensor_map = folded;
        } else {
            std::cout << "elp" << std::endl;
        }
        // std::cout << tensor_map << std::endl;
        // int count = 0;
        // for (int i = 0; i < tensor_map.size(); i++) if (tensor_map(i) > 0) count++;
        // std::cout << count << std::endl;
        // break;
    }
    tensor::Tensor<char> str_tensor(tensor_map.x_size(), tensor_map.y_size(), 1);
    for (int i = 0; i < tensor_map.size(); i++) if (tensor_map(i) > 0) {str_tensor(i) = '.';} else {str_tensor(i) = ' ';};
    std::cout << str_tensor << std::endl;

    return 0;
}