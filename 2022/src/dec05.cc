/**
 * @file dec05.cc
 * @author Daniel Nichols
 * @date December 2022
 * @brief AOC 2022 Day 5 https://adventofcode.com/2022/day/5
 * 
 */
// stl includes
#include <iostream>
#include <numeric>
#include <stack>
#include <string>
#include <vector>

// local includes
#include "utilities.hpp"

constexpr auto INPUT_FILE_PATH = "/home/daniel/dev/personal/advent-of-code/2022/inputs/dec05.txt";

typedef char Crate;
struct Move {
    uint32_t src, dst, amt;
};

std::vector<std::stack<Crate>> parseCrates(std::vector<std::string> lines) {
    /* parse how many stacks there are and create vector of according size */
    const auto lastLine = lines.back();
    lines.pop_back();
    auto nums = util::split(lastLine, ' ');
    auto removed = std::remove_if(std::begin(nums), std::end(nums), [](auto x) { return x == ""; });
    nums.erase(removed, std::end(nums));
    const uint32_t maxNum = std::stoul( nums.back() );
    std::vector<std::stack<Crate>> stacks(maxNum);

    /* read in crates */
    std::reverse(std::begin(lines), std::end(lines));
    for (auto const& line : lines) {
        for (uint32_t i = 0; i < line.size(); i += 4) {
            Crate c = line.at(i + 1);
            if (c != ' ')
                stacks.at(i / 4).push( c );
        }
    }

    return stacks;
}

std::vector<Move> parseMoves(std::vector<std::string> const& lines) {
    std::vector<Move> moves;
    for (auto const& line : lines) {
        auto parts = util::split(line, ' ');
        Move move;
        move.amt = std::stoul(parts.at(1));
        move.src = std::stoul(parts.at(3));
        move.dst = std::stoul(parts.at(5));
        moves.push_back(move);
    }
    return moves;
}


void doMovePart1(std::vector<std::stack<Crate>>& stacks, Move const& move) {
    auto& src = stacks.at(move.src - 1);
    auto& dst = stacks.at(move.dst - 1);

    for (uint32_t i = 0; i < move.amt; i += 1) {
        Crate crate = src.top();
        src.pop();
        dst.push(crate);
    }
}

void doMovePart2(std::vector<std::stack<Crate>>& stacks, Move const& move) {
    auto& src = stacks.at(move.src - 1);
    auto& dst = stacks.at(move.dst - 1);
    std::stack<Crate> tmp;

    for (uint32_t i = 0; i < move.amt; i += 1) {
        Crate crate = src.top();
        src.pop();
        tmp.push(crate);
    }

    while (!tmp.empty()) {
        dst.push(tmp.top());
        tmp.pop();
    }
}


int main() {

    auto lines = util::readLines(INPUT_FILE_PATH);
    std::vector<std::string> cratesString, moveStrings;
    for (auto const& line : lines) {
        if (line == "") {
            continue;
        } else if (line.starts_with("move")) {
            moveStrings.push_back( line );
        } else {
            cratesString.push_back( line );
        }
    }

    const auto stacksOriginal = parseCrates(cratesString);
    const auto moves = parseMoves(moveStrings);

    // part 1 -- print top crates in final configuration
    auto stacks = stacksOriginal;
    for (auto const& move : moves) {
        doMovePart1(stacks, move);
    }
    for (auto const& s : stacks) {
        std::cout << s.top();
    }
    std::cout << "\n";

    // part 2 -- print top crates in final configuration
    stacks = stacksOriginal;
    for (auto const& move : moves) {
        doMovePart2(stacks, move);
    }
    for (auto const& s : stacks) {
        std::cout << s.top();
    }
    std::cout << "\n";
}