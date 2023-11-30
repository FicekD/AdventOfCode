#include <iostream>
#include <fstream>
#include <sstream>

#include <cmath>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <algorithm>


std::string read_data(std::map<std::string, char>& data) {
    std::ifstream infile("data/14_data.txt");
    std::string line;

    std::getline(infile, line);
    std::string polymer_template = line;
    while(std::getline(infile, line)) {
        if (line.size() == 0) continue;
        auto delim_iter = line.find(" -> ");
        data[line.substr(0, delim_iter)] = line[line.size() - 1];
    }
    return polymer_template;
}


int main() {
    std::map<std::string, char> recipes;
    std::string polymer_template_str = read_data(recipes);

    std::map<std::string, long long> pairs;
    for (int i = 0; i < polymer_template_str.size() - 1; i++) {
        std::string key = polymer_template_str.substr(i, 2);
        pairs[key]++;
    }

    for (int iter = 0; iter < 40; iter++) {
        std::map<std::string, long long> new_pairs(pairs);
        for (auto& item : recipes) {
            auto map_it = pairs.find(item.first);
            if (map_it != pairs.end()) {
                std::string new_pair_left{(item.first)[0], item.second};
                std::string new_pair_right{item.second, (item.first)[1]};

                new_pairs[new_pair_left] += map_it->second;
                new_pairs[new_pair_right] += map_it->second;
                new_pairs[item.first] -= map_it->second;
            }
        }
        pairs = std::move(new_pairs);
    }

    std::map<char, long long> counters;
    for(auto& x : pairs) {
        counters[x.first[0]] += x.second;
        counters[x.first[1]] += x.second;
    }
    counters[polymer_template_str[0]]++;
    counters[polymer_template_str[polymer_template_str.size() - 1]]++;
    for (auto& c : counters) std::cout << c.first << ':' << c.second / 2 << std::endl;

    long long most_occ = 0, least_occ = LONG_LONG_MAX;
    for (auto& x : counters) {
        if (x.second > most_occ) most_occ = x.second;
        if (x.second < least_occ) least_occ = x.second;
    }
    std::cout << most_occ / 2 - least_occ / 2 << std::endl;

    return 0;
}