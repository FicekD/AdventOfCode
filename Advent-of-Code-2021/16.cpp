#include <iostream>
#include <fstream>
#include <sstream>

#include <cmath>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>


void read_data(std::vector<int>& data) {
    std::ifstream infile("data/16_data.txt");

    std::string line;
    std::getline(infile, line);
    std::istringstream iss(line);
    uint8_t hex_char;
    while (iss >> std::hex >> hex_char) {
        int hex_int = (hex_char >= 'A') ? (hex_char - 'A' + 10) : (hex_char - '0');
        data.push_back((hex_int & 0x08) >> 3);
        data.push_back((hex_int & 0x04) >> 2);
        data.push_back((hex_int & 0x02) >> 1);
        data.push_back((hex_int & 0x01) >> 0);
    }
}

unsigned long long decode(std::vector<int>::iterator& data_iter, size_t len) {
    unsigned long long result = 0;
    for (int i = len - 1; i >= 0; data_iter++, i--) {
        result += pow(2, i) * *(data_iter);
    }
    return result;
}

void decode_literals(std::vector<int>::iterator& data_iter, std::vector<unsigned long long>& decoded) {
    bool decode_next = true;
    while (decode_next) {
        decode_next = *(data_iter);
        data_iter++;
        decoded.push_back(decode(data_iter, 4));
    }
}

unsigned long long convert_literals(const std::vector<unsigned long long>& literals) {
    unsigned long long result = 0;
    for (int i = 0; i < (int)literals.size(); i++) result += literals[literals.size() - 1 - i] * pow(16, i);
    return result;
}

unsigned long long decode_packet(std::vector<int>::iterator& iter, int& version_sum, bool skip_to_hexa = true) {
    unsigned version = decode(iter, 3);
    version_sum += version;
    unsigned type = decode(iter, 3);
    if (type == 4) {
        std::vector<unsigned long long> decoded_literals;
        decode_literals(iter, decoded_literals);
        unsigned long long literal = (unsigned long long)convert_literals(decoded_literals);
        if (skip_to_hexa) iter += 4 - (6 + 5 * (int)decoded_literals.size()) % 4;
        return literal;
    } else {
        std::vector<unsigned long long> literals;
        int id_bit = *(iter++);
        if (id_bit) {
            unsigned n_packets = decode(iter, 11);
            for (int i = 0; i < (int)n_packets; i++) {
                unsigned long long subpacket_result = decode_packet(iter, version_sum, skip_to_hexa=false);
                literals.push_back(subpacket_result);
            }
        } else {
            unsigned packet_size = decode(iter, 15);
            auto target_iter = iter + packet_size;
            while (iter < target_iter) {
                unsigned long long subpacket_result = decode_packet(iter, version_sum, skip_to_hexa=false);
                literals.push_back(subpacket_result);
            }
        }
        switch (type) {
            case 0:
                return std::accumulate(literals.begin(), literals.end(), (unsigned long long)0);
            case 1:
                return std::accumulate(literals.begin(), literals.end(), (unsigned long long)1, std::multiplies<unsigned long long>());
            case 2:
                return *std::min_element(literals.begin(), literals.end());
            case 3:
                return *std::max_element(literals.begin(), literals.end());
            case 5:
                return (unsigned long long)(literals[0] > literals[1]);
            case 6:
                return (unsigned long long)(literals[0] < literals[1]);
            case 7:
                return (unsigned long long)(literals[0] == literals[1]);
        }
    }
    return 0;
}

int main() {
    std::vector<int> data;
    read_data(data);

    int version_sum = 0;
    auto iter = data.begin();
    unsigned long long result = decode_packet(iter, version_sum);
    std::cout << version_sum << std::endl;
    std::cout << result << std::endl;

    return 0;
}