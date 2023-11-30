#include <iostream>
#include <fstream>
#include <sstream>

#include <cmath>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <map>


void parse_line(std::vector<std::vector<char>>& data_vector, const std::string& data_string) {
    std::istringstream iss_in(data_string);
    std::string input_code;
    while(std::getline(iss_in, input_code, ' ')) {
        std::vector<char> data(input_code.begin(), input_code.end());
        std::sort(data.begin(), data.end());
        data_vector.push_back(data);
    }
}


void read_data(std::vector<std::vector<std::vector<char>>>& data_in, std::vector<std::vector<std::vector<char>>>& data_out) {
    std::ifstream infile("data/08_data.txt");
    std::string line;

    while(std::getline(infile, line)) {
        std::vector<std::vector<char>> input_data;
        parse_line(input_data, line.substr(0, line.find('|')));
        data_in.push_back(input_data);

        std::vector<std::vector<char>> output_data;
        parse_line(output_data, line.substr(line.find('|') + 2, line.size() - 1));
        data_out.push_back(output_data);
    }
}


int digits2integer(std::vector<int>& digits) {
    int integer = 0;
    for (int i = 0; i < digits.size(); i++) {
        integer += digits[i] * pow(10, digits.size() - i - 1);
    }
    return integer;
}


int main() {
    std::vector<std::vector<std::vector<char>>> data_in, data_out;
    read_data(data_in, data_out);

    int num_occ = 0;
    for(auto& line : data_out)
        for(auto& sig : line)
            if (sig.size() == 2 || sig.size() == 3 || sig.size() == 4 || sig.size() == 7) num_occ++;
    std::cout << num_occ << std::endl;

    long long result = 0;
    for(int i = 0; i < data_in.size(); i++) {
        auto line_in = data_in[i], line_out = data_out[i];
        std::sort(line_in.begin(), line_in.end(), [](const std::vector<char>& first, const std::vector<char>& second) {
            return first.size() < second.size();
        });
        std::map<int, std::vector<char>> decoded;
        for(auto& x : line_in) {
            switch(x.size()) {
                case 2: {decoded[1] = x; break;}
                case 3: {decoded[7] = x; break;}
                case 4: {decoded[4] = x; break;}
                case 7: {decoded[8] = x; break;}
            }
        }
        std::vector<char> four_leftover;
        std::set_difference(decoded[4].begin(), decoded[4].end(), decoded[1].begin(), decoded[1].end(), std::inserter(four_leftover, four_leftover.begin()));
        for(auto& x : line_in) {
            if (x.size() != 5) continue;
            std::vector<char> intersection;
            std::set_intersection(x.begin(), x.end(), decoded[1].begin(), decoded[1].end(), std::inserter(intersection, intersection.begin()));
            if (intersection.size() == 2) {decoded[3] = x; continue;}
            intersection.clear();
            std::set_intersection(x.begin(), x.end(), four_leftover.begin(), four_leftover.end(), std::inserter(intersection, intersection.begin()));
            if (intersection.size() == 2) {decoded[5] = x; continue;}
            decoded[2] = x;
        }
        for(auto& x : line_in) {
            if (x.size() != 6) continue;
            std::vector<char> intersection;
            std::set_intersection(x.begin(), x.end(), decoded[4].begin(), decoded[4].end(), std::inserter(intersection, intersection.begin()));
            if (intersection.size() == 4) {decoded[9] = x; continue;}
            intersection.clear();
            std::set_intersection(x.begin(), x.end(), four_leftover.begin(), four_leftover.end(), std::inserter(intersection, intersection.begin()));
            if (intersection.size() == 2) {decoded[6] = x; continue;}
            decoded[0] = x;
        }

        std::vector<int> values;
        for(auto& x : line_out) for(auto& y : decoded)
            if (x == y.second) {values.push_back(y.first); break;}
        result += digits2integer(values);
    }
    std::cout << result << std::endl;

    return 0;
}