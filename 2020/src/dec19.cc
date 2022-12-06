#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cstdint>

struct Rule {
    size_t id = 0;
    std::vector<std::string> rules {};
    
    bool matches(std::string const& s) { return std::find(std::begin(rules), std::end(rules), s) != std::end(rules); }
};

std::vector<size_t> parseListOfInts(std::string const& s) {
    std::istringstream ss(s);
    std::string val;
    std::vector<size_t> vals;
    while (std::getline(ss, val, ' ')) {
        vals.push_back(std::stoul(val));
    }
    return vals;
}

Rule getRule(std::map<size_t, std::string> const& input, std::map<size_t, Rule> &rules, size_t idx, size_t maxMessageLen);

std::vector<std::string> expandRules(std::vector<Rule> const& rules) {
    if (rules.size() == 0) return {""};

    std::vector<std::vector<std::string>> possibleStrings (rules.size());
    possibleStrings.at(0) = rules.at(0).rules;
    for (size_t i = 1; i < possibleStrings.size(); i++) {
        for (auto const& s1 : possibleStrings.at(i-1)) {
            for (auto const& s2 : rules.at(i).rules) {
                possibleStrings.at(i).push_back(s1 + s2);
            }
        }
    }
    return possibleStrings.back();
}


void _recursiveBuild(std::string const& s, std::set<std::string> &dst, std::vector<std::string> const& src1, std::vector<std::string> const& src2, size_t maxSize) {
    for (auto const& l : src1) {
        for (auto const& r : src2) {
            std::string tmp = l + s + r;
            if (tmp.size() <= maxSize) {
                //dst.push_back(tmp);
                dst.insert(tmp);

                _recursiveBuild(tmp, dst, src1, src2, maxSize);
            }
        }
    }
}

std::vector<std::string> calculatePossibleRules(std::vector<size_t> const& targets, std::map<size_t, std::string> const& input, std::map<size_t, Rule> &rules, size_t idx, size_t maxMessageLen) {

    std::vector<Rule> collectedRules;
    bool selfLoop = false;
    size_t selfLoopLoc = 0;

    for (size_t i = 0; i < targets.size(); i++) {
        auto t = targets.at(i);
        if (t != idx) {
            collectedRules.push_back( getRule(input, rules, t, maxMessageLen) );
        } else {
            selfLoop = true;
            selfLoopLoc = i;
        }
    }

    if (!selfLoop) {
        return expandRules(collectedRules);
    } else {
        std::vector<Rule> firstTargets, secondTargets;
        // get targets [0,i-1]
        for (size_t i = 0; i < selfLoopLoc; i++) {
            firstTargets.push_back(collectedRules.at(i));
        }
        auto firstPossible = expandRules(firstTargets);

        // get targets [i, collectedRules.size()-1]
        for (size_t i = selfLoopLoc; i < collectedRules.size(); i++) {
            secondTargets.push_back(collectedRules.at(i));
        }
        auto secondPossible = expandRules(secondTargets);

        printf("building possible rules for %lu (left %lu, right %lu)...\t", idx, firstPossible.size(), secondPossible.size()); fflush(stdout);
        std::set<std::string> allPossibleSet;
        _recursiveBuild("", allPossibleSet, firstPossible, secondPossible, maxMessageLen);
        printf("Done\n");

        std::vector<std::string> allPossible (allPossibleSet.begin(), allPossibleSet.end());
        return allPossible;
    }
}

