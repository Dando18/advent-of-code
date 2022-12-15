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

    // part 1 --find number of places on line y=2000000 where a beacon cannot be
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