#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <sstream>
#include <set>
#include <algorithm>
#include <queue>

#include <math.h>

#include "include/parsing.h"

class Sets {
private:
    std::set<int> _winning;
    std::set<int> _crossed;
public:
    Sets(const std::set<int>& winning, const std::set<int>& crossed) {
        _winning = winning;
        _crossed = crossed;
    }

    int n_intersections() const {
        std::set<int> intersection;
        set_intersection(
            _winning.begin(), _winning.end(),
            _crossed.begin(), _crossed.end(),
            std::inserter(intersection, intersection.begin())
        );
        return intersection.size();
    }
};

void parse_ints(const std::string& s, std::set<int>& ints) {
    std::istringstream iss(s);
    int num;
    while (iss >> num) ints.insert(num);
}

void read_data(std::vector<Sets>& data) {
    std::ifstream infile("./data/04.txt");
    std::string line;

    while(std::getline(infile, line)) {
        line = line.substr(line.find(':') + 2);

        std::vector<std::string> split;
        parsing::split(line, " | ", split);

        std::set<int> winning;
        parse_ints(split[0], winning);
        std::set<int> crossed;
        parse_ints(split[1], crossed);
        data.push_back({winning, crossed});
    }
}

int part1(const std::vector<Sets>& data) {
    long score = 0;
    for (const Sets& card : data) {
        int intersection_size = card.n_intersections();
        if (intersection_size > 0)
            score += powl(2, intersection_size - 1);
    }
    return score;
}

int part2(const std::vector<Sets>& data) {
    long score = 0;

    int n_cards[data.size()];
    std::fill_n(n_cards, data.size(), 1);

    for (std::size_t i = 0; i < data.size(); i++) {
        const Sets& card = data[i];
        std::size_t intersection_size = card.n_intersections();

        for (std::size_t j = 1; j <= intersection_size; j++) {
            std::size_t next_index = i + j;
            if (next_index >= data.size())
                break;
            n_cards[next_index] += n_cards[i];
        }
        score += n_cards[i];
    }

    return score;
}

int main() {
    std::vector<Sets> data;
    read_data(data);

    std::cout << part1(data) << std::endl;
    std::cout << part2(data) << std::endl;

    return 0;
}