/**
 * @file dec13.cc
 * @author Daniel Nichols
 * @date December 2022
 * @brief AOC 2022 Day 13 https://adventofcode.com/2022/day/13
 * 
 */
// stl includes
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <vector>

// local includes
#include "utilities.hpp"

constexpr auto INPUT_FILE_PATH = "/home/daniel/dev/personal/advent-of-code/2022/inputs/dec13.txt";

std::vector<std::string> parseNestedList(std::string const& s, char delim=',', char lbracket='[', char rbracket=']') {

    int32_t balance = 0;
    std::string tmp;
    std::vector<std::string> parts;

    for (auto const& c : s) {
        if (balance == 1 && ((c == delim) || (c == rbracket))) {
            if (!tmp.empty()) parts.push_back(std::move(tmp));
            tmp = "";
            continue;
        }

        if (balance >= 1) {
            if (tmp.empty() && c == ' ') continue;
            tmp += c;
        }

        if (c == lbracket) balance += 1;
        else if (c == rbracket) balance -= 1;
    }

    return parts;
}

bool isList(std::string const& s) {
    return s.find('[') != std::string::npos;
}

bool isInteger(std::string const& s) {
    return !s.empty() && std::all_of(std::begin(s), std::end(s), ::isdigit);
}

enum Order {
    CORRECT = 1, INCORRECT = -1, UNDECIDED = 0
};

Order isInRightOrder(std::string const& packet1, std::string const& packet2) {
    const auto parts1 = parseNestedList(packet1);
    const auto parts2 = parseNestedList(packet2);

    const size_t lim = std::min(parts1.size(), parts2.size());
    for (size_t i = 0; i < lim; i += 1) {
        auto item1 = parts1.at(i);
        auto item2 = parts2.at(i);

        if (isInteger(item1) && isInteger(item2)) {
            const int64_t integer1 = std::stoll(item1);
            const int64_t integer2 = std::stoll(item2);

            if (integer1 < integer2) return CORRECT;
            if (integer1 > integer2) return INCORRECT;
        } else {
            if (isInteger(item1)) item1 = '[' + item1 + ']';
            if (isInteger(item2)) item2 = '[' + item2 + ']';

            const auto result = isInRightOrder(item1, item2);
            if (result != UNDECIDED) return result;
        }
    }

    if (parts1.size() == parts2.size()) return UNDECIDED;
    return (parts1.size() <= parts2.size()) ? CORRECT : INCORRECT;
}


int main() {

    const auto lines = util::readLines(INPUT_FILE_PATH);
    std::vector<std::pair<std::string, std::string>> packets;
    for (size_t i = 0; i < lines.size(); i += 3) {
        packets.push_back( {lines.at(i), lines.at(i+1)} );
    }

    // part 1 -- find the number of packet pairs that are in the right order
    uint32_t indexSum = 0;
    for (size_t i = 0; i < packets.size(); i += 1) {
        if (isInRightOrder(packets.at(i).first, packets.at(i).second) == CORRECT) {
            indexSum += (i+1);
        }
    }
    std::cout << indexSum << '\n';

    // part 2 -- sort the packets and return the multiple of the indices of the special packets
    auto cmp = [](std::string const& p1, std::string const& p2) { return isInRightOrder(p1, p2) == CORRECT; };
    std::set<std::string, decltype(cmp)> allPackets;
    std::for_each(std::begin(packets), std::end(packets), [&allPackets](auto const& p) { 
        allPackets.insert(p.first); allPackets.insert(p.second); });
    
    const auto firstIter = allPackets.insert("[[2]]").first;
    const auto secondIter = allPackets.insert("[[6]]").first;

    const uint32_t decoderKey = (std::distance(std::begin(allPackets), firstIter) + 1) 
                               * (std::distance(std::begin(allPackets), secondIter) + 1);
    std::cout << decoderKey << '\n';
}