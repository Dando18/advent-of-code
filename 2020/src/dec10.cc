#include <cstdio>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <cstdint>


int64_t getCount(size_t idx, std::vector<int> const& adapters, std::vector<int64_t> &cache) {
    if (idx < cache.size() && cache.at(idx) != -1) return cache.at(idx);

    int64_t sum = 0;
    for (size_t i = idx+1; i < idx+4; i++) {
        if (std::find(std::begin(adapters), std::end(adapters), i) != std::end(adapters)) {
            sum += getCount(i, adapters, cache);
        }
    }
    cache.at(idx) = sum;
    return sum;
}

int main() {

    std::ifstream inFile("inputs/dec10.txt");
    std::string line;
    std::vector<int> adapters;

    while (std::getline(inFile, line)) {
        adapters.push_back(std::stoi(line));
    }

    int deviceJoltage = (*std::max_element(std::begin(adapters), std::end(adapters))) + 3;
    //adapters.push_back(deviceJoltage);

    std::vector<int> diffs(adapters.size());
    std::sort(std::begin(adapters), std::end(adapters));
    std::adjacent_difference(std::begin(adapters), std::end(adapters), std::begin(diffs));
    int solution1 = std::count(std::begin(diffs), std::end(diffs), 1) * (std::count(std::begin(diffs), std::end(diffs), 3)+1);
    printf("solution1 = %d\n", solution1);

    std::vector<int64_t> cache(deviceJoltage, -1);
    cache.at(adapters.back()) = 1;
    int64_t solution2 = getCount(0, adapters, cache);
    printf("solution2 = %ld\n", solution2);
}