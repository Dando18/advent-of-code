#include <cstdio>
#include <fstream>
#include <string>
#include <cstdint>
#include <vector>
#include <array>

constexpr size_t ZLIM = 25, YLIM = 25, XLIM = 25, WLIM = 25;
constexpr size_t ZCENTER = ZLIM/2, YCENTER = YLIM/2, XCENTER = XLIM/2, WCENTER = WLIM/2;
constexpr bool ACTIVE = true, INACTIVE = false;
constexpr char ACTIVE_CHAR = '#', INACTIVE_CHAR = '.';
constexpr uint32_t N_ITER = 6;

typedef std::vector<std::vector<bool>> Grid2D;
typedef std::vector<Grid2D> Grid3D;


bool getCell(std::vector<Grid2D> const& grid, int x, int y, int z) {
    if (x < 0 || x >= (int)grid.size() || 
        y < 0 || y >= (int)grid.at(0).size() || 
        z < 0 || z >= (int)grid.at(0).at(0).size())
        return INACTIVE;
    
    return grid.at(x).at(y).at(z);
}

bool getCell4D(std::vector<Grid3D> const& grid, int x, int y, int z, int w) {
    if (x < 0 || x >= (int)grid.size() || 
        y < 0 || y >= (int)grid.at(0).size() || 
        z < 0 || z >= (int)grid.at(0).at(0).size() ||
        w < 0 || w >= (int)grid.at(0).at(0).at(0).size())
        return INACTIVE;
    
    return grid.at(x).at(y).at(z).at(w);
}

void step(std::vector<Grid2D> const& inGrid, std::vector<Grid2D> &outGrid) {

    for (size_t x = 0; x < inGrid.size(); x++) {
        for (size_t y = 0; y < inGrid.at(x).size(); y++) {
            for (size_t z = 0; z < inGrid.at(x).at(y).size(); z++) {

                auto curCell = getCell(inGrid, x, y, z);

                int activeNeighborsCount = 0;
                for (int xOff : {0, 1, -1}) {
                    for (int yOff : {0, 1, -1}) {
                        for (int zOff : {0, 1, -1}) {
                            if (xOff == 0 && yOff == 0 && zOff == 0) continue;

                            auto neighbor = getCell(inGrid, x+xOff, y+yOff, z+zOff);
                            activeNeighborsCount += (neighbor == ACTIVE) ? 1 : 0;
                        }
                    }
                }

                bool output;
                if (curCell == ACTIVE) {
                    if (activeNeighborsCount == 2 || activeNeighborsCount == 3) {
                        output = ACTIVE;
                    } else {
                        output = INACTIVE;
                    }
                } else {
                    if (activeNeighborsCount == 3) {
                        output = ACTIVE;
                    } else {
                        output = INACTIVE;
                    }
                }

                outGrid.at(x).at(y).at(z) = output;

            }
        }
    }
}

/* THIS IS SUPER UGLY BUT IT WORKS LOL */
void step4d(std::vector<Grid3D> const& inGrid, std::vector<Grid3D> &outGrid) {
    for (size_t x = 0; x < inGrid.size(); x++) {
        for (size_t y = 0; y < inGrid.at(x).size(); y++) {
            for (size_t z = 0; z < inGrid.at(x).at(y).size(); z++) {
                for (size_t w = 0; w < inGrid.at(x).at(y).at(z).size(); w++) {

                    auto curCell = getCell4D(inGrid, x, y, z, w);

                    int activeNeighborsCount = 0;
                    for (int xOff : {0, 1, -1}) {
                        for (int yOff : {0, 1, -1}) {
                            for (int zOff : {0, 1, -1}) {
                                for(int wOff : {0, 1, -1}) {
                                    if (xOff == 0 && yOff == 0 && zOff == 0 && wOff == 0) continue;

                                    auto neighbor = getCell4D(inGrid, x+xOff, y+yOff, z+zOff, w+wOff);
                                    activeNeighborsCount += (neighbor == ACTIVE) ? 1 : 0;
                                }
                            }
                        }
                    }

                    bool output;
                    if (curCell == ACTIVE) {
                        if (activeNeighborsCount == 2 || activeNeighborsCount == 3) {
                            output = ACTIVE;
                        } else {
                            output = INACTIVE;
                        }
                    } else {
                        if (activeNeighborsCount == 3) {
                            output = ACTIVE;
                        } else {
                            output = INACTIVE;
                        }
                    }

                    outGrid.at(x).at(y).at(z).at(w) = output;

                }
            }
        }
    }
}

