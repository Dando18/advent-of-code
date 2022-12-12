/**
 * @file dec09.cc
 * @author Daniel Nichols
 * @date December 2022
 * @brief AOC 2022 Day 9 https://adventofcode.com/2022/day/9
 * 
 */
// stl includes
#include <array>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// local includes
#include "utilities.hpp"

constexpr auto INPUT_FILE_PATH = "/home/daniel/dev/personal/advent-of-code/2022/inputs/dec09.txt";


enum Direction {
    UP, DOWN, LEFT, RIGHT
};
std::unordered_map<char, Direction> char2Direction {{'U', UP}, {'D', DOWN}, {'L', LEFT}, {'R', RIGHT}};

struct Move {
    Direction dir;
    uint32_t amt;
};

struct Position {
    int64_t x, y;

    bool operator==(Position const& b) const noexcept {
        return (this->x == b.x) && (this->y == b.y);
    }
};

namespace std {
template<> struct hash<Position> {
    std::size_t operator()(const Position& pos) const noexcept {
        return std::hash<int64_t>()(pos.x) * 31 + std::hash<int64_t>()(pos.y);
    }
};
}


void moveHead(Position &head, Direction dir, uint32_t amt = 1) {
    switch (dir) {
        case UP:    head.y += amt; break;
        case DOWN:  head.y -= amt; break;
        case LEFT:  head.x -= amt; break;
        case RIGHT: head.x += amt; break;
        default: break;
    }
}


void updateTail(Position const& head, Position &tail) {
    /* early exit if still touching */
    if ( (std::abs(head.x - tail.x) < 2) && (std::abs(head.y - tail.y) < 2) ) {
        return;
    }

    if (head.y == tail.y) {
        /* same row */
        tail.x += (tail.x < head.x) ? 1 : -1;
    } else if (head.x == tail.x) {
        /* same col */
        tail.y += (tail.y < head.y) ? 1 : -1;
    } else {
        /* diagonal */
        tail.x += (tail.x < head.x) ? 1 : -1;
        tail.y += (tail.y < head.y) ? 1 : -1;
    }
}


void printGrid(std::ostream &oss, Position const& head, Position const& tail, int64_t gridSize) {
    for (int64_t i = 0; i < gridSize; i += 1) {
        for (int64_t j = 0; j < gridSize; j += 1) {
            if (head.y == (gridSize-i-1) && head.x == j) {
                oss << 'H';
            } else if (tail.y == (gridSize-i-1) && tail.x == j) {
                oss << 'T';
            } else {
                oss << '.';
            }
        }
        oss << '\n';
    }
}


int main() {

    const auto lines = util::readLines(INPUT_FILE_PATH);
    std::vector<Move> moves;
    for (auto const& line : lines) {
        const auto parts = util::split(line, ' ');
        moves.push_back( {char2Direction[parts.at(0).at(0)], static_cast<uint32_t>(std::stoul(parts.at(1)))} );
    }

    // part 1 -- simulate movements and return number of unique spaces tail covered
    Position head {0, 0}, tail {0, 0};
    std::unordered_set<Position> visited;
    visited.insert(tail);

    for (auto const& move : moves) {
        for (uint32_t i = 0; i < move.amt; i += 1) {
            moveHead(head, move.dir);
            updateTail(head, tail);
            visited.insert(tail);
        }
    }
    std::cout << visited.size() << "\n";

    // part 2 -- simulate movements of rope with more knots and return number of unique spaces last knot covered
    constexpr size_t NUM_KNOTS = 9;
    head = {0, 0};
    std::array<Position, NUM_KNOTS> tails({0, 0});
    visited.clear();
    visited.insert(tails.back());

    for (auto const& move : moves) {
        for (uint32_t i = 0; i < move.amt; i += 1) {
            /* first pair */
            moveHead(head, move.dir);
            updateTail(head, tails.front());

            /* rest of knots */
            for (size_t j = 1; j < tails.size(); j += 1) {
                updateTail(tails.at(j-1), tails.at(j));
            }

            visited.insert(tails.back());
        }
    }
    std::cout << visited.size() << "\n";
}