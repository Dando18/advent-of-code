/**
 * @file dec15.cc
 * @author Daniel Nichols
 * @date December 2022
 * @brief AOC 2022 Day 15 https://adventofcode.com/2022/day/15
 *
 */
// stl includes
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// local includes
#include "utilities.hpp"

constexpr auto INPUT_FILE_PATH = "/home/daniel/dev/personal/advent-of-code/2022/inputs/dec15.txt";

template <typename T>
using Row = std::vector<T>;
template <typename T>
using Grid = std::vector<Row<T>>;
typedef util::Point2D<int64_t> Point;

int64_t manhattan(Point const &a, Point const &b) { return std::abs(a.x - b.x) + std::abs(a.y - b.y); }

struct Sensor {
    Sensor(Point const &position, Point const &beacon)
        : position_(position), beacon_(beacon), distanceToBeacon_(manhattan(position, beacon)) {}

    Point position_, beacon_;
    int64_t distanceToBeacon_;
};

Point parsePoint(std::string const &str) {
    const auto parts = util::split(str, ", ");
    const auto [xStr, yStr] = std::tuple(parts.at(0), parts.at(1));
    return {std::stoll(util::split(xStr, '=').at(1)), std::stoll(util::split(yStr, '=').at(1))};
}

std::pair<Point, Sensor> parseSensor(std::string const &str) {
    const auto parts = util::split(str, ':');
    const auto [lhs, rhs] = std::tuple(parts.at(0), parts.at(1));

    const auto pos = parsePoint(lhs.substr(lhs.find('x')));
    const auto beacon = parsePoint(rhs.substr(rhs.find('x')));

    return {pos, {pos, beacon}};
}

void markNonBeacons(Grid<char> &grid, Grid<bool> &visited, const int64_t maxDist,
                    std::pair<int64_t, int64_t> const &xRange, std::pair<int64_t, int64_t> const &yRange,
                    Point const &src, Point const &pos) {

    if (manhattan(src, pos) > maxDist) return;
    if (pos.x < xRange.first || pos.x > xRange.second || pos.y < yRange.first || pos.y > yRange.second) return;
    if (visited.at(pos.y - yRange.first).at(pos.x - xRange.first)) return;

    visited.at(pos.y - yRange.first).at(pos.x - xRange.first) = true;
    char &curCell = grid.at(pos.y - yRange.first).at(pos.x - xRange.first);
    if (curCell == '.') curCell = '#';

    const std::array<Point, 8> offsets{{{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};
    for (auto const &off : offsets)
        markNonBeacons(grid, visited, maxDist, xRange, yRange, src, {pos.x + off.x, pos.y + off.y});
}

void printGrid(Grid<char> const &grid, std::ostream &oss, int64_t yStart = 0) {
    int64_t rowCounter = yStart;
    for (auto const &row : grid) {
        oss << rowCounter++ << '\t';
        for (auto const &c : row) {
            oss << c;
        }
        oss << '\n';
    }
}

int main() {

    const auto lines = util::readLines(INPUT_FILE_PATH);
    std::unordered_map<Point, Sensor> sensors;
    std::unordered_set<Point> beacons;
    constexpr int64_t INT_MIN = std::numeric_limits<int64_t>::min();
    constexpr int64_t INT_MAX = std::numeric_limits<int64_t>::max();
    std::pair<int64_t, int64_t> xRange{INT_MAX, INT_MIN}, yRange{INT_MAX, INT_MIN};
    for (auto const &line : lines) {
        const auto [pos, sensor] = parseSensor(line);
        sensors.insert({pos, sensor});
        beacons.insert(sensor.beacon_);
        xRange.first = std::min(std::min(xRange.first, pos.x - sensor.distanceToBeacon_), sensor.beacon_.x);
        xRange.second = std::max(std::max(xRange.second, pos.x + sensor.distanceToBeacon_), sensor.beacon_.x);
        yRange.first = std::min(std::min(yRange.first, pos.y - sensor.distanceToBeacon_), sensor.beacon_.y);
        yRange.second = std::max(std::max(yRange.second, pos.y + sensor.distanceToBeacon_), sensor.beacon_.y);
    }

    const uint32_t width = xRange.second - xRange.first + 1;
    const uint32_t height = yRange.second - yRange.first + 1;
    std::cout << width << " x " << height << "\n";
    // Grid<char> grid(height, Row<char>(width, '.'));
    // Grid<bool> visited(height, Row<bool>(width));
    // for (auto const &[pos, sensor] : sensors) {
    //     grid.at(pos.y - yRange.first).at(pos.x - xRange.first) = 'S';
    //     grid.at(sensor.beacon_.y - yRange.first).at(sensor.beacon_.x - xRange.first) = 'B';
    //
    //     std::for_each(visited.begin(), visited.end(), [](auto &r){ std::fill(r.begin(), r.end(), false); });
    //     markNonBeacons(grid, visited, sensor.distanceToBeacon_, xRange, yRange, pos, pos);
    // }
    // printGrid(grid, std::cout, yRange.first);

    // part 1 --
    const uint32_t targetY = 2000000;
    uint32_t count = 0;
    for (int64_t x = xRange.first; x <= xRange.second; x += 1) {
        if (beacons.contains({x, targetY}) || sensors.contains({x, targetY})) continue;

        const bool inRange = std::any_of(std::begin(sensors), std::end(sensors), [x, targetY](auto const &it) {
            auto const &[pos, sensor] = it;
            return manhattan(pos, {x, targetY}) <= sensor.distanceToBeacon_;
        });
        count += (inRange) ? 1 : 0;
    }
    std::cout << count << "\n";

    // part 2 -- search for only space in (0,0) to (4000000,4000000) that can be a beacon
    constexpr int64_t SEARCH_MIN = 0, SEARCH_MAX = 4000000;
    Point found{0, 0};
    std::unordered_set<Point> toSearch;
    /*  the actual search space is too large, so reduce the search space to the intersections of the boundaries of the
        sensor regions. */
    for (auto const &[pos1, sensor1] : sensors) {
        const auto bound1 = pos1.y - pos1.x + sensor1.distanceToBeacon_ + 1;
        const auto bound2 = pos1.y - pos1.x - sensor1.distanceToBeacon_ - 1;
        for (auto const &[pos2, sensor2] : sensors) {
            toSearch.insert({bound1, pos2.y + pos2.x + sensor2.distanceToBeacon_ + 1});
            toSearch.insert({bound2, pos2.y + pos2.x - sensor2.distanceToBeacon_ - 1});
        }
    }
    for (auto const &line : toSearch) {
        Point intersection{(line.y - line.x) / 2, (line.y + line.x) / 2};
        if (intersection.x > SEARCH_MIN && intersection.x < SEARCH_MAX && intersection.y > SEARCH_MIN &&
            intersection.y < SEARCH_MAX) {
            const bool outOfRange = std::all_of(sensors.begin(), sensors.end(), [intersection](auto const &s) {
                return manhattan(intersection, s.first) > s.second.distanceToBeacon_;
            });

            if (outOfRange) {
                found = intersection;
                break;
            }
        }
    }
    std::cout << (found.x * 4000000 + found.y) << '\n';
}