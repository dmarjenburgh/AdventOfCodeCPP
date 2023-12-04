#include "lib/IOUtils.hpp"
#include "lib/matrix.hpp"
#include <iostream>
#include <regex>
#include <unordered_set>
#include <vector>

using Grid = std::vector<std::string>;

std::unordered_set<Point> filter_grid_positions(const Grid &g, std::predicate<char> auto pred) {
    std::unordered_set<Point> points;
    for (int y = 0; y < g.size(); y++) {
        for (int x = 0; x < g[y].length(); x++) {
            if (pred(g[y][x])) {
                points.insert({x, y});
            }
        }
    }
    return points;
}

std::tuple<int, int> getNumber(const Grid &g, int row, int col) {
    int start = col, end = col;
    while (start > 0 && isdigit(g[row][start - 1]))
        --start;
    while (end < g[row].size() - 1 && isdigit(g[row][end + 1]))
        ++end;
    return {std::stoi(g[row].substr(start, end - start + 1)), end};
}


std::vector<int> process_adjacent_numbers(const Grid &g, int r, int c) {
    std::vector<int> numbers{};
    for (int row: {r - 1, r, r + 1}) {
        for (int col{c - 1}; col <= c + 1; ++col) {
            if (std::isdigit(g[row][col])) {
                auto [n, right_i] = getNumber(g, row, col);
                numbers.push_back(n);;
                col = right_i;
            }
        }
    }
    return numbers;
}

int sumOfPartNumbers(const Grid &g) {
    auto is_symbol = [](char c) { return !(std::isdigit(c) || c == '.'); };
    const auto symbols{filter_grid_positions(g, is_symbol)};
    int sum{};
    for (const auto p: symbols) {
        for (const auto n: process_adjacent_numbers(g, p.y, p.x)) {
            sum += n;
        }
    }
    return sum;
}

int sumOfGearRatios(const Grid &g) {
    auto is_gear = [](char c) { return c == '*'; };
    const auto gears{filter_grid_positions(g, is_gear)};
    int sum{};
    for (const auto p: gears) {
        const auto numbers{process_adjacent_numbers(g, p.y, p.x)};
        if (numbers.size() == 2){
            sum+=numbers[0]*numbers[1];
        }
    }
    return sum;
}

int main() {
    const auto lines{ReadAllLines("assets/input3.txt")};
    std::cout << "Part 1: " << sumOfPartNumbers(lines) << '\n';
    std::cout << "Part 2: " << sumOfGearRatios(lines) << '\n';
}
