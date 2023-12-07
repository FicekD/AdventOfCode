#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

#include <map>


void parse_hand(const std::string& string, std::vector<int>& hand) {
    for (char c : string) {
        int val;
        if (std::isdigit(c)) val = c - '0';
        else if (c == 'T') val = 10;
        else if (c == 'J') val = 11;
        else if (c == 'Q') val = 12;
        else if (c == 'K') val = 13;
        else if (c == 'A') val = 14;
        hand.push_back(val);
    }
}

void read_data(std::vector<std::pair<std::vector<int>, int>>& hands) {
    std::ifstream infile("./data/07.txt");
    std::string line;

    while(std::getline(infile, line)) {
        std::size_t space_idx = line.find(' ');
        std::vector<int> hand;
        parse_hand(line.substr(0, space_idx), hand);
        int bid = std::stoi(line.substr(space_idx + 1));
        hands.push_back(std::pair<std::vector<int>, int>(hand, bid));
    }
}

int evaluate_hand(const std::vector<int>& hand, bool jokers = false) {
    std::map<int, int> occ_map;
    for (int card : hand) occ_map[card] += 1;

    if (jokers) {
        int n_jokers = occ_map[-1];
        occ_map.erase(-1);
        std::pair<int, int> max_item(0, 0);
        for (auto& item : occ_map)
            if (item.second > max_item.second) max_item = item;
        occ_map[max_item.first] += n_jokers;
    }

    int score = 0;
    switch (occ_map.size()) {
        case 1: {
            score = 20;
            break;
        }
        case 2: {
            const std::pair<int, int>& first_item = *occ_map.begin();
            if (first_item.second == 1 || first_item.second == 4) score = 19;
            else score = 18;
            break;
        }
        case 3: {
            score = 16;
            for (auto& item : occ_map) {
                if (item.second == 3) {
                    score = 17;
                    break;
                }
            }
            break;
        }
        case 4: {
            score = 15;
            break;
        }
        case 5: {
            score = hand[0];
            break;
        }
    }
    return score;
}

struct hand_comparator {
    bool jokers = false;
    inline bool operator()(const std::pair<std::vector<int>, int>& hand_1,
                           const std::pair<std::vector<int>, int>& hand_2) {
        int hand_1_score = evaluate_hand(hand_1.first, jokers);
        int hand_2_score = evaluate_hand(hand_2.first, jokers);
        if (hand_1_score == hand_2_score) {
            for (std::size_t i = 0; i < hand_1.first.size(); i++) {
                if (hand_1.first[i] > hand_2.first[i]) {
                    hand_1_score++;
                    break;
                }
                else if (hand_1.first[i] < hand_2.first[i]) {
                    hand_2_score++;
                    break;
                }
            }
        }
        return hand_1_score < hand_2_score;
    }
};

int score_sorted(const std::vector<std::pair<std::vector<int>, int>>& hands) {
    int score = 0;
    for (int i = 0; i < (int)hands.size(); i++) {
        score += (i + 1) * hands[i].second;
    }
    return score;
}

int part1(std::vector<std::pair<std::vector<int>, int>> hands) {
    std::sort(hands.begin(), hands.end(), hand_comparator());
    return score_sorted(hands);
}

int part2(std::vector<std::pair<std::vector<int>, int>> hands) {
    for (std::pair<std::vector<int>, int>& hand : hands) {
        for (int& card : hand.first) {
            if (card == 11) card = -1;
        }
    }
    std::sort(hands.begin(), hands.end(), hand_comparator { true });
    return score_sorted(hands);
}

int main() {
    std::vector<std::pair<std::vector<int>, int>> hands;
    read_data(hands);

    std::cout << part1(hands) << std::endl;
    std::cout << part2(hands) << std::endl;

    return 0;
}