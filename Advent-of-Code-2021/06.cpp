#include <iostream>
#include <fstream>
#include <sstream>

#include <cmath>
#include <string>
#include <vector>
#include <array>
#include <algorithm>


#define INIT_CYCLE_LEN 8
#define CYCLE_LEN 6


void read_data(std::vector<int>& data) {
    std::ifstream infile("data/06_data.txt");
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


class Lanternfish {
    int cycle = INIT_CYCLE_LEN;
public:
    Lanternfish() {}
    Lanternfish(int init_cycle) : cycle(init_cycle) {}

    bool update() {
        cycle--;
        if (cycle == -1) {
            cycle = CYCLE_LEN;
            return true;
        }
        return false;
    }
};


int main() {
    std::vector<int> data;
    read_data(data);

    std::vector<Lanternfish> swarm;
    for (auto& x : data) swarm.push_back(Lanternfish(x));

    for (int day = 0; day < 80; day++) {
        int newborns = 0;
        for (auto& fish : swarm) if (fish.update()) newborns++;
        for (int i = 0; i < newborns; i++) swarm.push_back(Lanternfish());
    }
    std::cout << swarm.size() << std::endl;

    std::vector<long long> population(INIT_CYCLE_LEN + 1);
    std::fill(population.begin(), population.end(), 0);
    for (auto& x : data) population[x]++;

    for (int day = 0; day < 256; day++) {
        std::rotate(population.begin(), population.begin()+1, population.end() - (INIT_CYCLE_LEN - CYCLE_LEN));
        long long newborns = population[CYCLE_LEN];
        population[CYCLE_LEN] += population[CYCLE_LEN + 1];
        for (int i = CYCLE_LEN + 1; i < INIT_CYCLE_LEN; i++) population[i] = population[i + 1];
        population[INIT_CYCLE_LEN] = newborns;
    }

    long long sum = 0;
    for (auto& x : population) sum += x;
    std::cout << sum << std::endl;

    return 0;
}