#include <cstdio>
#include <cstdint>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <limits>
#include <bitset>

struct Instruction {
    size_t address_;
    uint64_t value_;
};

Instruction parseLine(std::string const& s) {
    std::string addressStr = s.substr(4, s.find(']')-4);
    size_t address = std::stoul(addressStr);

    std::string valueStr = s.substr(s.find_last_of(' ')+1);
    uint64_t value = std::stoull(valueStr);

    return {address, value};
}

void _recursiveAddAddresses(std::vector<size_t> &addresses, std::string addressStr) {
    size_t firstX = addressStr.find_first_of("X");
    if (firstX == std::string::npos) {
        /* apply mask and add to vec */
        uint64_t address = std::stoull(addressStr, nullptr, 2);
        addresses.push_back(address);
    } else {
        addressStr.at(firstX) = '0';
        _recursiveAddAddresses(addresses, addressStr);
        addressStr.at(firstX) = '1';
        _recursiveAddAddresses(addresses, addressStr);
    }
}

std::vector<size_t> getAddresses(std::string const& mask, size_t address) {
    std::vector<size_t> addresses;

    std::string addressStr = std::bitset<std::numeric_limits<uint64_t>::digits>(address).to_string();

    /* enforce all ones */
    for (size_t i = 0; i < mask.size(); i++) {
        if (mask.at(mask.size() - i-1) == 'X') {
            addressStr.at(addressStr.size() - i-1) = 'X';
        } else if (mask.at(mask.size() - i-1) == '1') {
            addressStr.at(addressStr.size() - i-1) = '1';
        }
    }

    _recursiveAddAddresses(addresses, addressStr);
    return addresses;
}

int main() {

    std::ifstream inFile("./inputs/dec14.txt");
    std::string line;

    std::map<size_t, uint64_t> memory1, memory2;

    uint64_t onesMask = 0, zerosMask = 0;
    std::string maskStr;
    while (std::getline(inFile, line)) {
        if (line.find("mask") == 0) {
            maskStr = line.substr(line.find_last_of(' ')+1);
            std::string onesMaskStr = maskStr, zerosMaskStr = maskStr;

            std::replace(std::begin(onesMaskStr), std::end(onesMaskStr), 'X', '0');
            onesMask = std::stoull(onesMaskStr, nullptr, 2);

            std::replace(std::begin(zerosMaskStr), std::end(zerosMaskStr), 'X', '1');
            zerosMask = std::stoull(zerosMaskStr, nullptr, 2);
            
        } else {
            auto instr = parseLine(line);
            uint64_t val = (instr.value_ | onesMask) & zerosMask;
            memory1[instr.address_] = val;

            /* part2 -- write all possible addresses */
            auto addresses = getAddresses(maskStr, instr.address_);
            for (auto const& address : addresses) {
               memory2[address] = instr.value_;
            }
            
        }
    }

    uint64_t sum = 0;
    for (auto const& [address, value] : memory1) {
        sum += value;
    }
    printf("sum of values in memory part 1 = %lu\n", sum);

    sum = 0;
    for (auto const& [address, value] : memory2) {
        sum += value;
    }
    printf("sum of values in memory part 2 = %lu\n", sum);
}