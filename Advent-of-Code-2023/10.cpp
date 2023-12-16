#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <map>

class Coords {
public:
    int row, col;

    Coords(int r, int c) {
        row = r;
        col = c;
    }

    Coords operator+(const Coords& c) const {
        return { this->row + c.row, this->col + c.col };
    }

    bool operator==(const Coords& c) const {
        return row == c.row && col == c.col;
    }
};

class Map : public std::vector<std::vector<char>> {
public:
    char operator[](const Coords& c) {
        return this->at(c.row).at(c.col);
    }

    char operator[](const Coords& c) const {
        return this->at(c.row).at(c.col);
    }

    Coords find_start() const {
        for (int i_row = 0; i_row < (int)this->size(); i_row++) {
            const std::vector<char>& row = this->at(i_row);
            for (int i_col = 0; i_col < (int)row.size(); i_col++) {
                if (row[i_col] == 'S')
                    return { i_row, i_col };
            }
        }
        return { -1, -1 };
    }

    std::size_t rows() const {
        return this->size();
    }

    std::size_t cols() const {
        return this->at(0).size();
    }

    bool coords_valid(const Coords& c) const {
        return c.row >= 0 && c.row < rows() && c.col >= 0 && c.col < cols();
    }

    void get_connected_neighbors(const Coords& c, std::vector<Coords>& neighbors, const Coords& prev) const {
        static const Coords up = { -1, 0 }, down = { 1, 0 }, left = { 0, -1 }, right = { 0, 1 };
        static const std::map<char, std::vector<Coords>> dir_map = {
            { 'S', { down, right, up, left } },
            { '|', { up, down } },
            { '-', { left, right } },
            { 'L', { up, right } },
            { 'J', { up, left } },
            { '7', { down, left } },
            { 'F', { down, right } },
            { '.', {} },
        };
        char current_symbol = (*this)[c];
        neighbors.clear();
        for (const Coords& dir : dir_map.at(current_symbol)) {
            Coords new_coords = c + dir;
            if (!coords_valid(new_coords) || new_coords == prev)
                continue;
            if ((*this)[new_coords] != '.')
                neighbors.push_back(new_coords);
        }
    }
};

void read_data(Map& data) {
    std::ifstream infile("./data/10.txt");
    std::string line;

    while(std::getline(infile, line)) {
        data.push_back(std::vector<char>(line.begin(), line.end()));
    }
}

template <typename T>
bool contains(const std::vector<T>& vec, const T& val) {
    for (auto& v : vec)
        if (v == val)
            return true;
    return false;
}

void find_longest_loop(const Map& data, std::vector<Coords>& longest_loop) {
    const Coords start = data.find_start();
    std::vector<std::vector<Coords>> paths;

    std::vector<Coords> neighbors;
    data.get_connected_neighbors(start, neighbors, start);
    for (const Coords& c : neighbors)
        paths.push_back({ c });
    
    std::vector<std::vector<Coords>> loops;
    for (std::vector<Coords>& path : paths) {
        Coords prev = start;
        Coords current = path[0];

        while (true) {
            data.get_connected_neighbors(current, neighbors, prev);
            if (neighbors.size() == 0) {
                break;
            }

            Coords next = neighbors[0];
            path.push_back(next);

            if (data[next] == 'S') {
                loops.push_back(path);
                break;
            }
            
            prev = current;
            current = next;
        }
    }

    longest_loop = loops[0];
    for (const std::vector<Coords>& loop : loops) {
        if (loop.size() > longest_loop.size())
            longest_loop = loop;
    }
}

int part1(const Map& data) {
    std::vector<Coords> longest_loop;
    find_longest_loop(data, longest_loop);

    return ceil((double)longest_loop.size() / 2);
}

int part2(const Map& data) {
    std::vector<Coords> longest_loop;
    find_longest_loop(data, longest_loop);

    std::size_t dots = 0;
    
    for (int i_row = 0; i_row < (int)data.size(); i_row++) {
        const std::vector<char>& row = data.at(i_row);

        bool closed = false;
        for (int i_col = 0; i_col < (int)row.size(); i_col++) {
            Coords c(i_row, i_col);
            char s = data[c];
            if (contains(longest_loop, c)) {
                if (s == '|' || s == 'J' || s == 'L')
                    closed = !closed;
            }
            else {
                dots += closed;
            }
        }
    }
    return dots;
}

int main() {
    Map data;
    read_data(data);

    std::cout << part1(data) << std::endl;
    std::cout << part2(data) << std::endl;

    return 0;
}