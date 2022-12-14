/**
 * @file dec14.cc
 * @author Daniel Nichols
 * @date December 2022
 * @brief AOC 2022 Day 14 https://adventofcode.com/2022/day/14
 *
 */
// stl includes
#include <iostream>
#include <unordered_map>
#include <map>
#include <optional>
#include <string>
#include <vector>

// tpl includes
#include "gif.h"

// local includes
#include "utilities.hpp"

constexpr auto INPUT_FILE_PATH = "/home/daniel/dev/personal/advent-of-code/2022/inputs/dec14.txt";

template <typename DType_> using Grid = std::vector<std::vector<DType_>>;
typedef char Cell;
typedef util::Point2D<uint32_t> Point;
struct RGBA { uint8_t r, g, b, a; };

constexpr Cell SAND_SYMBOL = 'o';
constexpr Cell ROCK_SYMBOL = '#';
constexpr Cell SOURCE_SYMBOL = '+';
constexpr Cell EMPTY_SYMBOL = '.';
const std::unordered_map<Cell, RGBA> colorMap {{SAND_SYMBOL, {204,204,0,255}}, {ROCK_SYMBOL, {0,0,0,255}}, 
    {SOURCE_SYMBOL, {255,0,0,255}}, {EMPTY_SYMBOL, {255,255,255,255}}};

void drawLine(Grid<Cell> &grid, Point const &start, Point const &end) {
    Point iter = start;

    while (iter != end) {
        grid.at(iter.y).at(iter.x) = ROCK_SYMBOL;                   // draw
        iter.y += (iter.y < end.y) ? 1 : (iter.y > end.y) ? -1 : 0; // move
        iter.x += (iter.x < end.x) ? 1 : (iter.x > end.x) ? -1 : 0; // move
    }
    grid.at(end.y).at(end.x) = ROCK_SYMBOL;
}

void drawRock(Grid<Cell> &grid, std::vector<Point> const &rock) {
    for (size_t i = 0; i < rock.size() - 1; i += 1) {
        drawLine(grid, rock.at(i), rock.at(i + 1));
    }
}

Grid<Cell> parseInput(std::vector<std::string> const &lines, Point source, uint32_t buffer = 2) {
    Grid<Point> rocks;

    uint32_t maxX = source.x, maxY = source.y;
    for (auto const &line : lines) {
        const auto pointStrs = util::split(line, "->");
        std::vector<Point> points;
        for (auto const &pointStr : pointStrs) {
            const auto vals = util::split(pointStr, ',');
            const Point p{static_cast<uint32_t>(std::stoul(vals.at(0))), static_cast<uint32_t>(std::stoul(vals.at(1)))};
            points.push_back(p);

            maxX = std::max(maxX, p.x);
            maxY = std::max(maxY, p.y);
        }
        rocks.push_back(points);
    }

    Grid<Cell> grid(maxY + buffer, std::vector<Cell>(maxX + buffer, EMPTY_SYMBOL));

    for (auto const &rock : rocks) {
        drawRock(grid, rock);
    }
    grid.at(source.y).at(source.x) = SOURCE_SYMBOL;
    return grid;
}

void printGrid(Grid<Cell> const &grid, std::ostream &oss, uint32_t minX = 494, uint32_t maxX = 503) {
    uint32_t rowCounter = 0;
    for (auto const &row : grid) {
        oss << rowCounter << "\t";
        rowCounter += 1;

        for (size_t j = minX; j <= maxX; j++)
            oss << row.at(j);
        oss << '\n';
    }
}

void writeFrame(Grid<Cell> const &grid, GifWriter &gif, std::vector<uint8_t> &buffer, int delay = 1) {

    uint32_t iter = 0;
    for (auto const& row : grid) {
        for (auto const& c : row) {
            const auto pixel = colorMap.at(c);
            buffer.at(iter++) = pixel.r;
            buffer.at(iter++) = pixel.g;
            buffer.at(iter++) = pixel.b;
            buffer.at(iter++) = pixel.a;
        }
    }

    GifWriteFrame(&gif, buffer.data(), grid.front().size(), grid.size(), delay);
}

std::optional<Point> scanDown(Grid<Cell> const &grid, Point const &start) {
    auto row = start.y;
    while ((row < grid.size()) &&
           ((grid.at(row).at(start.x) == EMPTY_SYMBOL) || (grid.at(row).at(start.x) == SOURCE_SYMBOL))) {

        row += 1;
    }
    if (row == grid.size()) return std::nullopt;

    Point p{start.x, row - 1};
    if (p.x != 0 && grid.at(p.y + 1).at(p.x - 1) == EMPTY_SYMBOL) {
        p.y += 1;
        p.x -= 1;
    } else if (p.x != grid.front().size() - 1 && grid.at(p.y + 1).at(p.x + 1) == EMPTY_SYMBOL) {
        p.y += 1;
        p.x += 1;
    }
    return p;
}

std::optional<Point> dropSand(Grid<Cell> const &grid, Point source) {
    auto sand = source;
    auto dropPoint = scanDown(grid, source);

    while (dropPoint && dropPoint != sand) {
        sand = *dropPoint;
        dropPoint = scanDown(grid, sand);
    }

    return dropPoint;
}

uint32_t countSand(Grid<Cell> grid, Point source, std::optional<GifWriter> vis = std::nullopt) {
    uint32_t count = 0;

    std::optional<std::vector<uint8_t>> visBuffer;
    if (vis) visBuffer = std::vector<uint8_t>(grid.size()*grid.front().size()*4, 255);

    auto sand = dropSand(grid, source);
    while (sand && sand != source) {
        grid.at(sand->y).at(sand->x) = SAND_SYMBOL;
        count += 1;

        if (vis) writeFrame(grid, *vis, *visBuffer);

        sand = dropSand(grid, source);
    }

    return count;
}

int main() {

    const auto lines = util::readLines(INPUT_FILE_PATH);
    constexpr Point SOURCE{500, 0};
    auto grid = parseInput(lines, SOURCE);

    // part 1 -- drop sand and see how long it is till things fall into abyss
    GifWriter part1Visualization;
    GifBegin(&part1Visualization, "figs/dec14/part1.gif", grid.front().size(), grid.size(), 1);
    uint32_t total = countSand(grid, SOURCE, part1Visualization);
    GifEnd(&part1Visualization);
    std::cout << total << '\n';

    // part 2 -- drop sand until source is covered
    const uint32_t PAD = 250;
    std::for_each(std::begin(grid), std::end(grid), [](auto &r) { r.insert(r.end(), PAD, EMPTY_SYMBOL); });
    grid.push_back(std::vector<Cell>(grid.front().size(), ROCK_SYMBOL));
    GifWriter part2Visualization;
    GifBegin(&part2Visualization, "figs/dec14/part2.gif", grid.front().size(), grid.size(), 1);
    total = countSand(grid, SOURCE, part2Visualization) + 1;
    GifEnd(&part2Visualization);
    std::cout << total << '\n';
}