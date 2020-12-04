#include <cstdio>
#include <fstream>
#include <algorithm>
#include <string>

int main(int argc, char **argv) {

    std::ifstream inFile("./inputs/dec2.txt");

    size_t validPasswordsPart1 = 0, validPasswordsPart2 = 0;

    std::string range, letter, password;
    while (inFile >> range >> letter >> password) {
        
        std::string minStr = range.substr(0, range.find('-'));
        std::string maxStr = range.substr(range.find('-')+1, range.size());

        size_t min = std::stoi(minStr);
        size_t max = std::stoi(maxStr);

        char ruleCharacter = letter.at(0);

        size_t ruleCharacterCount = std::count(password.begin(), password.end(), ruleCharacter);

        if (ruleCharacterCount >= min && ruleCharacterCount <= max) {
            validPasswordsPart1 += 1;
        }

        if ((password.at(min-1) == ruleCharacter) ^ (password.at(max-1) == ruleCharacter)) {
            validPasswordsPart2 += 1;
        }
        
    }

    printf("Number of Valid Passwords Part 1: %lu\n", validPasswordsPart1);
    printf("Number of Valid Passwords Part 2: %lu\n", validPasswordsPart2);

    return 0;
}