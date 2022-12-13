/**
 * @file dec10.cc
 * @author Daniel Nichols
 * @date December 2022
 * @brief AOC 2022 Day 10 https://adventofcode.com/2022/day/10
 *
 */
// stl includes
#include <iostream>
#include <sstream>
#include <string>

// local includes
#include "utilities.hpp"

constexpr auto INPUT_FILE_PATH = "/home/daniel/dev/personal/advent-of-code/2022/inputs/dec10.txt";

void increment(uint32_t &cycleNum, int64_t curValue, int64_t &totalSignalStrength, std::ostream &crt) {
    cycleNum += 1;
    if ((static_cast<int64_t>(cycleNum) - 20) % 40 == 0) {
        totalSignalStrength += curValue * cycleNum;
    }

    const int64_t pixel = ((cycleNum - 1) % 40);
    if (std::abs(curValue - pixel) <= 1) {
        crt << '#';
    } else {
        crt << '.';
    }
    if (pixel == 39) {
        crt << '\n';
    }
}

int main() {

    const auto lines = util::readLines(INPUT_FILE_PATH);

    // part 1 -- execute instructions and report total signal strength from selected cycles
    int64_t value = 1, totalSignalStrength = 0;
    uint32_t cycleNum = 0;
    std::ostringstream crt;
    for (auto const &line : lines) {
        if (line == "noop") {
            increment(cycleNum, value, totalSignalStrength, crt);
        } else if (line.starts_with("addx")) {
            const auto parts = util::split(line, ' ');
            const int64_t amount = std::stoll(parts.back());

            increment(cycleNum, value, totalSignalStrength, crt);
            increment(cycleNum, value, totalSignalStrength, crt);
            value += amount;
        }
    }
    std::cout << totalSignalStrength << "\n";

    // part 2 -- print crt message
    std::cout << crt.str();
}