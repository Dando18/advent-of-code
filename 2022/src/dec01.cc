/**
 * @file dec01.cc
 * @author Daniel Nichols
 * @date December 2022
 * @brief AOC 2022 Day 1 https://adventofcode.com/2022/day/1
 *
 */
// stl includes
#include <iostream>
#include <numeric>
#include <string>

// local includes
#include "utilities.hpp"

constexpr auto INPUT_FILE_PATH = "/home/daniel/dev/personal/advent-of-code/2022/inputs/dec01.txt";

int main() {

    auto lines = util::readLines(INPUT_FILE_PATH);
    std::vector<uint64_t> totals;

    uint64_t sum = 0;
    for (auto const &l : lines) {
        if (l.size() == 0 || l == "") {
            totals.push_back(sum);
            sum = 0;
        } else {
            sum += std::stoull(l);
        }
    }
    totals.push_back(sum);

    // part 1 -- max total
    auto maxTotal = *std::max_element(std::begin(totals), std::end(totals));
    std::cout << maxTotal << "\n";

    // part 2 -- sum of top K totals
    constexpr uint32_t K = 3;
    std::sort(std::begin(totals), std::end(totals));
    sum = std::accumulate(std::next(std::begin(totals), totals.size() - K), std::end(totals), 0, std::plus<uint64_t>());
    std::cout << sum << "\n";
}