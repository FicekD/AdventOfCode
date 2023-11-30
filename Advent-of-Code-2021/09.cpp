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
    std::ifstream infile("data/09_data.txt");
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

    int danger = 0;
    for (int i = 0; i < data.size(); i++)
        for (int j = 0; j < data[0].size(); j++) {
            bool left = false, right = false, top = false, bottom = false;
            try {left = map(i, j-1, 0) > map(i, j, 0);} 
            catch(std::out_of_range) {left = true;}
            try {right = map(i, j+1, 0) > map(i, j, 0);} 
            catch(std::out_of_range) {right = true;}
            try {top = map(i-1, j, 0) > map(i, j, 0);} 
            catch(std::out_of_range) {top = true;}
            try {bottom = map(i+1, j, 0) > map(i, j, 0);} 
            catch(std::out_of_range) {bottom = true;}
            if (left && right && top && bottom) danger += map(i, j, 0) + 1;
        }
    std::cout << danger << std::endl;


    for (int i = 0; i < map.size(); i++) map(i) = map(i) == 9 ? -1 : 0;

    std::vector<int> areas = {0};
    int basin_id = 1;
    for (int i = 0; i < data.size(); i++)
        for (int j = 0; j < data[0].size(); j++) {
            if (map(i, j, 0) != 0) continue;
            std::vector<std::pair<int, int>> candidates, invalid_tiles;
            candidates.push_back(std::pair<int, int>(i, j));
            while (candidates.size()) {
                const std::pair<int, int> tile = candidates.back();
                int x = tile.first, y = tile.second;
                try {
                    if (map(x, y, 0) == -1) throw std::out_of_range("");
                } catch(std::out_of_range) {
                    invalid_tiles.push_back(std::pair<int, int>(x, y));
                    candidates.pop_back();
                    continue;
                }
                map(x, y, 0) = basin_id;
                areas.back()++;
                invalid_tiles.push_back(tile);
                candidates.pop_back();
                std::vector<std::pair<int, int>> potential_candidate_tiles = {std::pair<int, int>(x-1, y), std::pair<int, int>(x+1, y),
                                                                              std::pair<int, int>(x, y-1), std::pair<int, int>(x, y+1)};
                for (auto& cand_tile : potential_candidate_tiles) {
                    if (std::find(invalid_tiles.begin(), invalid_tiles.end(), cand_tile) != invalid_tiles.end() ||
                        std::find(candidates.begin(), candidates.end(), cand_tile) != candidates.end()) continue;
                    candidates.push_back(cand_tile);
                }
            }
            basin_id++;
            areas.push_back(0);
        }
        std::sort(areas.rbegin(), areas.rend());
        std::cout << areas[0] * areas[1] * areas[2] << std::endl;

    return 0;
}