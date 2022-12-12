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

// local includes
#include "utilities.hpp"

constexpr auto INPUT_FILE_PATH = "/home/daniel/dev/personal/advent-of-code/2022/inputs/dec12.txt";

typedef char Elevation;
template<typename T>
using Grid = std::vector<std::vector<T>>;
typedef std::pair<uint32_t, uint32_t> Vertex;

std::vector<Vertex> getNeighbors(Vertex const& v, uint32_t rows, uint32_t cols) {
    std::vector<Vertex> neighbors;
    if (v.first != 0) neighbors.push_back( {v.first-1, v.second} );
    if (v.first != rows-1) neighbors.push_back( {v.first+1, v.second} );
    if (v.second != 0) neighbors.push_back( {v.first, v.second-1} );
    if (v.second != cols-1) neighbors.push_back( {v.first, v.second+1} );
    return neighbors;
}

bool canTravelTo(Elevation const& src, Elevation const& dst) {
    return (src + 1) >= dst;
}

void bfs(Grid<Elevation> const& grid, Vertex src, Vertex dst, Grid<uint32_t> &distances) {
    const uint32_t ROWS = grid.size(), COLS = grid.front().size();

    std::deque<Vertex> queue;
    Grid<bool> visited(ROWS, std::vector<bool>(COLS, false));

    visited.at(src.first).at(src.second) = true;
    distances.at(src.first).at(src.second) = 0;
    queue.push_back(src);

    while (!queue.empty()) {
        const auto v = queue.front();
        queue.pop_front();
        const auto curHeight = grid.at(v.first).at(v.second);

        for (auto const& neighbor : getNeighbors(v, ROWS, COLS)) {
            if (canTravelTo(curHeight, grid.at(neighbor.first).at(neighbor.second)) 
                && !visited.at(neighbor.first).at(neighbor.second)) {

                visited.at(neighbor.first).at(neighbor.second) = true;
                distances.at(neighbor.first).at(neighbor.second) = distances.at(v.first).at(v.second) + 1;
                queue.push_back( neighbor );

                if (neighbor == dst) {
                    queue.clear();
                    break;
                }
            }
        }
    }

}

int main() {

    const auto lines = util::readLines(INPUT_FILE_PATH);
    Grid<Elevation> grid(lines.size(), std::vector<Elevation>(lines.front().size()));
    Vertex src, dst; 
    for (size_t i = 0; i < grid.size(); i += 1) {
        for (size_t j = 0; j < grid.at(i).size(); j += 1) {
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

    // part 1 -- find shortest path from S to E
    Grid<uint32_t> distances(grid.size(), 
        std::vector<uint32_t>(grid.front().size(), std::numeric_limits<uint32_t>::max()));
    bfs(grid, src, dst, distances);
    std::cout << distances.at(dst.first).at(dst.second) << "\n";

    // part 2 -- find shortest path from any 'a' to E
    /* === WARNING: very naive solution lol -- probably better to switch to Floyd-Warshall === */
    uint32_t minDistance = std::numeric_limits<uint32_t>::max();
    for (size_t i = 0; i < grid.size(); i += 1) {
        for (size_t j = 0; j < grid.at(i).size(); j += 1) {
            if (grid.at(i).at(j) != 0) continue;

            std::for_each(std::begin(distances), std::end(distances), [](auto &row) {
                std::fill(std::begin(row), std::end(row), std::numeric_limits<uint32_t>::max());
            });

            src = {i, j};
            bfs(grid, src, dst, distances);
            minDistance = std::min(minDistance, distances.at(dst.first).at(dst.second));
        }
    }
    std::cout << minDistance << "\n";

}