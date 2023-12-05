#include "lib/IOUtils.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <regex>
#include <vector>

using Range = std::array<int, 2>;
using RangePair = std::array<Range, 2>;
namespace ranges = std::ranges;

std::vector<RangePair> parseInput() {
    const auto lines{ReadAllLines("assets/input4.txt")};
    const std::regex re{R"((\d+)-(\d+),(\d+)-(\d+))"};
    std::vector<RangePair> result;
    for (const auto &line: lines) {
        std::smatch sm;
        std::regex_match(line, sm, re);
        Range p1{std::stoi(sm[1]), std::stoi(sm[2])};
        Range p2{std::stoi(sm[3]), std::stoi(sm[4])};
        result.push_back({p1, p2});
    }
    return result;
}

bool isContained(const Range &r1, const Range &r2) {
    return (r1[0] <= r2[0]) && (r1[1] >= r2[1]);
}

bool overlaps(const Range &r1, const Range &r2) {
    return (r1[1] >= r2[0]) && (r1[0] <= r2[1]);
}

int main() {
    const auto pairs{parseInput()};
    size_t numContained = ranges::count_if(pairs, [](auto rp) {
        return isContained(rp[0], rp[1]) || isContained(rp[1], rp[0]);
    });
    size_t numOverlaps =
            ranges::count_if(pairs, [](auto rp) { return overlaps(rp[0], rp[1]); });

    std::cout << "Part 1: " << numContained << '\n';
    std::cout << "Part 2: " << numOverlaps << '\n';
}
