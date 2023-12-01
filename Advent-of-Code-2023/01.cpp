#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <numeric>


void read_data(std::vector<std::string>& data) {
    std::ifstream infile("data/01.txt");
    std::string line;

    while(std::getline(infile, line)) {
        data.push_back(line);
    }
}

void parse_row_p1(std::string& row, std::vector<int>& data) {
    for (char c : row) {
        if (isdigit(c))
            data.push_back(c - '0');
    }
}

void parse_row_p2(std::string& row, std::vector<int>& data) {
    static const std::string digits[10] = {
        "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
    };
    for (int i = 0; i < row.size(); i++) {
        if (isdigit(row[i])) {
            data.push_back(row[i] - '0');
            continue;
        }
        for (int j = 0; j < 10; j++) {
            std::string digit = digits[j];
            if (i + digit.size() > row.size())
                continue;
            std::string substring = row.substr(i, digit.size());
            if (substring == digit) {
                data.push_back(j);
                break;
            }
        }
    }
}

int process(std::vector<std::string>& data, bool part1 = true) {
    std::vector<int> values;
    for (std::string& row : data) {
        std::vector<int> values_row;
        if (part1)
            parse_row_p1(row, values_row);
        else
            parse_row_p2(row, values_row);
            
        int first = -1, last = -1;
        for (int val : values_row) {
            if (first == -1)
                first = val;
            else
                last = val;
        }
        if (last == -1) 
            last = first;
        values.push_back(10 * first + last);
    }

    return std::accumulate(values.begin(), values.end(), 0);
}


int main() {
    std::vector<std::string> data;
    read_data(data);

    std::cout << process(data, true) << std::endl;
    std::cout << process(data, false) << std::endl;

    return 0;
}