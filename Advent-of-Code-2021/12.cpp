#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>

class Node {
    static int node_id;
    bool _revisitable;
    int _n_visited = 0;
    int _n_visit_max = 1;
    bool _is_start, _is_end;
    int _id;
public:
    Node(bool revisitable, bool is_start = false, bool is_end = false, bool is_med_sized = false) : _id(node_id), _revisitable(revisitable), _is_start(is_start), _is_end(is_end) {
        if (is_start) _n_visited = 1;
        if (is_med_sized) _n_visit_max = 2;
        node_id++;
    }
    Node(const Node& node) : _id(node._id), _revisitable(node._revisitable), _is_start(node._is_start), _is_end(node._is_end), _n_visited(node._n_visited), _n_visit_max(node._n_visit_max) {}
    
    static Node* get_node(std::string s, std::map<std::string, Node*>& nodes, bool is_med_sized = false) {
        auto node_at = nodes.find(s);
        if (node_at == nodes.end()) {
            bool rev = std::all_of(s.begin(), s.end(), [](unsigned char c){ return std::isupper(c); });
            Node* node = new Node(rev, s == std::string("start"), s == std::string("end"), is_med_sized);
            nodes[s] = node;
            return node;
        } else return node_at->second;
    }

    static Node* get_node(const Node& node_cpy, std::map<int, Node*>& nodes, bool is_med_sized = false) {
        auto node_at = nodes.find(node_cpy.id());
        if (node_at == nodes.end()) {
            Node* node = new Node(node_cpy);
            nodes[node_cpy.id()] = node;
            return node;
        } else return node_at->second;
    }
    
    int id() const { return _id; }
    bool is_start() const { return _is_start; }
    bool is_end() const { return _is_end; }
    bool is_visit_capped() const { return _n_visited >= _n_visit_max; }
    bool is_revisitable() const { return _revisitable; }
    void visit() {
        if (!_revisitable) _n_visited++;
    }
    void promote() { _n_visit_max = 2; }
    void demote() { _n_visit_max = 1; }
    bool operator==(const Node& node) const { return _id == node._id; }
};
int Node::node_id = 0;

class Path {
    bool _taken = false;
    Node *_start = nullptr, *_end = nullptr;
public:
    Path(Node* start, Node* end) : _start(start), _end(end) {}
    Path(const Path& path) {
        _start = new Node(*path._start);
        _end = new Node(*path._end);
        _taken = path._taken;
    }
    Path(const Path& path, std::map<int, Node*>& nodes) {
        _start = Node::get_node(*path._start, nodes);
        _end = Node::get_node(*path._end, nodes);
        _taken = path._taken;
    }
    Node* start() { return _start; }
    Node* end() { return _end; }
    bool is_taken() const { return _taken; }
    void take() {
        _taken = true;
        _end->visit();
    }
    bool operator==(const Path& path) const { return *_start == *(path._start) && *_end == *(path._end); }
    ~Path() {
        if (_start != nullptr) delete _start;
        if (_end != nullptr) delete _end;
    }
};

void read_data(std::vector<Path*>& paths, std::map<std::string, Node*>& nodes) {
    std::ifstream infile("data/12_data.txt");
    std::string line;

    while(std::getline(infile, line)) {
        int delim_idx = line.find('-');
        Node* lnode = Node::get_node(line.substr(0, delim_idx), nodes);
        Node* rnode = Node::get_node(line.substr(delim_idx+1, line.size()), nodes);
        paths.push_back(new Path(lnode, rnode));
        if (!lnode->is_start()) paths.push_back(new Path(rnode, lnode));
    }
}

void delete_nodes(std::map<std::string, Node*>& nodes) {
    for (auto& node : nodes) if (node.second != nullptr) delete node.second;
}

void calculate_number_of_paths(std::vector<Path*>& paths, int& curr_count, int start_id) {
    for (auto& path : paths) {
        if (path->start()->id() != start_id) continue;
        // if (path->is_taken()) continue;
        if (path->end()->is_start()) continue;
        if (path->end()->is_visit_capped()) continue;
        if (path->end()->is_end()) { curr_count++; continue; }
        std::vector<Path*> new_paths;
        std::map<int, Node*> node_dict;
        for (auto& new_path : paths) {
            Path* np = new Path(*new_path, node_dict);
            new_paths.push_back(np);
            if (new_path == path) np->take();
        }
        calculate_number_of_paths(new_paths, curr_count, path->end()->id());
    }
}

int part1() {
    std::map<std::string, Node*> nodes;
    std::vector<Path*> paths;
    read_data(paths, nodes);

    int start_id;
    for (auto& path : paths) if(path->start()->is_start()) { start_id = path->start()->id(); break; }

    int number_of_paths = 0;
    calculate_number_of_paths(paths, number_of_paths, start_id);

    delete_nodes(nodes);
    return number_of_paths;
}

int part2(int base_number_of_paths) {
    std::map<std::string, Node*> nodes;
    std::vector<Path*> paths;
    read_data(paths, nodes);

    int start_id;
    for (auto& path : paths) if(path->start()->is_start()) { start_id = path->start()->id(); break; }
    
    int number_of_paths_total = base_number_of_paths;
    for (auto& node : nodes) {
        if (node.second->is_revisitable() || node.second->is_start() || node.second->is_end()) continue;
        node.second->promote();
        int curr_number_of_paths = -base_number_of_paths;
        calculate_number_of_paths(paths, curr_number_of_paths, start_id);
        number_of_paths_total += curr_number_of_paths;
        node.second->demote();
    }

    delete_nodes(nodes);
    return number_of_paths_total;
}

int main() {
    int base_number_of_paths = part1();
    std::cout << base_number_of_paths << std::endl;
    std::cout << part2(base_number_of_paths) << std::endl;
    
    return 0;
}