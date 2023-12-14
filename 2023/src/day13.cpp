#include "lib/IOUtils.hpp"
#include <vector>
#include <string>
#include <iostream>

using Pattern = std::vector<std::string>;

std::vector<std::string> transpose(const std::vector<std::string> &input) {
    std::size_t num_strings{input.size()};
    std::size_t length{input[0].size()};
    std::vector<std::string> transposed(length);

    for (std::size_t i{}; i < length; ++i) {
        transposed[i].reserve(num_strings);
        for (std::size_t j{}; j < num_strings; ++j) {
            transposed[i] += input[j][i];
        }
    }

    return transposed;
}

int count_differences(const std::string &str1, const std::string &str2) {
    int difference_count{};
    for (std::size_t i{}; i < str1.size(); ++i) {
        if (str1[i] != str2[i]) { ++difference_count; }
    }
    return difference_count;
}

int difference_score(const Pattern &pattern, int r) {
    int diff{};
    for (int i{}; r - i >= 0 && r + i + 1 < pattern.size(); ++i) {
        diff += count_differences(pattern[r - i], pattern[r + i + 1]);
    }
    return diff;
}

int find_with_difference_score(const Pattern &pattern, int score) {
    for (int i{}; i < pattern.size() - 1; ++i) {
        if (difference_score(pattern, i) == score) {
            return i;
        };
    }
    return -1;
}

int find_reflection_value(const Pattern &pattern, int difference) {
    int r{find_with_difference_score(pattern, difference)};
    return r == -1 ? (find_with_difference_score(transpose(pattern), difference) + 1) : 100 * (r + 1);
}

std::vector<Pattern> parsePatterns(const std::vector<std::string> &lines) {
    std::vector<Pattern> patterns;
    Pattern current_pattern;
    for (const auto &line: lines) {
        if (line.empty()) {
            patterns.push_back(current_pattern);
            current_pattern.clear();
        } else {
            current_pattern.push_back(line);
        }
    }
    patterns.push_back(current_pattern);
    return patterns;
}

int main() {
    const auto lines{ReadAllLines("assets/input13.txt")};
    const auto patterns{parsePatterns(lines)};
    int sum{}, sum2{};
    for (const auto &pattern: patterns) {
        sum += find_reflection_value(pattern, 0);
        sum2 += find_reflection_value(pattern, 1);
    }
    std::cout << "Part 1: " << sum << '\n';
    std::cout << "Part 1: " << sum2 << '\n';
}