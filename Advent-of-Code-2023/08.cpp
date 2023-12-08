#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

#include <map>

void read_data(std::vector<char>& directions, std::map<std::string, std::pair<std::string, std::string>>& map) {
    std::ifstream infile("./data/08.txt");
    std::string line;

    std::getline(infile, line);
    std::copy(line.begin(), line.end(), std::back_inserter(directions));

    std::getline(infile, line);
    while(std::getline(infile, line)) {
        std::string key = line.substr(0, 3);
        std::string val_left = line.substr(7, 3);
        std::string val_right = line.substr(12, 3);
        map[key] = std::pair<std::string, std::string>(val_left, val_right);
    }
}

long part1(const std::vector<char>& directions, const std::map<std::string, std::pair<std::string, std::string>>& map) {
    long steps = 0;
    std::string key("AAA"), target("ZZZ");
    for (;; steps++) {
        if (key == target)
            break;
        key = directions[steps % directions.size()] == 'R' ? map.at(key).second : map.at(key).first;
    }
    return steps;
}

template <typename T = unsigned long long>
T part2(const std::vector<char>& directions, const std::map<std::string, std::pair<std::string, std::string>>& map) {
    std::vector<T> steps_vector;
    for (auto& item : map) {
        if (item.first.back() != 'A')
            continue;
        std::string key = item.first;
        
        T steps = 0;
        for (;; steps++) {
            if (key.back() == 'Z')
                break;
            key = directions[steps % directions.size()] == 'R' ? map.at(key).second : map.at(key).first;
        }
        steps_vector.push_back(steps);
    }

    return std::accumulate(steps_vector.begin(), steps_vector.end(), steps_vector[0], std::lcm<T, T>);
}

int main() {
    std::vector<char> directions;
    std::map<std::string, std::pair<std::string, std::string>> map;
    read_data(directions, map);

    std::cout << part1(directions, map) << std::endl;
    std::cout << part2(directions, map) << std::endl;

    return 0;
}