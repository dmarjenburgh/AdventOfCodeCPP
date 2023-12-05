#include "lib/IOUtils.hpp"
#include <iostream>
#include <numeric>

using Pair = std::array<char, 2>;

/**
 * Evaluates the result of a Rock, Scissors, Paper round.
 * @return +1 for a win, -1 for a loss and 0 for a draw
 */
int RockPaperScissors(char first, char second) {
    int r{((second - 'X') - (first - 'A') + 3) % 3};
    return r == 1 ? 1 : (r == 2 ? -1 : 0);
}

int Score(const Pair &pair) {
    return (pair[1] - 'X' + 1) + RockPaperScissors(pair[0], pair[1]) * 3 + 3;
}

char FindMove(const Pair &pair) {
    switch (pair[1]) {
        case 'X': // Lose
            return static_cast<char>((pair[0] - 'A' + 2) % 3 + 'X');
        case 'Y': // Draw
            return static_cast<char>(pair[0] - 'A' + 'X');
        case 'Z': // Win
            return static_cast<char>((pair[0] - 'A' + 1) % 3 + 'X');
        default:
            throw std::runtime_error("Invalid RPS value.");
    }
}

std::vector<Pair> ParseLines() {
    const auto lines{ReadAllLines("assets/input2.txt")};
    std::vector<Pair> result;
    for (auto it{lines.begin()}; it != lines.end(); ++it) {
        std::stringstream ss{*it};
        char first, second;
        ss >> first >> second;
        result.push_back({first, second});
    }
    return result;
}

int main() {
    const auto pairs{ParseLines()};
    const int totalScore{
            std::accumulate(pairs.cbegin(), pairs.cend(), 0,
                            [](auto acc, auto p) { return acc + Score(p); })};
    const int totalScore2{
            std::accumulate(pairs.cbegin(), pairs.cend(), 0, [](auto acc, auto p) {
                return acc + Score({p[0], FindMove(p)});
            })};
    std::cout << "Part 1: " << totalScore << '\n';
    std::cout << "Part 2: " << totalScore2 << '\n';
}
