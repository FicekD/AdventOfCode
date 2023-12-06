#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

#include <sstream>
#include <math.h>


void read_data(std::vector<std::pair<int, int>>& data) {
    std::ifstream infile("./data/06.txt");
    std::string time_line, distance_line;

    std::getline(infile, time_line);
    std::getline(infile, distance_line);

    time_line = time_line.substr(5);
    distance_line = distance_line.substr(9);

    std::istringstream iss_time(time_line);
    std::istringstream iss_distance(distance_line);
    int time, distance;
    while (iss_time >> time && iss_distance >> distance) {
        data.push_back(std::pair<int, int>(time, distance));
    }
}

int part1(const std::vector<std::pair<int, int>>& data) {
    std::vector<int> possible_wins;
    for (const std::pair<int, int>& pair : data) {
        int ways_to_win = 0;
        for (int velocity = 1; velocity < pair.first; velocity++) {
            if (velocity * (pair.first - velocity) > pair.second) ways_to_win++;
        }
        possible_wins.push_back(ways_to_win);
    }
    return std::accumulate(possible_wins.begin(), possible_wins.end(), 1, std::multiplies<int>());
}

int part2(const std::vector<std::pair<int, int>>& data) {
    std::stringstream time_ss, dist_ss;
    for (const std::pair<int, int>& pair : data) {
        time_ss << pair.first;
        dist_ss << pair.second;
    }
    long double time = (long double)std::stoll(time_ss.str()), distance = (long double)std::stoll(dist_ss.str());
    long double D_sqrt = sqrt(time * time - 4 * distance);
    long double root_1 = (time - D_sqrt) / 2, root_2 = (time + D_sqrt) / 2;
    return floorl(root_2) - ceill(root_1) + 1;
}

int main() {
    std::vector<std::pair<int, int>> data;
    read_data(data);

    std::cout << part1(data) << std::endl;
    std::cout << part2(data) << std::endl;

    return 0;
}