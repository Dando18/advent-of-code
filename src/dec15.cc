#include <cstdio>
#include <fstream>
#include <string>
#include <cstdint>
#include <sstream>
#include <map>
#include <vector>

constexpr size_t GAME_LIMIT = 30000001;
constexpr size_t TARGET1 = 2020;
constexpr size_t TARGET2 = 30000000;

int main() {

    std::ifstream inFile("./inputs/dec15.txt");
    std::string line;

    std::getline(inFile, line);
    std::istringstream ss(line);

    uint32_t num = 0, counter = 0, lastNumSpoken = 0;
    std::string numStr;
    //std::array<uint32_t, GAME_LIMIT+1> vals {0};
    //std::vector<uint32_t> vals(GAME_LIMIT+1, 0);
    std::map<uint32_t, uint32_t> table;
    while (std::getline(ss, numStr, ',')) {
        num = std::stoi(numStr);
        table[num] = counter;
        lastNumSpoken = num;
        counter += 1;
    }
    table.erase(lastNumSpoken);

    for (size_t i = counter; i < GAME_LIMIT; i++) {
        /* find last of lastNumSpoken */
        auto lastSpokenLocation = table.find(lastNumSpoken);
        
        /* say the next number */
        if (lastSpokenLocation == table.end()) {
            /* not found */
            table[lastNumSpoken] = i-1;
            lastNumSpoken = 0;
        } else {
            /* found -- say difference */
            uint32_t loc = lastSpokenLocation->second;
            table[lastNumSpoken] = i-1;
            lastNumSpoken = i - 1 - loc;
        }

        if (i == TARGET1-1) {
            printf("%lu num = %u\n", TARGET1, lastNumSpoken);
        } else if (i == TARGET2-1) {
            printf("%lu num = %u\n", TARGET2, lastNumSpoken);
        }
    }
    
}