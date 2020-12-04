#include <fstream>
#include <vector>
#include <numeric>
#include <cstdint>
#include <string>
#include <array>

constexpr char TREE = '#';
constexpr char EMPTY = '.';

size_t countTotalTreesOnPath(std::vector<std::string> const& grid, size_t startColumn, size_t startRow, size_t horizontalStep, size_t verticalStep) {
    size_t currentColumn = startColumn, currentRow = startRow;
    const size_t GRID_ROW_SIZE = grid.at(0).size(); // artificially extended

    size_t totalTrees = 0;
    while (currentRow < grid.size()) {

        char curBlock = grid.at(currentRow).at(currentColumn % GRID_ROW_SIZE);

        totalTrees += (curBlock == TREE);

        currentColumn += horizontalStep;
        currentRow += verticalStep;
    }

    return totalTrees;
}

int main(int argc, char **argv) {

    std::ifstream inFile("./inputs/dec3.txt");
    std::vector<std::string> grid;

    std::string line;
    while (inFile >> line) {
        grid.push_back(line);
    }

    
    size_t totalTreesPart1 = countTotalTreesOnPath(grid, 0, 0, 3, 1);
    printf("Total Trees: %lu\n", totalTreesPart1);

    /* lol this is kinda ugly but C++ do be like that sometimes */
    auto possibleSteps = {std::make_pair<size_t, size_t>(1, 1),
                          std::make_pair<size_t, size_t>(3, 1),
                          std::make_pair<size_t, size_t>(5, 1),
                          std::make_pair<size_t, size_t>(7, 1),
                          std::make_pair<size_t, size_t>(1, 2)};
    std::array<uint64_t, 5> solutions;

    size_t i = 0;
    for (auto const& step : possibleSteps) {
        size_t result = countTotalTreesOnPath(grid, 0, 0, step.first, step.second);
        solutions.at(i++) = static_cast<uint64_t>(result);
    }

    uint64_t productPart2 = std::accumulate(solutions.begin(), solutions.end(), 1ULL, std::multiplies<uint64_t>());
    printf("Product for Part 2: %lu\n", productPart2);

    return 0;
}