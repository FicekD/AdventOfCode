#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

#include <sstream>
#include <bitset>

#include "include/parsing.h"


void read_data(std::vector<std::pair<std::vector<char>, std::vector<int>>>& data) {
    std::ifstream infile("./data/12.txt");
    std::string line;

    while(std::getline(infile, line)) {
        std::size_t space = line.find(' ');

        std::string conditions_s = line.substr(0, space);
        std::vector<char> conditions(conditions_s.begin(), conditions_s.end());

        std::vector<std::string> numbers_s;
        parsing::split(line.substr(space + 1), ",", numbers_s);
        std::vector<int> numbers;
        for (const std::string& s : numbers_s) {
            numbers.push_back(std::stoi(s));
        }
        data.push_back(std::pair<std::vector<char>, std::vector<int>>(conditions, numbers));
    }
}

bool setting_valid(const std::vector<char>& conditions, const std::vector<int>& numbers) {
    std::size_t cluster = 0;
    std::size_t current_cluster_size = 0;
    bool started = false;
    for (char c : conditions) {
        if (c == '#') {
            started = true;
            current_cluster_size++;
        }
        else if (started) {
            if (current_cluster_size != numbers[cluster])
                return false;
            started = false;
            cluster++;
            if (cluster > numbers.size())
                return false;
            current_cluster_size = 0;
        }
    }
    if (started && current_cluster_size != numbers[cluster])
        return false;
    if (cluster != numbers.size() - started)
        return false;
    return true;
}

void argwhere(const std::vector<char>& vec, char val, std::vector<std::size_t>& indices) {
    for (std::size_t i = 0; i < vec.size(); i++) {
        if (vec[i] == val)
            indices.push_back(i);
    }
}

std::size_t part1(const std::vector<std::pair<std::vector<char>, std::vector<int>>>& data) {
    std::size_t n_arrangements = 0;
    for (auto& pair : data) {
        std::vector<char> combination(pair.first.begin(), pair.first.end());
        std::vector<std::size_t> unknown_idx;
        argwhere(combination, '?', unknown_idx);

        int max_crosses = std::accumulate(pair.second.begin(), pair.second.end(), 0);
        for (char c : combination) if (c == '#') max_crosses--;

        std::size_t combinations = powl(2, unknown_idx.size());
        for (std::size_t comb = 0; comb < combinations; comb++) {
            std::string binary = std::bitset<64>(comb).to_string();
            int crosses = 0;
            for (std::size_t i = 0; i < unknown_idx.size(); i++) {
                if (binary[binary.size() - 1 - i] == '1') {
                    combination[unknown_idx[i]] = '.';
                }
                else {
                    combination[unknown_idx[i]] = '#';
                    if (++crosses > max_crosses)
                        break;
                }
            }
            if (crosses > max_crosses)
                continue;
            if (setting_valid(combination, pair.second))
                n_arrangements++;
        }
    }
    return n_arrangements;
}

std::size_t part2(const std::vector<std::pair<std::vector<char>, std::vector<int>>>& data) {
    std::size_t n_arrangements = 0;
    return n_arrangements;
}

int main() {
    std::vector<std::pair<std::vector<char>, std::vector<int>>> data;
    read_data(data);

    std::cout << part1(data) << std::endl;
    std::cout << part2(data) << std::endl;

    return 0;
}