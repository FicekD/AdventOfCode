#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

#include <sstream>


void read_data(std::vector<std::vector<int>>& data) {
    std::ifstream infile("./data/09.txt");
    std::string line;

    while(std::getline(infile, line)) {
        std::istringstream iss(line);

        std::vector<int> series;
        int num;
        while (iss >> num) {
            series.push_back(num);
        }

        data.push_back(series);
    }
}

int extrapolate(const std::vector<std::vector<int>>& data, bool extrapolate_last) {
    std::vector<int> values;
    for (const std::vector<int>& original_series : data) {
        std::vector<std::vector<int>> series;
        series.push_back(original_series);

        while(!all_of(series.back().begin(), series.back().end(), [&series](int i){ return i == series.back()[0]; })) {
            std::vector<int> diff_series;
            for (std::size_t i = 0; i < series.back().size() - 1; i++) {
                diff_series.push_back(series.back()[i + 1] - series.back()[i]);
            }
            series.push_back(diff_series);
        }

        int last_number = series.back()[0];
        for (auto i = series.rbegin() + 1; i != series.rend(); i++) {
            last_number = extrapolate_last ? i->back() + last_number : i->front() - last_number;
        } 
        values.push_back(last_number);
    }
    return std::accumulate(values.begin(), values.end(), 0);
}

int part1(const std::vector<std::vector<int>>& data) {
    return extrapolate(data, true);
}

int part2(const std::vector<std::vector<int>>& data) {
    return extrapolate(data, false);
}

int main() {
    std::vector<std::vector<int>> data;
    read_data(data);

    std::cout << part1(data) << std::endl;
    std::cout << part2(data) << std::endl;

    return 0;
}