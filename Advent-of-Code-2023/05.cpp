#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <deque>
#include <tuple>

class ClosedRangeSet {
private:
    long long _start = 0, _end = 0;
public:
    ClosedRangeSet() {}
    ClosedRangeSet(long long start, long long end) {
        _start = start;
        _end = end;
    }
    long long start() const { return _start; }
    long long end() const { return _end; }

    std::tuple<ClosedRangeSet, ClosedRangeSet, ClosedRangeSet> intersection(const ClosedRangeSet& map_set) const {
        if (map_set.start() >= this->end() || this->start() >= map_set.end()) {
            throw std::range_error("");
        }
        long long max_start = this->start() > map_set.start() ? this->start() : map_set.start();
        long long min_end = this->end() < map_set.end() ? this->end() : map_set.end();

        return std::make_tuple(
            ClosedRangeSet(max_start, min_end),
            ClosedRangeSet(this->start(), max_start),
            ClosedRangeSet(min_end, this->end())
        );
    }

    bool empty() const {
        return _end <= _start;
    }

    void offset(long long offset_val) {
        _start += offset_val;
        _end += offset_val;
    }
};

struct Mapping {
    long long destination_start;
    long long source_start;
    long long range;
};

class Mapper {
private:
    std::vector<Mapping> _mappings;
public:
    Mapper(const std::vector<Mapping>& mappings) {
        std::copy(mappings.begin(), mappings.end(), std::back_inserter(_mappings));
    }

    const std::vector<Mapping>& mappings() const {
        return _mappings;
    }

    long long map(long long source) const {
        for (const Mapping& mapping : _mappings) {
            if (source >= mapping.source_start && (source < (mapping.source_start + mapping.range))) {
                return mapping.destination_start + (source - mapping.source_start);
            }
        }
        return source;
    }
};

void parse_longs(const std::string& s, std::vector<long long>& longs) {
    std::istringstream iss(s);
    long long num;
    while (iss >> num) longs.push_back(num);
}

Mapping parse_mapping(const std::string& s) {
    long long destination_start, source_start, range;
    std::istringstream iss(s);
    iss >> destination_start >> source_start >> range;
    return {destination_start, source_start, range};
}

void read_data(std::vector<long long>& seeds, std::vector<Mapper>& mappers) {
    std::ifstream infile("./data/05.txt");
    std::string line;

    std::getline(infile, line);
    line = line.substr(7);
    parse_longs(line, seeds);

    std::getline(infile, line);
    std::getline(infile, line);

    std::vector<Mapping> mappings;
    while(std::getline(infile, line)) {
        if (line.size() == 0) {
            mappers.push_back(Mapper(mappings));
            mappings.clear();
            std::getline(infile, line);
        }
        else {
            Mapping mapping = parse_mapping(line);
            mappings.push_back(mapping);
        }
    }
    mappers.push_back(Mapper(mappings));
}

long long part1(const std::vector<long long>& seeds, const std::vector<Mapper>& mappers) {
    std::vector<long long> locations;
    for (long long seed : seeds) {
        long long location = seed;
        for (const Mapper& mapper : mappers) {
            location = mapper.map(location);
        }
        locations.push_back(location);
    }
    return *std::min_element(locations.begin(), locations.end());
}

long long part2(const std::vector<long long>& seeds, const std::vector<Mapper>& mappers) {
    std::vector<long long> locations;

    for (std::size_t i = 0; i < seeds.size() / 2; i++) {
        std::deque<ClosedRangeSet> input_sets;
        input_sets.push_back(ClosedRangeSet(seeds[2 * i], seeds[2 * i] + seeds[2 * i + 1]));

        for (const Mapper& mapper : mappers) {
            std::deque<ClosedRangeSet> transformed_sets;
            while (!input_sets.empty()) {
                ClosedRangeSet input_set = input_sets.back();
                input_sets.pop_back();
                
                bool mapped = false;
                for (const Mapping& mapping : mapper.mappings()) {
                    ClosedRangeSet mapping_set(mapping.source_start, mapping.source_start + mapping.range);
                    ClosedRangeSet intersection, rcomp_a, rcomp_b;
                    try { 
                        std::tie(intersection, rcomp_a, rcomp_b) = input_set.intersection(mapping_set);
                    }
                    catch (const std::range_error& e) { continue; }

                    long long transform = mapping.destination_start - mapping.source_start;
                    intersection.offset(transform);
                    transformed_sets.push_back(intersection);
                    
                    if (!rcomp_a.empty()) input_sets.push_back(rcomp_a);
                    if (!rcomp_b.empty()) input_sets.push_back(rcomp_b);

                    mapped = true;
                    break;
                }

                if (!mapped) {
                    transformed_sets.push_back(input_set);
                }
            }
            input_sets = transformed_sets;
        }
        for (const ClosedRangeSet& set : input_sets)
            locations.push_back(set.start());
    }
    return *std::min_element(locations.begin(), locations.end());
}

int main() {
    std::vector<long long> seeds;
    std::vector<Mapper> mappers;
    read_data(seeds, mappers);

    std::cout << part1(seeds, mappers) << std::endl;
    std::cout << part2(seeds, mappers) << std::endl;

    return 0;
}