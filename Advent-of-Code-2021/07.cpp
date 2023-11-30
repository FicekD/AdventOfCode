#include <iostream>
#include <fstream>
#include <sstream>

#include <cmath>
#include <string>
#include <vector>
#include <array>
#include <algorithm>


void read_data(std::vector<int>& data) {
    std::ifstream infile("data/07_data.txt");
    std::string line;

    while(std::getline(infile, line)) {
        std::istringstream iss(line);
        int number;
        char delim;
        do {
            iss >> number;
            data.push_back(number);
        } while(iss >> delim);
    }
}


int main() {
    std::vector<int> data;
    read_data(data);

    std::sort(data.begin(), data.end());
    int median = data[int(data.size() / 2)];
    int fuel = 0;
    for (auto& x : data) fuel += abs(x - median);
    std::cout << fuel << std::endl;

    std::vector<int> cost;
    for (int i = data[0]; i <= data[data.size() - 1]; i++) {
        int c = 0;
        for (auto& x : data) {
            int n = abs(i - x);
            c += int(n * (n + 1) / 2);
        }
        cost.push_back(c);
    }
    std::cout << *std::min_element(cost.begin(), cost.end()) << std::endl;

    return 0;
}