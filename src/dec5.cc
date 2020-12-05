#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>


size_t getSeatId(std::string const& boardingPass) {
    size_t lower = 0, upper = 127;
    for (size_t i = 0; i < 7; i++) {
        if (boardingPass.at(i) == 'F')
            upper -= (upper-lower)/2+1;
        else
            lower += (upper-lower)/2+1;
    }
    size_t row = (boardingPass.at(6) == 'F') ? lower : upper;

    lower = 0;
    upper = 7;
    for (size_t i = 7; i < 10; i++) {
        if (boardingPass.at(i) == 'L')
            upper -= (upper-lower)/2+1;
        else
            lower += (upper-lower)/2+1;
    }
    size_t seat = (boardingPass.at(9) == 'L') ? lower : upper;

    return row*8 + seat;
}

int main() {

    std::ifstream inFile ("./inputs/dec5.txt");
    std::vector<size_t> boardingPasses;
    std::string boardingPass;

    while (inFile >> boardingPass) {
        boardingPasses.push_back(getSeatId(boardingPass));
    }

    const auto [min, max] = std::minmax_element(boardingPasses.begin(), boardingPasses.end());
    size_t maxSeatId = *max;
    size_t minSeatId = *min;
    printf("Max seatId: %lu\n", maxSeatId);

    size_t missingSeatId = (maxSeatId*(maxSeatId+1)/2 - minSeatId*(minSeatId-1)/2) - std::accumulate(boardingPasses.begin(), boardingPasses.end(), 0UL);
    printf("Missing seat id: %lu\n", missingSeatId);

    return 0;
}