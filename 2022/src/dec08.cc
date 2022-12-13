/**
 * @file dec08.cc
 * @author Daniel Nichols
 * @date December 2022
 * @brief AOC 2022 Day 8 https://adventofcode.com/2022/day/8
 *
 */
// stl includes
#include <iostream>
#include <string>
#include <vector>

// local includes
#include "utilities.hpp"

constexpr auto INPUT_FILE_PATH = "/home/daniel/dev/personal/advent-of-code/2022/inputs/dec08.txt";

bool isVisible(std::vector<std::vector<uint32_t>> const &trees, uint32_t row, uint32_t col) {
    if (row == 0 || col == 0 || row == trees.size() - 1 || col == trees.front().size() - 1) {
        return true;
    }
    const uint32_t height = trees.at(row).at(col);

    // check above
    bool visible = true;
    for (uint32_t i = 0; i < row; i += 1) {
        if (trees.at(i).at(col) >= height) visible = false;
    }
    if (visible) return true;

    // check below
    visible = true;
    for (int32_t i = trees.size() - 1; i > static_cast<int32_t>(row); i -= 1) {
        if (trees.at(i).at(col) >= height) visible = false;
    }
    if (visible) return true;

    // check left
    visible = true;
    for (uint32_t i = 0; i < col; i += 1) {
        if (trees.at(row).at(i) >= height) visible = false;
    }
    if (visible) return true;

    // check right
    for (int32_t i = trees.front().size() - 1; i > static_cast<int32_t>(col); i -= 1) {
        if (trees.at(row).at(i) >= height) return false;
    }

    return true;
}

uint32_t scenicScore(std::vector<std::vector<uint32_t>> const &trees, uint32_t row, uint32_t col) {
    const uint32_t height = trees.at(row).at(col);

    // above viewing distance
    uint32_t aboveDistance = 0;
    for (int32_t i = row - 1; i >= 0; i -= 1) {
        aboveDistance += 1;
        if (trees.at(i).at(col) >= height) break;
    }

    // below viewing distance
    uint32_t belowDistance = 0;
    for (uint32_t i = row + 1; i < trees.size(); i += 1) {
        belowDistance += 1;
        if (trees.at(i).at(col) >= height) break;
    }

    // left viewing distance
    uint32_t leftDistance = 0;
    for (int32_t i = col - 1; i >= 0; i -= 1) {
        leftDistance += 1;
        if (trees.at(row).at(i) >= height) break;
    }

    // below viewing distance
    uint32_t rightDistance = 0;
    for (uint32_t i = col + 1; i < trees.front().size(); i += 1) {
        rightDistance += 1;
        if (trees.at(row).at(i) >= height) break;
    }

    return aboveDistance * belowDistance * leftDistance * rightDistance;
}

int main() {

    const auto lines = util::readLines(INPUT_FILE_PATH);
    std::vector<std::vector<uint32_t>> trees(lines.size(), std::vector<uint32_t>(lines.at(0).size()));
    for (size_t i = 0; i < lines.size(); i += 1) {
        for (size_t j = 0; j < lines.at(i).size(); j += 1) {
            trees.at(i).at(j) = lines.at(i).at(j) - '0';
        }
    }

    // part 1 -- count number of visible trees
    uint32_t numVisible = 0;
    for (size_t i = 0; i < trees.size(); i += 1) {
        for (size_t j = 0; j < trees.at(i).size(); j += 1) {
            numVisible += (isVisible(trees, i, j)) ? 1 : 0;
        }
    }
    std::cout << numVisible << "\n";

    // part 2 -- max scenic score
    uint32_t maxScenicScore = 0;
    for (size_t i = 0; i < trees.size(); i += 1) {
        for (size_t j = 0; j < trees.at(i).size(); j += 1) {
            maxScenicScore = std::max(maxScenicScore, scenicScore(trees, i, j));
        }
    }
    std::cout << maxScenicScore << "\n";
}