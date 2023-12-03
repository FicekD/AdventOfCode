#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <numeric>
#include <math.h>

enum PointType {
    Value, Symbol, Empty
};

enum SymbolType {
    Gear, Other
};

struct Point {
    int value;
    PointType type;
    SymbolType symbol_type = Other;
    bool positive_part = false;
    bool part_accounted_for = false;
    bool gear_accounter_for = false;
};

struct CartCoord {
    int row;
    int col;
};

const CartCoord dirs[] = {
    { -1, -1 },
    { -1,  0 },
    { -1,  1 },
    {  0, -1 },
    {  0,  1 },
    {  1, -1 },
    {  1,  0 },
    {  1,  1 },
};

void read_data(std::vector<std::vector<Point>>& data) {
    std::ifstream infile("./data/03.txt");
    std::string line;

    while(std::getline(infile, line)) {
        std::vector<Point> points;
        for (char c : line) {
            if (isdigit(c)) points.push_back({ c - '0', Value });
            else if (c == '.') points.push_back({-1, Empty});
            else if (c == '*') points.push_back({-1, Symbol, Gear});
            else points.push_back({-1, Symbol});
        }
        data.push_back(points);
    }
}

int find_start_col(std::vector<Point>& row, int anchor) {
    int start_col = anchor - 1;
    for (; start_col >= 0 && row[start_col].type == Value; start_col--) {}
    return start_col + 1;
}

int find_stop_col(std::vector<Point>& row, int anchor) {
    int stop_col = anchor + 1;
    for (; stop_col < (int)row.size() && row[stop_col].type == Value; stop_col++) {}
    return stop_col;
}

int part1(std::vector<std::vector<Point>>& data) {
    int rows = data.size(), cols = data[0].size();
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            Point& p = data[row][col];
            if (p.type != Value) continue;

            bool positive = false;
            for (const CartCoord& dir : dirs) {
                int row_d = row + dir.row, col_d = col + dir.col;
                if (row_d < 0 || row_d >= rows || col_d < 0 || col_d >= cols)
                    continue;
                if (data[row_d][col_d].type == Symbol) {
                    positive = true;
                    break;
                }
            }
            p.positive_part = positive;
        }
    }

    std::vector<int> values;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            Point& p = data[row][col];
            if (p.type != Value || !p.positive_part || p.part_accounted_for) continue;

            int start_col = find_start_col(data[row], col);
            int stop_col = find_stop_col(data[row], col);

            int value = 0;
            for (int power = 0; power < stop_col - start_col; power++) {
                int index = stop_col - 1 - power;
                data[row][index].part_accounted_for = true;
                int digit = data[row][index].value;
                value += pow(10, power) * digit;
            }
            values.push_back(value);                      
        }
    }
    return std::accumulate(values.begin(), values.end(), 0);
}

int part2(std::vector<std::vector<Point>> data) {
    std::vector<int> scores;
    int rows = data.size(), cols = data[0].size();
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            Point& p = data[row][col];
            if (p.symbol_type != Gear) continue;

            std::vector<CartCoord> matches;
            for (const CartCoord& dir : dirs) {
                int row_d = row + dir.row, col_d = col + dir.col;
                if (row_d < 0 || row_d >= rows || col_d < 0 || col_d >= cols)
                    continue;
                if (data[row_d][col_d].type == Value)
                    matches.push_back({row_d, col_d});
            }

            std::vector<int> gear_score;
            for (CartCoord& match : matches) {
                if (data[match.row][match.col].gear_accounter_for) continue;

                int start_col = find_start_col(data[match.row], match.col);
                int stop_col = find_stop_col(data[match.row], match.col);

                int value = 0;
                for (int power = 0; power < stop_col - start_col; power++) {
                    int index = stop_col - 1 - power;
                    data[match.row][index].gear_accounter_for = true;
                    int digit = data[match.row][index].value;
                    value += pow(10, power) * digit;
                }
                gear_score.push_back(value);
            }
            if (gear_score.size() == 2)
                scores.push_back(gear_score[0] * gear_score[1]);
        }   
    }
    return std::accumulate(scores.begin(), scores.end(), 0);
}

int main() {
    std::vector<std::vector<Point>> data;
    read_data(data);

    std::cout << part1(data) << std::endl;
    std::cout << part2(data) << std::endl;
}
