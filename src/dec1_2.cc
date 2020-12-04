#include <cstdio>
#include <fstream>
#include <vector>

constexpr int TARGET = 2020;

int main(int argc, char **argv) {

    std::vector<int> values;
    std::ifstream input("inputs/dec1_1.txt");

    int tmpVal;
    while (input >> tmpVal) {
        values.push_back(tmpVal);
    }

    int val1=-1, val2=-1, val3=-1;
    for (size_t i = 0; i < values.size(); i++) {
        for (size_t j = i; j < values.size(); j++) {
            for (size_t k = j; k < values.size(); k++) {
                if (values.at(i) + values.at(j) + values.at(k) == TARGET) {
                    val1 = values.at(i);
                    val2 = values.at(j);
                    val3 = values.at(k);
                    break;
                }   
            }
        }
    }

    printf("%d + %d + %d = %d\n", val1, val2, val3, val1+val2+val3);
    printf("%d * %d * %d = %d\n", val1, val2, val3, val1*val2*val3);

    return 0;
}