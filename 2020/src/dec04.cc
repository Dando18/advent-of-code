#include <fstream>
#include <sstream>
#include <cstdio>
#include <algorithm>
#include <set>


inline bool isAlphaNumeric(std::string const& s);
inline bool isNumeric(std::string const& s);
inline bool valueCheck(std::string const& field, std::string const& value);


int main() {

    std::ifstream inFile("./inputs/dec4.txt");

    std::set<std::string> passport;
    std::set<std::string> fullSet = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid", "cid"};
    std::set<std::string> partialSet = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};
    size_t validPassportsPart1 = 0, validPassportsPart2 = 0;

    std::string line;
    bool isValid = true;
    while (getline(inFile, line)) {
        if (line.find_first_not_of(" \t\n\v\f\r") != std::string::npos) {
            
            std::istringstream iss(line);
            std::string token;
            while (iss >> token) {
                std::string field = token.substr(0, token.find(':'));
                passport.insert(field);

                std::string value = token.substr(token.find(':')+1, token.size());
                isValid &= valueCheck(field, value);
            }

        } else {

            if ((passport == partialSet) || (passport == fullSet)) {
                if (isValid) {
                    validPassportsPart2 += 1;
                }
                validPassportsPart1 += 1;
            }

            passport.clear();
            isValid = true;
        }
    }
    if (passport == partialSet || passport == fullSet) {
        if (isValid) {
            validPassportsPart2 += 1;
        }
        validPassportsPart1 += 1;
    }

    printf("Valid Passports Part 1: %lu\n", validPassportsPart1);
    printf("Valid Passports Part 2: %lu\n", validPassportsPart2);

    return 0;
}

inline bool isAlphaNumeric(std::string const& s) {
    return std::find_if_not(s.begin(), s.end(), [](unsigned char c) { return std::isalnum(c); }) == std::end(s);
}

inline bool isNumeric(std::string const& s) {
    return std::find_if_not(s.begin(), s.end(), [](unsigned char c) { return std::isdigit(c); }) == std::end(s);
}


/* This is a really ugly function but it's 1 am and the problem statement kinda sucks */
bool valueCheck(std::string const& field, std::string const& value) {
    if (field == "byr") {
        int byr = std::stoi(value);
        return byr >= 1920 && byr <= 2002;
    } else if (field == "iyr") {
        int iyr = std::stoi(value);
        return iyr >= 2010 && iyr <= 2020;
    } else if (field == "eyr") {
        int eyr = std::stoi(value);
        return eyr >= 2020 && eyr <= 2030;
    } else if (field == "hgt") {
        if (value.at(value.size()-2) == 'i' && value.back() == 'n') {
            int inches = std::stoi(value.substr(0, value.find("in")));
            return inches >= 59 && inches <= 76;
        } else if (value.at(value.size()-2) == 'c' && value.back() == 'm') {
            int cm = std::stoi(value.substr(0, value.find("cm")));
            return cm >= 150 && cm <= 193;
        } else {
            return false;
        }
    } else if (field == "hcl") {
        return value.at(0) == '#' && value.size() == 7 && isAlphaNumeric(value.substr(1, value.size()));
    } else if (field == "ecl") {
        return (value == "amb") || (value == "amb") || (value == "blu") || (value == "brn") || (value == "gry") ||
               (value == "grn") || (value == "hzl") || (value == "oth");
    } else if (field == "pid") {
        return (value.size() == 9) && isNumeric(value);
    } else if (field == "cid") {
        return true;
    }
    return false;
}