/**
 * @file dec02.cc
 * @author Daniel Nichols
 * @date December 2022
 * @brief AOC 2022 Day 2 https://adventofcode.com/2022/day/2
 * 
 */
// stl includes
#include <iostream>
#include <numeric>
#include <string>

// local includes
#include "utilities.hpp"

constexpr auto INPUT_FILE_PATH = "/home/daniel/dev/personal/advent-of-code/2022/inputs/dec02.txt";

enum MoveType {
    ROCK,
    PAPER,
    SCISSORS
};

struct Round {
    MoveType opponentMove, playerMove;
};


int32_t getRoundScore(Round const& round) {
    int32_t score = (round.playerMove == ROCK) ? 1 : (round.playerMove == PAPER) ? 2 : 3;

    if (round.playerMove == round.opponentMove) {
        return score + 3;
    }

    if (round.playerMove == ROCK) {
        score += (round.opponentMove == SCISSORS) ? 6 : 0;
    } else if (round.playerMove == PAPER) {
        score += (round.opponentMove == ROCK) ? 6 : 0;
    } else {
        score += (round.opponentMove == PAPER) ? 6 : 0;
    }

    return score;
}


int main() {

    auto lines = util::readLines(INPUT_FILE_PATH);
    std::vector<Round> rounds, roundsPart2;

    for (auto const& line : lines) {
        Round curRound, roundPart2;
        const char opponent = line[0];
        const char player = line[2];

        if (opponent == 'A') curRound.opponentMove = ROCK;
        else if (opponent == 'B') curRound.opponentMove = PAPER;
        else if (opponent == 'C') curRound.opponentMove = SCISSORS;

        roundPart2 = curRound;

        if (player == 'X') {
            curRound.playerMove = ROCK;
            roundPart2.playerMove = (roundPart2.opponentMove == ROCK) ? SCISSORS : (roundPart2.opponentMove == PAPER) ? ROCK : PAPER;
        } else if (player == 'Y') {
            curRound.playerMove = PAPER;
            roundPart2.playerMove = roundPart2.opponentMove;
        } else if (player == 'Z') {
            curRound.playerMove = SCISSORS;
            roundPart2.playerMove = (roundPart2.opponentMove == ROCK) ? PAPER : (roundPart2.opponentMove == PAPER) ? SCISSORS : ROCK;
        }

        rounds.push_back(curRound);
        roundsPart2.push_back(roundPart2); 
    }

    // part 1 -- sum of normal scores
    uint32_t sum = std::transform_reduce(std::begin(rounds), std::end(rounds), 0, std::plus{}, getRoundScore);
    std::cout << sum << "\n";

    // part 2 -- sum of rounds with different encoding
    sum = std::transform_reduce(std::begin(roundsPart2), std::end(roundsPart2), 0, std::plus{}, getRoundScore);
    std::cout << sum << "\n";
}