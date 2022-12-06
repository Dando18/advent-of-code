#include <cstdio>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <cmath>
#include <cstdint>
#include <limits>


int64_t nearestMultiple(int64_t x, int64_t mult) {
    return mult * std::ceil(static_cast<double>(x+1)/mult);
}

int main() {

    std::ifstream inFile("./inputs/dec13.txt");
    std::string line, token;

    std::getline(inFile, line);
    int64_t earliestPossibleDeparture = std::stoll(line);

    std::getline(inFile, line);
    std::istringstream ss(line);
    std::vector<int64_t> busses;
    while (std::getline(ss, token, ',')) {
        busses.push_back( (token == "x") ? -1 : std::stoi(token) );
    }

    int64_t min = std::numeric_limits<int64_t>::max();
    int64_t minId = -1;
    for (auto const& bus : busses) {
        if (bus == -1) continue;
        int64_t m = nearestMultiple(earliestPossibleDeparture, bus);

        if (m < min) {
            min = m;
            minId = bus;
        }
    }
    int64_t waitingTime = min - earliestPossibleDeparture;
    printf("waiting time times busId = %ld\n", waitingTime*minId);

    int64_t time = 1, step = 1;
    for (size_t i = 0; i < busses.size(); i++) {
        if (busses.at(i) == -1) continue;
        while ((time + i) % busses.at(i) != 0) {
            time += step;
        }
        step *= busses.at(i);
    }
    printf("first time t = %ld\n", time);
}