/**
 * @file dec12.cc
 * @author Daniel Nichols
 * @date December 2022
 * @brief AOC 2022 Day 12 https://adventofcode.com/2022/day/12
 *
 */
// stl includes
#include <deque>
#include <iostream>
#include <limits>
#include <string>

// stl includes
#include "gif.h"

// local includes
#include "utilities.hpp"

constexpr auto INPUT_FILE_PATH = "/home/daniel/dev/personal/advent-of-code/2022/inputs/dec12.txt";
constexpr bool VISUALIZATION = true;
constexpr uint32_t FINAL_FRAME_REPEAT = 200;

typedef char Elevation;
template <typename T> using Grid = std::vector<std::vector<T>>;
typedef std::pair<uint32_t, uint32_t> Vertex;

std::vector<Vertex> getNeighbors(Vertex const &v, uint32_t rows, uint32_t cols) {
    std::vector<Vertex> neighbors;
    if (v.first != 0) neighbors.push_back({v.first - 1, v.second});
    if (v.first != rows - 1) neighbors.push_back({v.first + 1, v.second});
    if (v.second != 0) neighbors.push_back({v.first, v.second - 1});
    if (v.second != cols - 1) neighbors.push_back({v.first, v.second + 1});
    return neighbors;
}

bool canTravelTo(Elevation const &src, Elevation const &dst) { return (src + 1) >= dst; }

void writeHeightsToFrame(Grid<Elevation> const &grid, std::vector<uint8_t> &frame) {
    size_t iter = 0;
    for (auto const &row : grid) {
        for (auto const &h : row) {
            const uint8_t scale = static_cast<uint8_t>(static_cast<float>(h) / ('z' - 'a') * 255.0f);
            frame.at(iter++) = scale;
            frame.at(iter++) = scale;
            frame.at(iter++) = scale;
            frame.at(iter++) = 255;
        }
    }
}

inline uint8_t average(uint8_t a, uint8_t b) {
    return static_cast<uint8_t>((static_cast<uint32_t>(a) + static_cast<uint32_t>(b)) / 2);
}

inline uint8_t interp(uint8_t a, uint8_t b, float weightA = 1.0f) {
    return static_cast<uint8_t>(weightA * static_cast<float>(a) + (1.0f - weightA) * static_cast<float>(b));
}

void writeVertexToFrame(std::vector<uint8_t> &frame, Vertex const &v, uint32_t width, std::array<uint8_t, 3> color,
                        bool blend = false, float blendAmt = 1.0f) {
    const uint32_t offset = 4 * (v.first * width + v.second);

    if (blend) {
        color = {interp(color.at(0), frame.at(offset + 0), blendAmt),
                 interp(color.at(1), frame.at(offset + 1), blendAmt),
                 interp(color.at(2), frame.at(offset + 2), blendAmt)};
    }

    frame.at(offset + 0) = color.at(0);
    frame.at(offset + 1) = color.at(1);
    frame.at(offset + 2) = color.at(2);
    frame.at(offset + 3) = 255;
}

void writePath(std::vector<uint8_t> &frame, uint32_t width, Grid<Vertex> const &path, Vertex const &src,
               Vertex const &dst, std::array<uint8_t, 3> color) {

    Vertex v = src;
    while (v != dst) {
        writeVertexToFrame(frame, v, width, color);
        v = path.at(v.first).at(v.second);
    }
}

void highlightVisited(std::vector<uint8_t> &frame, Grid<bool> const &visited, Vertex const &src,
                      std::array<uint8_t, 3> color) {
    for (size_t i = 0; i < visited.size(); i += 1) {
        for (size_t j = 0; j < visited.at(i).size(); j += 1) {
            if (src.first == i && src.second == j) continue;
            if (visited.at(i).at(j)) writeVertexToFrame(frame, {i, j}, visited.front().size(), color, true, 0.15f);
        }
    }
}

void writeFrame(GifWriter &writer, std::vector<uint8_t> const &frame, uint32_t width, uint32_t height) {
    GifWriteFrame(&writer, frame.data(), width, height, 1);
}

