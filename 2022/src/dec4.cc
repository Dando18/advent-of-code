/**
 * @file dec4.cc
 * @author Daniel Nichols
 * @date December 2022
 * @brief AOC 2022 Day 4 https://adventofcode.com/2022/day/4
 * 
 */
// stl includes
#include <iostream>
#include <numeric>
#include <string>

// local includes
#include "utilities.hpp"

constexpr auto INPUT_FILE_PATH = "/home/daniel/dev/personal/advent-of-code/2022/inputs/dec4.txt";

struct Range {
    int64_t min, max;
};


Range parseRange(std::string const& s) {
    auto nums = util::split(s, '-');
    return {std::stoll(nums.at(0)), std::stoll(nums.at(1))};
}

/**
 * @brief Returns true if x fully contains y
 * 
 */
bool doesFullyContain(Range x, Range y) {
    return (x.min <= y.min) && (x.max >= y.max);
}

/**
 * @brief Returns true if x and y overlap
 */
bool doesOverlap(Range x, Range y) {
    return (x.min <= y.min && y.min <= x.max) || (x.min <= y.max && y.max <= x.max) || doesFullyContain(y, x);
}

int main() {

    auto lines = util::readLines(INPUT_FILE_PATH);
    std::vector<std::pair<Range, Range>> values;
    for (auto const& line : lines) {
        auto parts = util::split(line, ',');
        values.push_back( {parseRange(parts.at(0)), parseRange(parts.at(1))} );
    }

    // part 1 -- in how many pairs does a range fully contain the other
    const auto totalContains = std::count_if(std::begin(values), std::end(values), [](auto const& x) -> bool {
        return doesFullyContain(x.first, x.second) || doesFullyContain(x.second, x.first);
    });
    std::cout << totalContains << "\n";

    // part 2 -- in how many pairs do the ranges overlap
    const auto totalOverlap = std::count_if(std::begin(values), std::end(values), [](auto const& x) -> bool {
        return doesOverlap(x.first, x.second);
    });
    std::cout << totalOverlap << "\n";
}