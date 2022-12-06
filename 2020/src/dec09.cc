#include <cstdio>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>


constexpr size_t PREAMBLE_LEN = 25;

bool isValid(size_t begin, size_t end, size_t value, std::vector<size_t> const& values) {
    for (size_t i = begin; i <= end; i++) {
        for (size_t j = i+1; j <= end; j++) {
            if (values.at(i) + values.at(j) == value) return true;
        }
    }
    return false;
}

int main() {

    std::ifstream inFile("inputs/dec9.txt");
    std::string line;
    std::vector<size_t> values;

    while (std::getline(inFile, line)) {
        values.push_back(std::stoul(line));
    }

    size_t solution = 0;
    for (size_t i = PREAMBLE_LEN; i < values.size(); i++) {
        size_t begin = i-PREAMBLE_LEN;
        size_t end = i-1;
        if (!isValid(begin, end, values.at(i), values)) {
            solution = values.at(i);
        }
    }
    printf("%lu does not fit\n", solution);

    size_t solution2 = 0;
    for (size_t i = 0; i < values.size(); i++) {
        size_t sum = 0;
        size_t j = i;
        size_t min = ~0UL;
        size_t max = 0;

        while (sum < solution) {
            size_t curVal = values.at(j);
            sum += values.at(j);
            min = (curVal < min) ? curVal : min;
            max = (curVal > max) ? curVal : max;

            j += 1;
        }
        if (sum == solution) {
            solution2 = max + min;
            break;
        }

    }
    printf("sum of min and max in range = %lu\n", solution2);

}