Rule getRule(std::map<size_t, std::string> const& input, std::map<size_t, Rule> &rules, size_t idx, size_t maxMessageLen) {
    if (rules.contains(idx)) {
        return rules.at(idx);
    }

    Rule r {0, {}};
    r.id = idx;
    std::string curLine = input.at(idx);

    if (curLine.find('"') != std::string::npos) {
        /* has quote to read token and add to rules */
        std::string value = curLine.substr(curLine.find_first_of('"')+1, 1);
        r.rules.push_back(value);
    } else if (curLine.find('|') != std::string::npos) {
        /* two parts */
        std::string firstPart = curLine.substr(0, curLine.find('|'));
        std::string secondPart = curLine.substr(curLine.find('|')+2);
        auto firstTargets = parseListOfInts(firstPart);
        auto secondTargets = parseListOfInts(secondPart);

        auto rules1 = calculatePossibleRules(firstTargets, input, rules, idx, maxMessageLen);
        auto rules2 = calculatePossibleRules(secondTargets, input, rules, idx, maxMessageLen);

        r.rules.clear();
        r.rules.reserve(rules1.size() + rules2.size());
        r.rules.insert(r.rules.end(), rules1.begin(), rules1.end());
        r.rules.insert(r.rules.end(), rules2.begin(), rules2.end());
        
    } else {
        /* one part */
        auto targets = parseListOfInts(curLine);
        r.rules = calculatePossibleRules(targets, input, rules, idx, maxMessageLen);
    }

    rules[idx] = r;
    return r;
}

bool _matches(std::map<size_t, std::string> const& input, std::string &s, size_t idx) {
    if (s == "") return true;

    std::string curRule = input.at(idx);
    std::string firstS = s;

    //printf("%lu: %s\n", idx, s.c_str());

    if (curRule.find('"') != std::string::npos) {
        /* has quote to read token and add to rules */
        std::string value = curRule.substr(curRule.find_first_of('"')+1, 1);
        if (s.starts_with(value)) {
            s = s.substr(1);
            return true;
        } else {
            return false;
        }
    } else if (curRule.find('|') != std::string::npos) {
        /* two parts */
        std::string firstPart = curRule.substr(0, curRule.find('|'));
        std::string secondPart = curRule.substr(curRule.find('|')+2);
        auto firstTargets = parseListOfInts(firstPart);
        auto secondTargets = parseListOfInts(secondPart);

        bool works = true;
        for (auto const& t : firstTargets) {
            if (!_matches(input, s, t)) {
                works = false;
                s = firstS;
                break;
            }
        }
        if (works) return true;

        for (auto const &t : secondTargets) {
            if (!_matches(input, s, t)) {
                s = firstS;
                return false;
            }
        }

        return true;
        
    } else {
        /* one part */
        auto targets = parseListOfInts(curRule);

        for (auto const& t : targets) {
            if (!_matches(input, s, t)) {
                s = firstS;
                return false;
            }
        }
        return true;
    }
}

bool matches(std::map<size_t, std::string> const& input, std::string const& s, size_t idx) {
    std::string sCopy = s;
    return _matches(input, sCopy, idx) && (sCopy.size() == 0);
}

int main() {

    std::ifstream inFile("inputs/dec19.txt");
    std::string line;
    std::vector<std::string> messages;
    std::map<size_t, std::string> inputLines, inputLinesWithLoop;
    std::map<size_t, Rule> rules;
    size_t maxMessageLen = 0;

    while (std::getline(inFile, line)) {
        if (line == "") break;

        size_t ruleNo = std::stoul(line.substr(0, line.find(':')));
        std::string endOfLine = line.substr(line.find(':')+2);

        inputLines.insert({ruleNo, endOfLine});

        if (ruleNo == 8) {
            endOfLine += " | 42 8";
        } else if (ruleNo == 11) {
            endOfLine += " | 42 11 31";
        }

        inputLinesWithLoop.insert({ruleNo, endOfLine});
    }

    while (std::getline(inFile, line)) {
        messages.push_back(line);
        maxMessageLen = std::max(maxMessageLen, line.size());
    }


    Rule rule0 = getRule(inputLines, rules, 0, maxMessageLen);
    size_t sum = 0;
    for (auto const& m : messages) {
        sum += (rule0.matches(m)) ? 1 : 0;
    }
    printf("total matches to rule 0 = %lu\n", sum);

    sum = 0;
    for (auto const& m : messages) {
        //sum += (matches(inputLinesWithLoop, m, 0)) ? 1 : 0;
        if (matches(inputLinesWithLoop, m, 0)) {
            sum += 1;
            printf("%s matches\n", m.c_str());
        } else {
            //printf("%s does not match\n", m.c_str());
        }
    }
    printf("total matches to rule 0 with loops = %lu\n", sum);
}