#include "lib/IOUtils.hpp"
#include <iostream>
#include <unordered_set>
#include <regex>
#include <ranges>

int num_matches(const std::string &line) {
    const std::regex re{R"(\d+)"};
    auto it_colon{std::ranges::find(line, ':')};
    auto it_pipe{std::ranges::find(line, '|')};
    auto it_begin{std::sregex_iterator(it_colon, it_pipe, re)};
    auto it_end{std::sregex_iterator()};
    std::unordered_set<int> winning_nums{};
    std::unordered_set<int> card_nums{};
    for (auto it{it_begin}; it != it_end; ++it) {
        winning_nums.insert(std::stoi(it->str()));
    }
    it_begin = std::sregex_iterator(it_pipe, line.end(), re);

    for (auto it{it_begin}; it != it_end; ++it) {
        card_nums.insert(std::stoi(it->str()));
    }
    int n{};
    for (const auto &x: winning_nums) { n += card_nums.contains(x); }
    return n;
}

int card_points(const std::string &line) {
    return (1 << num_matches(line)) >> 1;
}

int total_card_points(const std::vector<std::string> &lines) {
    int sum{};
    for (const auto &l: lines) { sum += card_points(l); }
    return sum;
}

int total_num_cards(const std::vector<std::string> &lines) {
    std::unordered_map<int, int> num_copies;
    for (int i{}; i < lines.size(); ++i) {
        int n{num_matches(lines[i])};
        for (int dc{1}; dc <= n; ++dc) {
            num_copies[i + dc] += 1 + num_copies[i];
        }
    }
    int sum{};
    for (const auto &[k, v]: num_copies) {
        sum += v;
    }
    return sum + static_cast<int>(lines.size());
}

int main() {
    const auto lines{ReadAllLines("assets/input4.txt")};
    std::cout << "Part 1: " << total_card_points(lines) << '\n';
    std::cout << "Part 2: " << total_num_cards(lines) << '\n';

}
