#include <iostream>
#include <fstream>

#include <cmath>
#include <string>
#include <array>
#include <vector>

void read_data(std::vector<std::string>& data) {
    std::ifstream infile("data/03_data.txt");
    std::string line;

    while(std::getline(infile, line)) {
        data.push_back(line);
    }
}


long long binary2decimal(std::string binary) {
    long long decimal = 0;
    for (int i = 0; i < binary.size(); i++) {
        decimal += (long long)(int(binary[binary.size() - i - 1] == '1') * powl(2, i));
    }
    return decimal;
}


void find_num_occurances(std::vector<std::string>& data, std::vector<int>& num_occurances) {
    std::fill(num_occurances.begin(), num_occurances.end(), -int(data.size() / 2));
    for (auto& val : data) {
        for (int i = 0; i < data[0].size(); i++) {
            num_occurances[i] += int(val[i] == '1');
        }
    }
}


float bit_ratio_at(std::vector<std::string>& data, int index) {
    int num_ones = 0;
    for (auto& val : data) {
        num_ones += int(val[index] == '1');
    }
    return float(num_ones) / data.size();
}


int main() {
    std::vector<std::string> data;
    read_data(data);

    int num_bits = data[0].size();

    std::vector<int> num_occurances(num_bits);
    find_num_occurances(data, num_occurances);
    
    int gamma = 0, epsilon = 0;
    for (int i = 0; i < num_bits; i++) {
        gamma += int(num_occurances[num_bits - i - 1] >= 0) * powl(2, i);
        epsilon += int(num_occurances[num_bits - i - 1] < 0) * powl(2, i);
    }
    std::cout << gamma * epsilon << std::endl;

    std::vector<std::string> oxygen_data(data);
    for (int i = 0; i < data[0].size(); i++) {
        if (oxygen_data.size() == 0) break;
        bool most_freq_bit = bit_ratio_at(oxygen_data, i) >= 0.5;
        std::vector<std::string> filtered_data;
        for (auto& val : oxygen_data) if (most_freq_bit == (val[i] == '1')) filtered_data.push_back(val);
        oxygen_data = filtered_data;
    }
    std::vector<std::string> co2_data(data);
    for (int i = 0; i < data[0].size(); i++) {
        if (co2_data.size() == 0) break;
        bool least_freq_bit = bit_ratio_at(co2_data, i) < 0.5;
        std::vector<std::string> filtered_data;
        for (auto& val : co2_data) if (least_freq_bit == (val[i] == '1')) filtered_data.push_back(val);
        co2_data = filtered_data;
    }

    std::cout << binary2decimal(oxygen_data[0]) * binary2decimal(co2_data[0]) << std::endl;

    return 0;
}