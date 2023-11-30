#include <iostream>
#include <fstream>
#include <sstream>

#include <cmath>
#include <string>
#include <vector>
#include <array>
#include <algorithm>

#include "include/tensor.hpp"


void read_data(std::vector<std::vector<int>>& data) {
    std::ifstream infile("data/11_data.txt");
    std::string line;

    while(std::getline(infile, line)) {
        std::vector<int> row;
        for(auto& x : line) row.push_back(x - '0');
        data.push_back(row);
    }
}


int main() {
    std::vector<std::vector<int>> data;
    read_data(data);

    tensor::Tensor<int> map(data.size(), data[0].size(), 1);
    for (int i = 0; i < data.size(); i++)
        for (int j = 0; j < data[0].size(); j++)
            map(i, j, 0) = data[i][j];

    long long evolvs = 0;
    for (int steps = 0;; steps++) {
        for (int i = 0; i < map.size(); i++) map(i)++;
        bool evolving;
        do {
            evolving = false;
            for (int i = 0; i < data.size(); i++)
                for (int j = 0; j < data[0].size(); j++) {
                    if (map(i, j, 0) < 10) continue;
                    evolving = true;
                    for (int k = -1; k < 2; k++)
                        for (int m = -1; m < 2; m++) {
                            try {map(i+k, j+m, 0)++;}
                            catch(std::out_of_range) {}
                        }
                    map(i, j, 0) = -10;
                    if (steps < 100) evolvs++;
                }
        } while(evolving);
        int num_zeros = 0;
        for (int i = 0; i < map.size(); i++)
            if (map(i) < 0) {map(i) = 0; num_zeros++;};
        if (num_zeros == map.size()) {
            std::cout << "Steps to flash: " << steps + 1 << std::endl;
            break;
        }
    }
    std::cout << "Evolves in 100 steps: " << evolvs << std::endl;

    return 0;
}