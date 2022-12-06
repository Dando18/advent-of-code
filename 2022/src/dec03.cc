/**
 * @file dec03.cc
 * @author Daniel Nichols
 * @date December 2022
 * @brief AOC 2022 Day 3 https://adventofcode.com/2022/day/3
 * 
 */
// stl includes
#include <iostream>
#include <numeric>
#include <string>

// local includes
#include "utilities.hpp"

constexpr auto INPUT_FILE_PATH = "/home/daniel/dev/personal/advent-of-code/2022/inputs/dec03.txt";


/*char getSharedItem(std::string const& a, std::string const& b) {
    auto it = std::find_if(std::begin(a), std::end(a), [&b](auto const& c) {
        return b.find(c) != std::string::npos;
    });
    
    return *it;
}*/

char getSharedItem(std::vector<std::string> strs) {
    auto first = *std::begin(strs);
    auto it = std::find_if(std::begin(first), std::end(first), [&strs](auto const& c) {
        return std::all_of(std::next(std::begin(strs), 1), std::end(strs), [&c](auto const& s) {
            return s.find(c) != std::string::npos;
        });
    });
    return *it;
} 

uint64_t getItemPriority(const char item) {
    if (item >= 'a' && item <= 'z') {
        return item - 'a' + 1;
    } else if (item >= 'A' && item <= 'Z') {
        return item - 'A' + 27;
    } else {
        std::cerr << "Unknown item: '" << item << "'\n";
        return 0;
    }
}

int main() {

    auto lines = util::readLines(INPUT_FILE_PATH);

    // part 1 -- sum of all priorities
    uint64_t prioritySum = 0;
    for (auto const& line : lines) {
        const char sharedItem = getSharedItem({line.substr(0, line.size()/2), line.substr(line.size()/2)});
        prioritySum += getItemPriority(sharedItem);
    }
    std::cout << prioritySum << "\n";

    // part 2 -- sum of all group badge priorities
    constexpr size_t GROUP_SIZE = 3;
    prioritySum = 0;
    for (size_t i = 0; i < lines.size(); i += GROUP_SIZE) {
        std::vector<std::string> group(std::next(std::begin(lines), i), std::next(std::begin(lines), i+GROUP_SIZE));
        const char badgeItem = getSharedItem(group);
        prioritySum += getItemPriority(badgeItem);
    }
    std::cout << prioritySum << "\n";
}