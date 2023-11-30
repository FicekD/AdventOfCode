#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>

void read_data(std::vector<std::pair<std::string, int>>& data) {
    std::ifstream infile("data/02_data.txt");
    std::string line;

    while(std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string direction;
        int num;
        iss >> direction;
        iss >> num;
        data.push_back(std::pair<std::string, int>(direction, num));
    }
}


int main() {
    std::vector<std::pair<std::string, int>> data;
    read_data(data);

    int aim = 0;
    std::pair<int, int> coordinates(0, 0);
    for (auto& val : data) {
        // std::cout << val.first << ':' << val.second << std::endl;
        if (val.first == "forward") {
            coordinates.first += val.second;
            coordinates.second += aim * val.second;
        }
        else if (val.first == "down") aim += val.second;
        else if (val.first == "up") aim -= val.second;
    }
    std::cout << coordinates.first * coordinates.second << std::endl;

    return 0;
}