int main() {

    std::ifstream inFile("./inputs/dec17.txt");
    std::string line;

    std::vector<Grid2D> grid(XLIM, Grid2D(YLIM, std::vector<bool>(ZLIM, false)));
    std::vector<Grid3D> grid4D(XLIM, Grid3D(YLIM, Grid2D(ZLIM, std::vector<bool>(WLIM, false))));
    std::vector<Grid2D> gridCopy( std::begin(grid), std::end(grid) );
    std::vector<Grid3D> grid4DCopy( std::begin(grid4D), std::end(grid4D) );
    Grid2D input;

    while (std::getline(inFile, line)) {
        std::vector<bool> row;
        for (auto const& c : line) {
            row.push_back((c==ACTIVE_CHAR) ? ACTIVE : INACTIVE);
        }
        input.push_back(row);
    }

    const int OFFSET = ((int) input.size()) / 2;
    for (int i = 0; i < (int) input.size(); i++) {
        for (int j = 0; j < (int) input.at(i).size(); j++) {

            grid.at(XCENTER-OFFSET+i).at(YCENTER-OFFSET+j).at(ZCENTER) = input.at(i).at(j);
            
            grid4D.at(XCENTER-OFFSET+i).at(YCENTER-OFFSET+j).at(ZCENTER).at(WCENTER) = input.at(i).at(j);

        }
    }


    for (uint32_t iter = 0; iter < N_ITER; iter++) {
        step(grid, gridCopy);

        for (size_t i = 0; i < grid.size(); i++) {
            for (size_t j = 0; j < grid.at(i).size(); j++) {
                std::copy( std::begin(gridCopy.at(i).at(j)), std::end(gridCopy.at(i).at(j)), std::begin(grid.at(i).at(j)) );
            }
        }
    }

    uint32_t activeCount = 0;
    for (size_t x = 0; x < grid.size(); x++) {
        for (size_t y = 0; y < grid.at(x).size(); y++) {
            for (size_t z = 0; z < grid.at(x).at(y).size(); z++) {
                activeCount += (grid.at(x).at(y).at(z) == ACTIVE) ? 1 : 0;
            }
        }
    }
    printf("Number of active cells 3D = %u\n", activeCount);


    for (uint32_t iter = 0; iter < N_ITER; iter++) {
        step4d(grid4D, grid4DCopy);

        for (size_t i = 0; i < grid4D.size(); i++) {
            for (size_t j = 0; j < grid4D.at(i).size(); j++) {
                for (size_t k = 0; k < grid4D.at(i).at(j).size(); k++) {
                    std::copy( std::begin(grid4DCopy.at(i).at(j).at(k)), std::end(grid4DCopy.at(i).at(j).at(k)), std::begin(grid4D.at(i).at(j).at(k)) );
                }
            }
        }
    }

    activeCount = 0;
    for (size_t x = 0; x < grid4D.size(); x++) {
        for (size_t y = 0; y < grid4D.at(x).size(); y++) {
            for (size_t z = 0; z < grid4D.at(x).at(y).size(); z++) {
                for (size_t w = 0; w < grid4D.at(x).at(y).at(z).size(); w++) {
                    activeCount += (grid4D.at(x).at(y).at(z).at(w) == ACTIVE) ? 1 : 0;
                }
            }
        }
    }
    printf("Number of active cells 4D = %u\n", activeCount);

}