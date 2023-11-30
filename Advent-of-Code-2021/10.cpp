#include <iostream>
#include <fstream>
#include <sstream>

#include <cmath>
#include <string>
#include <vector>
#include <array>
#include <algorithm>


void read_data(std::vector<std::string>& data) {
    std::ifstream infile("data/10_data.txt");
    std::string line;

    while(std::getline(infile, line)) {
        data.push_back(line);
    }
}


int main() {
    std::vector<std::string> data;
    read_data(data);

    long long score = 0;
    bool found;
    std::vector<std::string> incomplete_data;
    for (auto& line : data) {
        found = false;
        std::vector<char> opening_brackets;
        for (auto& bracket : line) {
            switch(bracket) {
                case '{':
                case '(':
                case '<':
                case '[': {
                    opening_brackets.push_back(bracket);
                    break;
                }
                case '}': {
                    if (opening_brackets.back() == '{') opening_brackets.pop_back();
                    else {score += 1197; found = true;}
                    break;
                }
                case ')': {
                    if (opening_brackets.back() == '(') opening_brackets.pop_back();
                    else {score += 3; found = true;}
                    break;
                }
                case '>': {
                    if (opening_brackets.back() == '<') opening_brackets.pop_back();
                    else {score += 25137; found = true;}
                    break;
                }
                case ']': {
                    if (opening_brackets.back() == '[') opening_brackets.pop_back();
                    else {score += 57; found = true;}
                    break;
                }
            }
            if (found) break;
        }
        if (!found) incomplete_data.push_back(line);
    }
    std::cout << score << std::endl;

    std::vector<long long> scores;
    for (auto& line : incomplete_data) {
        score = 0;
        std::vector<char> opening_brackets;
        for (auto& bracket : line) {
            switch(bracket) {
                case '{':
                case '(':
                case '<':
                case '[': {
                    opening_brackets.push_back(bracket);
                    break;
                }
                case '}':
                case ')':
                case '>':
                case ']': {
                    opening_brackets.pop_back();
                    break;
                }
            }
        }
        for (auto i = opening_brackets.rbegin(); i != opening_brackets.rend(); i++) {
            int score_add;
            switch(*i) {
                case '(': {score_add = 1; break;}
                case '[': {score_add = 2; break;}
                case '{': {score_add = 3; break;}
                case '<': {score_add = 4; break;}
            }
            score = score * 5 + score_add;
            opening_brackets.pop_back();
        }
        scores.push_back(score);
    }
    std::sort(scores.begin(), scores.end());
    std::cout << scores[int(scores.size() / 2)] << std::endl;

    return 0;
}