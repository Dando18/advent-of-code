#include <cstdio>
#include <string>
#include <fstream>
#include <tuple>
#include <map>


bool canContainTarget(std::string const& s, std::map<std::string, std::map<std::string, int>> const& m) {

    if (!m.contains(s)) return false;

    for (auto const& val : m.at(s)) {
        if (val.first == "shiny gold") {
            return true;
        }
    }

    for (auto const& val : m.at(s)) {
        if (canContainTarget(val.first, m)) {
            return true;
        }
    }

    return false;
}

int countBags(std::string const& s, std::map<std::string, std::map<std::string, int>> const& m) {
    if (!m.contains(s)) {
        return 1;
    }

    int sum = 0;
    for (auto const& v : m.at(s)) {
        sum += countBags(v.first, m)*v.second;
        if (m.contains(v.first)) {
            sum += v.second;
        }
    }
    return sum;
}


std::tuple<std::string, std::map<std::string, int>> parseLine(std::string const& s) {
    std::string bagColor = s.substr(0, s.find(" bags contain"));
    std::map<std::string, int> m;

    std::string endList = s.substr(s.find(" contain") + 9);
    std::string token;
    size_t pos = 0;
    while ((pos = endList.find(", ")) != std::string::npos) {
        token = endList.substr(0, pos);
        endList.erase(0, pos + 2);

        // token is of the form -- "integer color bag(s)"
        std::string countStr = token.substr(0, token.find(" "));
        int count = std::stod(countStr);

        std::string subToken = token.substr(token.find(" ")+1);
        std::string color = subToken.substr(0, subToken.find(" bag"));

        m.insert(std::make_pair(color, count));
    }
    token = endList;
    std::string countStr = token.substr(0, token.find(" "));
    if (countStr != "no") {
        int count = std::stod(countStr);

        std::string subToken = token.substr(token.find(" ")+1);
        std::string color = subToken.substr(0, subToken.find(" bag"));

        m.insert(std::make_pair(color, count));
    }


    return std::make_tuple(bagColor, m);
}

int main() {

    std::ifstream inFile("./inputs/dec7.txt");
    std::string line;
    std::map<std::string, std::map<std::string, int>> table;

    while(std::getline(inFile, line)) {
        auto [bagColor, possibleVals] = parseLine(line);

        if (possibleVals.size() > 0)
            table.insert(std::make_pair(bagColor, possibleVals));
    }

    
    int count = 0;
    for (auto const& v : table) {
        if (canContainTarget(v.first, table)) {
            count += 1;
        }
    }
    printf("count = %d\n", count);


    int numBags = countBags("shiny gold", table);
    printf("num bags needed = %d\n", numBags);
}