#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

struct Coords {
    int row, col;
};

class Map : public std::vector<std::vector<char>> {
public:
    std::size_t rows() const {
        return this->size();
    }

    std::size_t cols() const {
        return this->at(0).size();
    }

    void empty_rows(std::vector<std::size_t>& rows) const {
        for (std::size_t i_row = 0; i_row < this->size(); i_row++) {
            const std::vector<char>& row = this->at(i_row);
            bool row_empty = true;
            for (std::size_t i_col = 0; row_empty && i_col < row.size(); i_col++) {
                if (row[i_col] == '#')
                    row_empty = false;
            }
            if (row_empty)
                rows.push_back(i_row);
        }
    }

    void empty_cols(std::vector<std::size_t>& cols) const {
        for (std::size_t i_col = 0; i_col < this->cols(); i_col++) {
            bool col_empty = true;
            for (std::size_t i_row = 0; col_empty && i_row < this->rows(); i_row++) {
                if (this->at(i_row).at(i_col) == '#')
                    col_empty = false;
            }
            if (col_empty)
                cols.push_back(i_col);
        }
    }

    void find_all(char val, std::vector<Coords>& coords) const {
        for (std::size_t i_row = 0; i_row < this->size(); i_row++) {
            const std::vector<char>& row = this->at(i_row);
            for (std::size_t i_col = 0; i_col < row.size(); i_col++) {
                if (row[i_col] == val)
                    coords.push_back(Coords{ (int)i_row, (int)i_col });
            }
        }
    }
};

std::size_t n_between(const std::vector<std::size_t>& values, std::size_t lower, std::size_t upper) {
    std::size_t n = 0;
    for (std::size_t val : values) {
        if (val > upper)
            break;
        if (val > lower)
            n++;
    }
    return n;
}

void read_data(Map& data) {
    std::ifstream infile("./data/11.txt");
    std::string line;

    while(std::getline(infile, line)) {
        data.push_back(std::vector<char>(line.begin(), line.end()));
    }
}

std::size_t find_shortest_paths(const Map& data, std::size_t growth) {
    std::vector<Coords> galaxies;
    data.find_all('#', galaxies);
    std::vector<std::size_t> empty_rows, empty_cols;
    data.empty_rows(empty_rows);
    data.empty_cols(empty_cols);

    std::size_t sum_shortest_paths = 0;
    for (std::size_t i = 0; i < galaxies.size() - 1; i++) {
        for (std::size_t j = i + 1; j < galaxies.size(); j++) {
            Coords g1 = galaxies[i], g2 = galaxies[j];
            std::size_t row_dist = g1.row <= g2.row ?
                g2.row - g1.row + growth * n_between(empty_rows, g1.row, g2.row) :
                g1.row - g2.row + growth * n_between(empty_rows, g2.row, g1.row);
            std::size_t col_dist = g1.col <= g2.col ?
                g2.col - g1.col + growth * n_between(empty_cols, g1.col, g2.col) :
                g1.col - g2.col + growth * n_between(empty_cols, g2.col, g1.col);
            sum_shortest_paths += row_dist + col_dist;
        }
    }
    return sum_shortest_paths;
}

int main() {
    Map data;
    read_data(data);

    std::cout << find_shortest_paths(data, 1) << std::endl;
    std::cout << find_shortest_paths(data, 1000000 - 1) << std::endl;

    return 0;
}