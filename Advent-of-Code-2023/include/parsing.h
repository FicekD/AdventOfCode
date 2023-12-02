#ifndef _PARSING_H
#define _PARSING_H

#include <string>
#include <vector>

namespace parsing {
    void split(const std::string& string, const std::string& delim, std::vector<std::string>& output) {
        size_t pos_start = 0, pos_end, delim_len = delim.length();
        std::string token;
        while ((pos_end = string.find(delim, pos_start)) != std::string::npos) {
            token = string.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            output.push_back(token);
        }
        output.push_back(string.substr(pos_start));
    }
}

#endif