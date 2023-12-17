#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

void read_data(std::vector<std::vector<std::vector<char>>>& data) {
    std::ifstream infile("./data/13.txt");
    std::string line;

    std::vector<std::vector<char>> current_map;
    while(std::getline(infile, line)) {
        if (line.size() == 0) {
            data.push_back(current_map);
            current_map = std::vector<std::vector<char>>();
        }
        else {
            current_map.push_back(std::vector<char>(line.begin(), line.end()));
        }
    }
    data.push_back(current_map);
}

long encode_vector(const std::vector<char>& vec) {
    long code = 0;
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == '.')
            code += powl(2, i);
    }
    return code;
}

int find_symmetry(const std::vector<long>& vec, int ignore_idx = -1) {
    for (std::size_t i = 0; i < vec.size() - 1; i++) {
        if (i == ignore_idx)
            continue;
        if (vec[i] == vec[i + 1]) {
            std::size_t d = vec.size() - 1 - i < i + 1 ? vec.size() - 1 - i : i + 1;
            bool symmetric = true;
            for (std::size_t j = 1; j < d; j++) {
                if (vec[i - j] != vec[i + 1 + j]) {
                    symmetric = false;
                    break;
                }
            }
            if (symmetric)
                return i + 1;
        }
    }
    return -1;
}

int reflection_score(const std::vector<std::vector<char>>& map, int ignore_score = -1) {
    std::vector<long> row_encodement;
    std::vector<long> col_encodement;

    for (const std::vector<char>& row : map)
        row_encodement.push_back(encode_vector(row));
    for (std::size_t col_i = 0; col_i < map[0].size(); col_i++) {
        std::vector<char> col;
        for (std::size_t row_i = 0; row_i < map.size(); row_i++) {
            col.push_back(map[row_i][col_i]);
        }
        col_encodement.push_back(encode_vector(col));
    }
    
    int row_pivot = find_symmetry(row_encodement, ignore_score >= 100 ? ignore_score / 100 - 1 : -1);
    if (row_pivot != -1) {
        return 100 * row_pivot;
    }

    int col_pivot = find_symmetry(col_encodement, ignore_score < 100 ? ignore_score - 1 : -1);
    if (col_pivot != -1) {
        return col_pivot;
    }
    return -1;
}

std::size_t part1(const std::vector<std::vector<std::vector<char>>>& data) {
    std::size_t reflections = 0;
    for (const std::vector<std::vector<char>>& map : data) {
        int score = reflection_score(map);
        reflections += score;
    }
    return reflections;
}

std::size_t part2(const std::vector<std::vector<std::vector<char>>>& data) {
    std::size_t reflections = 0;
    for (const std::vector<std::vector<char>>& map : data) {
        int original_score = reflection_score(map);
        int new_score = 0;
        std::vector<std::vector<char>> duplicate(map.begin(), map.end());
        for (std::size_t row_i = 0; row_i < map.size() && new_score == 0; row_i++) {
            for (std::size_t col_i = 0; col_i < map[0].size(); col_i++) {
                char& ref = duplicate[row_i][col_i];
                
                ref = ref == '.' ? '#' : '.';
                int score = reflection_score(duplicate, original_score);
                ref = ref == '.' ? '#' : '.';

                if (score != original_score && score != -1) {
                    new_score = score;
                    break;
                }
            }
        }
        if (new_score == 0) {
            new_score = original_score;
        }
        reflections += new_score;
    }
    return reflections;
}

int main() {
    std::vector<std::vector<std::vector<char>>> data;
    read_data(data);

    std::cout << part1(data) << std::endl;
    std::cout << part2(data) << std::endl;

    return 0;
}