/**
 * @file dec06.cc
 * @author Daniel Nichols
 * @date December 2022
 * @brief AOC 2022 Day 6 https://adventofcode.com/2022/day/6
 * 
 */
// stl includes
#include <deque>
#include <iostream>
#include <numeric>
#include <stack>
#include <string>
#include <vector>

// local includes
#include "utilities.hpp"

constexpr auto INPUT_FILE_PATH = "/home/daniel/dev/personal/advent-of-code/2022/inputs/dec06.txt";

uint32_t findMarker(std::string const& buffer, const uint32_t markerLength) {
    std::deque<char> marker(std::begin(buffer), std::next(std::begin(buffer), markerLength));

    if (!util::hasDuplicates(std::begin(marker), std::end(marker))) {
        return markerLength;
    }

    for (uint32_t i = markerLength; i < buffer.size(); i += 1) {
        marker.pop_front();
        marker.push_back(buffer.at(i));

        if (!util::hasDuplicates(std::begin(marker), std::end(marker))) {
            return i + 1;
        }
    }

    return buffer.size();
}


int main() {

    auto lines = util::readLines(INPUT_FILE_PATH);
    const std::string BUFFER = lines.at(0);

    // part 1 -- return marker position with message length of 4
    auto markerPosition = findMarker(BUFFER, 4);
    std::cout << markerPosition << "\n";

    // part 2 -- return marker position with message length of 14
    markerPosition = findMarker(BUFFER, 14);
    std::cout << markerPosition << "\n";
}