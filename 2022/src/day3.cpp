#include <iostream>
#include <numeric>
#include <unordered_set>
#include <vector>

#include "lib/IOUtils.hpp"

int ItemPriority(const char c) {
    return std::isupper(c) ? c - 'A' + 27 : c - 'a' + 1;
}

char OddItem(const std::string_view &s) {
    int m{static_cast<int>(s.size() / 2)};

    std::string_view firstHalf{s.substr(0, m)};
    std::string_view secondHalf{s.substr(m, m)};
    for (const auto &c: firstHalf) {
        for (const auto &d: secondHalf) {
            if (c == d) {
                return c;
            }
        }
    }
    throw std::runtime_error("No packing failure found.");
}

std::vector<char> FindBadgeItemTypes(const std::vector<std::string> &lines) {
    std::vector<char> badgeItemTypes;
    for (auto it{lines.begin()}; it != lines.end(); ++it) {
        std::unordered_set<char> s1{it->begin(), it->end()};
        ++it;
        std::unordered_set<char> s2{it->begin(), it->end()};
        ++it;
        std::unordered_set<char> s3{it->begin(), it->end()};
        for (const auto &c: s1) {
            if (s2.contains(c) && s3.contains(c)) {
                badgeItemTypes.push_back(c);
            }
        }
    }
    return badgeItemTypes;
}

int main() {
    const auto lines{ReadAllLines("assets/input3.txt")};
    int prioritySum{};
    for (const auto &line: lines) {
        int prio{ItemPriority(OddItem(line))};
        prioritySum += prio;
    }

    const auto badgeItemTypes{FindBadgeItemTypes(lines)};

    std::cout << "Part 1: " << prioritySum << '\n';
    prioritySum = std::accumulate(
            badgeItemTypes.begin(), badgeItemTypes.end(), 0,
            [](auto acc, auto itemType) { return acc + ItemPriority(itemType); });
    std::cout << "Part 2: " << prioritySum << '\n';
}
