/**
 * @file dec09.cc
 * @author Daniel Nichols
 * @date December 2022
 * @brief AOC 2022 Day 9 https://adventofcode.com/2022/day/9
 *
 */
// stl includes
#include <array>
#include <cassert>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// stl includes
#include "gif.h"

// local includes
#include "utilities.hpp"

constexpr auto INPUT_FILE_PATH = "/home/daniel/dev/personal/advent-of-code/2022/inputs/dec09.txt";
constexpr bool VISUALIZATION = true;
constexpr uint32_t FINAL_FRAME_REPEAT = 200;

enum Direction { UP, DOWN, LEFT, RIGHT };
std::unordered_map<char, Direction> char2Direction{{'U', UP}, {'D', DOWN}, {'L', LEFT}, {'R', RIGHT}};

struct Move {
    Direction dir;
    uint32_t amt;
};

struct Position {
    int64_t x, y;

    bool operator==(Position const &b) const noexcept { return (this->x == b.x) && (this->y == b.y); }
};

namespace std {
template <> struct hash<Position> {
    std::size_t operator()(const Position &pos) const noexcept {
        return std::hash<int64_t>()(pos.x) * 31 + std::hash<int64_t>()(pos.y);
    }
};
} // namespace std

void moveHead(Position &head, Direction dir, uint32_t amt = 1) {
    switch (dir) {
    case UP:
        head.y += amt;
        break;
    case DOWN:
        head.y -= amt;
        break;
    case LEFT:
        head.x -= amt;
        break;
    case RIGHT:
        head.x += amt;
        break;
    default:
        break;
    }
}

