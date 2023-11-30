#include <iostream>
#include <fstream>
#include <sstream>

#include <cmath>
#include <string>
#include <array>
#include <vector>

#include "include/tensor.hpp"

#define SIDE 5
#define BOARDS 100

typedef unsigned uint;


void read_data(std::vector<uint>& draws, tensor::Tensor<int>& boards) {
    std::ifstream infile("data/04_data.txt");
    std::string line;

    std::getline(infile, line);
    uint val; char comma;
    std::stringstream stream(line);
    while(!stream.eof()) {
        stream >> val;
        stream >> comma;
        draws.push_back(val);
    }
    uint x = 0, y = 0, z = -1;
    while(std::getline(infile, line)) {
        if (line.empty()) {
            x = 0;
            z++;
            continue;
        }
        std::stringstream stream(line);
        for (y = 0; y < SIDE; y++) {
            stream >> val;
            boards(x, y, z) = val;
        }
        x++;
    }
}


int main() {
    std::vector<uint> draws;
    tensor::Tensor<int> boards(SIDE, SIDE, BOARDS);
    read_data(draws, boards);

    std::array<bool, BOARDS> finished;
    finished.fill(false);
    uint last_z = -1, last_draw;

    for (auto& draw : draws) {
        for (int i = 0; i < boards.size(); i++) {
            if (boards(i) == draw) boards(i) = -1;
        }
        for (uint z = 0; z < BOARDS; z++) {
            if (finished[z]) continue;
            for (uint y = 0; y < SIDE; y++) {
                for (uint x = 0; x < SIDE; x++) {
                    if (boards(x, y, z) != -1) break;
                    else if (x == SIDE-1) {
                        if (last_z == -1) std::cout << boards.sum_single_z_dim(z) * draw << std::endl;
                        finished[z] = true;
                        last_z = z;
                        break;
                    }
                }
                if (finished[z]) break;
            }
            if (finished[z]) continue;
            for (uint x = 0; x < SIDE; x++) {
                for (uint y = 0; y < SIDE; y++) {
                    if (boards(x, y, z) != -1) break;
                    else if (y == SIDE-1) {
                        if (last_z == -1) std::cout << boards.sum_single_z_dim(z) * draw << std::endl;
                        finished[z] = true;
                        last_z = z;
                        break;
                    }
                }
                if (finished[z]) break;
            }
            if (finished[z]) break;
        }
        bool finished_all = true;
        for (auto& val : finished) finished_all *= val;
        if (finished_all) {
            last_draw = draw;
            break;
        }
    }
    
    std::cout << boards.sum_single_z_dim(last_z) * last_draw << std::endl;

    return 0;
}