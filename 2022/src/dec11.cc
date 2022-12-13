/**
 * @file dec11.cc
 * @author Daniel Nichols
 * @date December 2022
 * @brief AOC 2022 Day 11 https://adventofcode.com/2022/day/11
 *
 */
// stl includes
#include <iostream>
#include <limits>
#include <list>
#include <numeric>
#include <string>
#include <vector>

// local includes
#include "utilities.hpp"

constexpr auto INPUT_FILE_PATH = "/home/daniel/dev/personal/advent-of-code/2022/inputs/dec11.txt";

typedef uint64_t Item;

class Monkey {
  public:
    Monkey(uint32_t id, std::list<Item> items, char modifierOperator, Item modifierAmount, bool modifyWithCurrentValue,
           Item testDivisor, uint32_t destinationOnSuccess, uint32_t destinationOnFailure)
        : id_(id), items_(items), modifierOperator_(modifierOperator), modifierAmount_(modifierAmount),
          modifyWithCurrentValue_(modifyWithCurrentValue), testDivisor_(testDivisor),
          ringSize_(std::numeric_limits<Item>::max()), destinationOnSuccess_(destinationOnSuccess),
          destinationOnFailure_(destinationOnFailure), numInspections_(0) {}

    void receive(Item const &item) noexcept { items_.push_back(item); }

    uint64_t getNumInspections() const noexcept { return this->numInspections_; }

    Item getTestDivisor() const noexcept { return this->testDivisor_; }

    Item getRingSize() const noexcept { return this->ringSize_; }

    void setRingSize(Item ringSize) noexcept { this->ringSize_ = ringSize; }

    void takeTurn(std::vector<Monkey> &monkeys, bool isWorryReduced = true) noexcept {
        while (!items_.empty()) {
            auto item = items_.front();
            items_.pop_front();

            /* inspect */
            item = this->applyModifier_(item);
            if (!isWorryReduced) item = item % ringSize_; /* modulo product(m.testDivisor for m in monkeys) */
            numInspections_ += 1;

            /* monkey gets bored */
            if (isWorryReduced) item = item / 3;

            /* test */
            if (item % testDivisor_ == 0) {
                monkeys.at(destinationOnSuccess_).receive(item);
            } else {
                monkeys.at(destinationOnFailure_).receive(item);
            }
        }
    }

    friend std::ostream &operator<<(std::ostream &oss, Monkey const &monkey);

  private:
    uint32_t id_;
    std::list<Item> items_;
    char modifierOperator_;
    Item modifierAmount_;
    bool modifyWithCurrentValue_;
    Item testDivisor_, ringSize_;
    uint32_t destinationOnSuccess_, destinationOnFailure_;
    uint64_t numInspections_;

    Item applyModifier_(Item const &item) const noexcept {
        Item modifier = modifierAmount_;
        if (modifyWithCurrentValue_) modifier = item;

        switch (modifierOperator_) {
        case '*':
            return item * modifier;
        case '+':
            return item + modifier;
        default:
            return item;
        }
    }
};

std::ostream &operator<<(std::ostream &oss, Monkey const &monkey) {
    oss << "Monkey " << monkey.id_ << " (numInspections = " << monkey.numInspections_ << "):\n\tStarting items: ";
    for (auto const &item : monkey.items_) {
        oss << item << ' ';
    }
    oss << "\n\tOperation: new = old " << monkey.modifierOperator_ << ' '
        << ((monkey.modifyWithCurrentValue_) ? "old" : std::to_string(monkey.modifierAmount_)) << "\n";
    oss << "\tTest: divisible by " << monkey.testDivisor_ << "\n";
    oss << "\t\tIf true: throw to monkey " << monkey.destinationOnSuccess_ << "\n";
    oss << "\t\tIf false: throw to monkey " << monkey.destinationOnFailure_ << "\n";
    return oss;
}

Monkey parseMonkeyFromLineRange(std::vector<std::string>::const_iterator start, uint32_t id = 0) {
    auto iter = start;

    /* skip first line */
    iter += 1;

    /* starting items */
    std::list<Item> startingItems;
    const auto itemsString = util::split(*iter, ':').back();
    for (const auto &itemStr : util::split(itemsString, ',')) {
        startingItems.push_back(std::stoull(itemStr));
    }
    iter += 1;

    /* operation */
    const auto parts = util::split(*iter, ' ');
    const char operation = parts.at(parts.size() - 2).at(0);
    Item amount = 0;
    bool modifyWithCurrentValue = false;
    if (parts.back() == "old") {
        modifyWithCurrentValue = true;
    } else {
        amount = std::stoull(parts.back());
    }
    iter += 1;

    /* test */
    const Item testDivisor = std::stoull(util::split(*iter, ' ').back());
    iter += 1;
    const uint32_t destinationOnSuccess = std::stoul(util::split(*iter, ' ').back());
    iter += 1;
    const uint32_t destinationOnFailure = std::stoul(util::split(*iter, ' ').back());
    iter += 1;

    return Monkey(id, startingItems, operation, amount, modifyWithCurrentValue, testDivisor, destinationOnSuccess,
                  destinationOnFailure);
}

int main() {

    const auto lines = util::readLines(INPUT_FILE_PATH);
    std::vector<Monkey> monkeys;
    uint32_t idCounter = 0;
    for (auto it = std::begin(lines); it < std::end(lines); it = std::next(it, 7)) {
        monkeys.push_back(parseMonkeyFromLineRange(it, idCounter++));
    }
    uint64_t ringSize = std::accumulate(std::begin(monkeys), std::end(monkeys), 1,
                                        [](auto const &p, auto const &x) { return p * x.getTestDivisor(); });
    std::for_each(std::begin(monkeys), std::end(monkeys), [ringSize](auto &m) { m.setRingSize(ringSize); });
    const auto originalMonkeys = monkeys;

    // part 1 -- simulate monkeys throwing objects and record most interactions
    uint32_t numRounds = 20;
    for (uint32_t round = 0; round < numRounds; round += 1) {
        for (auto &monkey : monkeys) {
            monkey.takeTurn(monkeys);
        }
    }
    std::sort(std::begin(monkeys), std::end(monkeys),
              [](auto const &a, auto const &b) -> bool { return a.getNumInspections() < b.getNumInspections(); });
    uint64_t monkeyBusiness = monkeys.back().getNumInspections() * monkeys.at(monkeys.size() - 2).getNumInspections();
    std::cout << monkeyBusiness << "\n";

    // part 2 -- using new rules simulate 10000 rounds and record monkey business
    numRounds = 10000;
    monkeys = originalMonkeys;
    for (uint32_t round = 0; round < numRounds; round += 1) {
        for (auto &monkey : monkeys) {
            monkey.takeTurn(monkeys, false);
        }
    }
    std::sort(std::begin(monkeys), std::end(monkeys),
              [](auto const &a, auto const &b) -> bool { return a.getNumInspections() < b.getNumInspections(); });
    monkeyBusiness = monkeys.back().getNumInspections() * monkeys.at(monkeys.size() - 2).getNumInspections();
    std::cout << monkeyBusiness << "\n";
}