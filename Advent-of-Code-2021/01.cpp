#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>


void read_data(std::vector<int>& data) {
    std::ifstream infile("data/01_data.txt");
    std::string line;

    while(std::getline(infile, line)) {
        std::istringstream iss(line);
        int val;
        iss >> val;
        data.push_back(val);
    }
}


int main() {
    std::vector<int> data;

    read_data(data);

    int increases = 0;
    int prev_sum = 0;
    for(int i = 1; i < data.size()-1; i++) {
        int sum = data[i-1] + data[i] + data[i+1];
        if (sum > prev_sum) increases++;
        prev_sum = sum;
    }
    std::cout << increases - 1 << std::endl;

    return 0;
}