void updateTail(Position const &head, Position &tail) {
    /* early exit if still touching */
    if ((std::abs(head.x - tail.x) < 2) && (std::abs(head.y - tail.y) < 2)) {
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

void printGrid(std::ostream &oss, Position const &head, Position const &tail, int64_t gridSize) {
    for (int64_t i = 0; i < gridSize; i += 1) {
        for (int64_t j = 0; j < gridSize; j += 1) {
            if (head.y == (gridSize - i - 1) && head.x == j) {
                oss << 'H';
            } else if (tail.y == (gridSize - i - 1) && tail.x == j) {
                oss << 'T';
            } else {
                oss << '.';
            }
        }
        oss << '\n';
    }
}

void setPoint(std::vector<uint8_t> &frame, Position const &p, std::pair<int64_t, int64_t> xRange,
              std::pair<int64_t, int64_t> yRange, std::array<uint8_t, 3> const &color) {
    const int64_t offset = (p.y - yRange.first) * (xRange.second - xRange.first + 1) + (p.x - xRange.first);
    frame.at(offset * 4 + 0) = color.at(0);
    frame.at(offset * 4 + 1) = color.at(1);
    frame.at(offset * 4 + 2) = color.at(2);
}

template <typename PositionIterator>
void drawFrame(GifWriter &vis, std::vector<uint8_t> &frame, std::pair<int64_t, int64_t> xRange,
               std::pair<int64_t, int64_t> yRange, std::unordered_set<Position> const &visited, Position const &head,
               PositionIterator const& tails) {

    std::fill(std::begin(frame), std::end(frame), 0);

    for (auto const &v : visited)
        setPoint(frame, v, xRange, yRange, {0, 64, 0});
    for (auto const &tail : tails)
        setPoint(frame, tail, xRange, yRange, {0, 255, 0});
    setPoint(frame, head, xRange, yRange, {255, 0, 0});

    GifWriteFrame(&vis, frame.data(), (xRange.second - xRange.first + 1), (yRange.second - yRange.first + 1), 1);
}

int main() {

    const auto lines = util::readLines(INPUT_FILE_PATH);
    std::vector<Move> moves;
    for (auto const &line : lines) {
        const auto parts = util::split(line, ' ');
        moves.push_back({char2Direction[parts.at(0).at(0)], static_cast<uint32_t>(std::stoul(parts.at(1)))});
    }

    // part 1 -- simulate movements and return number of unique spaces tail covered
    Position head{0, 0}, tail{0, 0};
    std::unordered_set<Position> visited, headPositions({head});
    visited.insert(tail);

    for (auto const &move : moves) {
        for (uint32_t i = 0; i < move.amt; i += 1) {
            moveHead(head, move.dir);
            updateTail(head, tail);
            visited.insert(tail);

            headPositions.insert(head); // for visualization
        }
    }
    std::cout << visited.size() << "\n";

    // part 2 -- simulate movements of rope with more knots and return number of unique spaces last knot covered
    constexpr size_t NUM_KNOTS = 9;
    head = {0, 0};
    std::array<Position, NUM_KNOTS> tails({0, 0});
    visited.clear();
    visited.insert(tails.back());

    for (auto const &move : moves) {
        for (uint32_t i = 0; i < move.amt; i += 1) {
            /* first pair */
            moveHead(head, move.dir);
            updateTail(head, tails.front());

            /* rest of knots */
            for (size_t j = 1; j < tails.size(); j += 1) {
                updateTail(tails.at(j - 1), tails.at(j));
            }

            visited.insert(tails.back());
        }
    }
    std::cout << visited.size() << "\n";

    /* VISUALIZATIONS -- problem is solved sparsely, but dense info is needed for visualization. so repeat... */
    if constexpr (VISUALIZATION) {
        const auto [minX, maxX] = std::minmax_element(std::cbegin(headPositions), std::cend(headPositions),
                                                      [](auto const &a, auto const &b) { return a.x < b.x; });
        const auto [minY, maxY] = std::minmax_element(std::cbegin(headPositions), std::cend(headPositions),
                                                      [](auto const &a, auto const &b) { return a.y < b.y; });

        const auto xRange = std::pair<int64_t, int64_t>(minX->x, maxX->x);
        const auto yRange = std::pair<int64_t, int64_t>(minY->y, maxY->y);
        const uint32_t width = xRange.second - xRange.first + 1;
        const uint32_t height = yRange.second - yRange.first + 1;

        GifWriter vis;
        std::vector<uint8_t> frame(width * height * 4, 0);
        GifBegin(&vis, "figs/dec09/part1.gif", width, height, 1);

        /* VIS PART 1 */
        head = {0, 0}, tail = {0, 0};
        visited.clear();
        visited.insert(tail);
        for (auto const &move : moves) {
            for (uint32_t i = 0; i < move.amt; i += 1) {
                moveHead(head, move.dir);
                updateTail(head, tail);
                visited.insert(tail);

                drawFrame(vis, frame, xRange, yRange, visited, head, std::initializer_list<Position>{tail});
            }
        }
        for (uint32_t i = 0; i < FINAL_FRAME_REPEAT; i += 1)
            drawFrame(vis, frame, xRange, yRange, visited, head, std::initializer_list<Position>{tail});
        GifEnd(&vis);

        /* VIS PART 2*/
        GifBegin(&vis, "figs/dec09/part2.gif", width, height, 1);
        head = {0, 0};
        std::fill(std::begin(tails), std::end(tails), Position{0,0});
        visited.clear();
        visited.insert(tails.back());
        for (auto const &move : moves) {
            for (uint32_t i = 0; i < move.amt; i += 1) {
                moveHead(head, move.dir);
                updateTail(head, tails.front());
                for (size_t j = 1; j < tails.size(); j += 1) {
                    updateTail(tails.at(j - 1), tails.at(j));
                }
                visited.insert(tails.back());

                drawFrame(vis, frame, xRange, yRange, visited, head, tails);
            }
        }
        for (uint32_t i = 0; i < FINAL_FRAME_REPEAT; i += 1)
            drawFrame(vis, frame, xRange, yRange, visited, head, tails);
        GifEnd(&vis);
    }
}