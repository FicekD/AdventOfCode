#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "include/parsing.h"

class Round {
private:
    int _red, _green, _blue;
public:
    Round(int red, int green, int blue) {
        _red = red;
        _green = green;
        _blue = blue;
    }
    int& red() {
        return _red;
    }
    int& green() {
        return _green;
    }
    int& blue() {
        return _blue;
    }
};

void read_data(std::vector<std::vector<Round>>& data) {
    std::ifstream infile("./data/02.txt");
    std::string line;

    while(std::getline(infile, line)) {
        std::vector<Round> rounds;

        line = line.substr(line.find(':') + 2);
        std::vector<std::string> round_split;
        parsing::split(line, "; ", round_split);

        for (std::string& pull_string : round_split) {
            int red = 0, green = 0, blue = 0;

            std::vector<std::string> pull_split;
            parsing::split(pull_string, ", ", pull_split);
            for (std::string& ball_string : pull_split) {
                std::vector<std::string> ball_split;
                parsing::split(ball_string, " ", ball_split);

                int n = std::stoi(ball_split[0]);
                if (ball_split[1] == "red") red = n;
                else if (ball_split[1] == "green") green = n;
                else if (ball_split[1] == "blue") blue = n;
            }
            rounds.push_back(Round(red, green, blue));
        }
        data.push_back(rounds);
    }
}

int part1(std::vector<std::vector<Round>>& data) {
    int result = 0;
    Round max_round(12, 13, 14);
    for (std::size_t i = 0; i < data.size(); i++) {
        std::vector<Round>& rounds = data[i];

        bool possible = true;
        for (Round& round : rounds) {
            if (round.red() > max_round.red() ||
                round.green() > max_round.green() || 
                round.blue() > max_round.blue()
            ) {
                possible = false;
                break;
            } 
        }
        if (possible)
            result += i + 1;
    }
    return result;
}

int part2(std::vector<std::vector<Round>>& data) {
    int result = 0;
    for (std::size_t i = 0; i < data.size(); i++) {
        std::vector<Round>& rounds = data[i];
        Round max_balls(0, 0, 0);
        for (Round& round : rounds) {
            if (round.red() > max_balls.red()) max_balls.red() = round.red();
            if (round.green() > max_balls.green()) max_balls.green() = round.green();
            if (round.blue() > max_balls.blue()) max_balls.blue() = round.blue();
        }
        result += max_balls.red() * max_balls.green() * max_balls.blue();
    }
    return result;
}

int main() {
    std::vector<std::vector<Round>> data;
    read_data(data);

    std::cout << part1(data) << std::endl;
    std::cout << part2(data) << std::endl;

}