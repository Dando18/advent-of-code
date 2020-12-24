#include <cstdio>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>


constexpr char FLOOR = '.';
constexpr char EMPTY = 'L';
constexpr char OCCUPIED = '#';
constexpr char BORDER = 'B';

char getCell(int i, int j, std::vector<std::string> const& table) {
    if (i < 0 || j < 0 || i > static_cast<int>(table.size())-1 || j > static_cast<int>(table.at(0).size())-1) {
        return BORDER;
    }
    return table.at(i).at(j);
}

size_t updateBoard(std::vector<std::string> &table) {
    std::vector<std::string> tableCopy (std::begin(table), std::end(table));
    std::vector<std::pair<int, int>> adj {{-1,-1}, {-1,0},{-1,1},
                                         {0,-1},         {0,1},
                                         {1,-1}, {1,0}, {1,1}};

    size_t changeCount = 0;
    for (int i = 0; i < static_cast<int>(table.size()); i++) {
        for (int j = 0; j < static_cast<int>(table.at(i).size()); j++) {
            std::vector<char> neighbors;
            for (auto const& [v, h] : adj) {
                neighbors.push_back(getCell(i+v, j+h, table));
            }

            const char curCell = getCell(i, j, table);
            if (curCell == EMPTY && std::count(std::begin(neighbors), std::end(neighbors), OCCUPIED) == 0) {
                tableCopy.at(i).at(j) = OCCUPIED;
                changeCount += 1;
            } else if (curCell == OCCUPIED && std::count(std::begin(neighbors), std::end(neighbors), OCCUPIED) >= 4) {
                tableCopy.at(i).at(j) = EMPTY;
                changeCount += 1;
            } else {
                tableCopy.at(i).at(j) = curCell;
            }
        }
    }

    std::copy(std::begin(tableCopy), std::end(tableCopy), std::begin(table));
    return changeCount;
}

size_t updateBoardPart2(std::vector<std::string> &table) {
    std::vector<std::string> tableCopy (std::begin(table), std::end(table));
    std::vector<std::pair<int, int>> adj {{-1,-1}, {-1,0},{-1,1},
                                         {0,-1},         {0,1},
                                         {1,-1}, {1,0}, {1,1}};

    size_t changeCount = 0;
    for (int i = 0; i < static_cast<int>(table.size()); i++) {
        for (int j = 0; j < static_cast<int>(table.at(i).size()); j++) {
            std::vector<char> neighbors;
            for (auto const& [v, h] : adj) {
                int mult = 1;
                char cell = getCell(i+v*mult, j+h*mult, table);
                while (cell == FLOOR) {
                    mult += 1;
                    cell = getCell(i+v*mult, j+h*mult, table);
                }
                if (cell == OCCUPIED) neighbors.push_back(OCCUPIED);
            }

            const char curCell = getCell(i, j, table);
            if (curCell == EMPTY && std::count(std::begin(neighbors), std::end(neighbors), OCCUPIED) == 0) {
                tableCopy.at(i).at(j) = OCCUPIED;
                changeCount += 1;
            } else if (curCell == OCCUPIED && std::count(std::begin(neighbors), std::end(neighbors), OCCUPIED) >= 5) {
                tableCopy.at(i).at(j) = EMPTY;
                changeCount += 1;
            } else {
                tableCopy.at(i).at(j) = curCell;
            }
        }
    }

    std::copy(std::begin(tableCopy), std::end(tableCopy), std::begin(table));
    return changeCount;
}


int main() {

    std::ifstream inFile("./inputs/dec11.txt");
    std::string line;
    std::vector<std::string> table, table2;

    while (std::getline(inFile, line)) {
        table.push_back(line);
        table2.push_back(line);
    }

    size_t numSeatsChanged = 1;
    while (numSeatsChanged > 0) {
        numSeatsChanged = updateBoard(table);
    }

    int numOccupied = 0;
    for (auto const& s : table) {
        numOccupied += std::count(std::begin(s), std::end(s), OCCUPIED);
    }
    printf("num occupied part 1 = %d\n", numOccupied);


    numSeatsChanged = 1;
    while (numSeatsChanged > 0) {
        numSeatsChanged = updateBoardPart2(table2);
    }

    numOccupied = 0;
    for (auto const& s : table2) {
        numOccupied += std::count(std::begin(s), std::end(s), OCCUPIED);
    }
    printf("num occupied part 2 = %d\n", numOccupied);
}