Vertex bfs(Grid<Elevation> const &grid, Vertex src, Vertex dst, Grid<uint32_t> &distances, Grid<Vertex> &predecessors,
         std::optional<GifWriter> vis = std::nullopt) {
    const uint32_t ROWS = grid.size(), COLS = grid.front().size();

    std::optional<std::vector<uint8_t>> frame;
    if (vis) {
        frame = std::vector<uint8_t>(ROWS * COLS * 4, 0);
        writeHeightsToFrame(grid, *frame);
        writeVertexToFrame(*frame, src, COLS, {0, 0, 255});
        writeVertexToFrame(*frame, dst, COLS, {76, 153, 0});
    }

    std::deque<Vertex> queue;
    Grid<bool> visited(ROWS, std::vector<bool>(COLS, false));

    visited.at(src.first).at(src.second) = true;
    distances.at(src.first).at(src.second) = 0;
    queue.push_back(src);

    Vertex endpoint = src;
    while (!queue.empty()) {
        const auto v = queue.front();
        queue.pop_front();
        endpoint = v;
        const auto curHeight = grid.at(v.first).at(v.second);

        if (vis) {
            writeHeightsToFrame(grid, *frame);
            writeVertexToFrame(*frame, src, COLS, {0, 0, 255});
            writeVertexToFrame(*frame, dst, COLS, {76, 153, 0});
            highlightVisited(*frame, visited, src, {255, 0, 0});
            writePath(*frame, COLS, predecessors, v, src, {255, 0, 0});
            writeFrame(*vis, *frame, COLS, ROWS);
        }

        for (auto const &neighbor : getNeighbors(v, ROWS, COLS)) {
            if (canTravelTo(curHeight, grid.at(neighbor.first).at(neighbor.second)) &&
                !visited.at(neighbor.first).at(neighbor.second)) {

                visited.at(neighbor.first).at(neighbor.second) = true;
                distances.at(neighbor.first).at(neighbor.second) = distances.at(v.first).at(v.second) + 1;
                predecessors.at(neighbor.first).at(neighbor.second) = v;
                queue.push_back(neighbor);

                if (neighbor == dst) {
                    queue.clear();
                    endpoint = neighbor;
                    break;
                }
            }
        }
    }

    if (vis)
        for (uint32_t i = 0; i < FINAL_FRAME_REPEAT; i += 1)
            writeFrame(*vis, *frame, COLS, ROWS);

    return endpoint;
}

int main() {

    const auto lines = util::readLines(INPUT_FILE_PATH);
    const uint32_t ROWS = lines.size(), COLS = lines.front().size();
    Grid<Elevation> grid(ROWS, std::vector<Elevation>(COLS));
    Vertex src, dst;
    for (size_t i = 0; i < ROWS; i += 1) {
        for (size_t j = 0; j < COLS; j += 1) {
            const char c = lines.at(i).at(j);
            Elevation val = static_cast<Elevation>(c - 'a');
            if (c == 'S') {
                val = 'a' - 'a';
                src = {i, j};
            } else if (c == 'E') {
                val = 'z' - 'a';
                dst = {i, j};
            }

            grid.at(i).at(j) = val;
        }
    }

    std::optional<GifWriter> vis;
    if constexpr (VISUALIZATION) {
        vis = GifWriter{};
        GifBegin(&(*vis), "figs/dec12/part1.gif", COLS, ROWS, 1);
    }

    // part 1 -- find shortest path from S to E
    const uint32_t intMax = std::numeric_limits<uint32_t>::max();
    Grid<uint32_t> distances(ROWS, std::vector<uint32_t>(COLS, intMax));
    Grid<Vertex> predecessors(ROWS, std::vector<Vertex>(COLS, {intMax, intMax}));
    bfs(grid, src, dst, distances, predecessors, vis);
    std::cout << distances.at(dst.first).at(dst.second) << "\n";

    if constexpr (VISUALIZATION) {
        GifEnd(&(*vis));
    }

    // part 2 -- find shortest path from any 'a' to E
    /* === WARNING: very naive solution lol -- probably better to switch to Floyd-Warshall === */
    std::optional<std::vector<uint8_t>> frame;
    if constexpr (VISUALIZATION) {
        GifBegin(&(*vis), "figs/dec12/part2.gif", COLS, ROWS, 1);
        frame = std::vector<uint8_t>(ROWS*COLS*4, 0);
    }
    uint32_t minDistance = std::numeric_limits<uint32_t>::max();
    for (size_t i = 0; i < ROWS; i += 1) {
        for (size_t j = 0; j < COLS; j += 1) {
            if (grid.at(i).at(j) != 0) continue;

            std::for_each(std::begin(distances), std::end(distances),
                          [intMax](auto &row) { std::fill(std::begin(row), std::end(row), intMax); });

            src = {i, j};
            Vertex end = bfs(grid, src, dst, distances, predecessors);
            minDistance = std::min(minDistance, distances.at(dst.first).at(dst.second));

            if (vis) {
                writeHeightsToFrame(grid, *frame);
                writeVertexToFrame(*frame, src, COLS, {0, 0, 255});
                writeVertexToFrame(*frame, dst, COLS, {76, 153, 0});
                writePath(*frame, COLS, predecessors, end, src, {255, 0, 0});
                writeFrame(*vis, *frame, COLS, ROWS);
            }
        }
    }
    std::cout << minDistance << "\n";

    if constexpr (VISUALIZATION) {
        GifEnd(&(*vis));
    }
}