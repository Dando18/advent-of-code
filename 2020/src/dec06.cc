#include <fstream>
#include <cstdio>
#include <string>
#include <array>

int main() {

    std::ifstream inFile("./inputs/dec6.txt");
    std::string line;
    std::array<bool, 26> mask {false};
    std::array<size_t, 26> counts {0};

    size_t totalPart1 = 0, totalPart2 = 0, groupCount = 0;
    while (getline(inFile, line)) {
        if (line.find_first_not_of(" \t\n\v\f\r") != std::string::npos) {
            /* not a blank line */
            for (char c : line) {
                mask.at(c - 'a') = true;
                counts.at(c - 'a') += 1;
            }
            groupCount += 1;
        } else {
            /* new group */
            totalPart1 += std::count(std::begin(mask), std::end(mask), true);
            std::fill(std::begin(mask), std::end(mask), false);

            totalPart2 += std::count(std::begin(counts), std::end(counts), groupCount);
            std::fill(std::begin(counts), std::end(counts), 0);
            groupCount = 0;
        }
    }
    totalPart1 += std::count(std::begin(mask), std::end(mask), true);
    totalPart2 += std::count(std::begin(counts), std::end(counts), groupCount);

    printf("Sum of any counts: %lu\n", totalPart1);
    printf("Sum of all counts: %lu\n", totalPart2);

    return 0;
}
