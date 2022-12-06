/**
 * @file utilities.hpp
 * @author Daniel Nichols
 * @brief Utility routines
 * @date December 2022
 * 
 */
// stl includes
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <vector>

namespace util {


std::vector<std::string> readLines(std::string const& fname) {
    std::ifstream infile(fname);
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(infile, line)) {
        lines.push_back(line);
    }

    return lines;
}

std::vector<std::string> split(std::string const& s, char const delim) {
    std::vector<std::string> parts;
    std::stringstream ss(s);
    std::string part;

    while (std::getline(ss, part, delim)) {
        parts.push_back(part);
    }
    return parts;
}

template <typename Iterator>
bool hasDuplicates(Iterator first, Iterator last) {
    std::unordered_set<std::iter_value_t<Iterator>> set(first, last);
    return set.size() != static_cast<size_t>( std::distance(first, last) );
}


} // end namespace util