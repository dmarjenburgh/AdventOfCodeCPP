#include "lib/IOUtils.hpp"
#include <array>
#include <iostream>
#include <ranges>
#include <algorithm>

namespace ranges = std::ranges;

int hands_type_from_counts(const std::array<int, 5> &ordered_counts) {
    switch (ordered_counts[0]) {
        case 5: // Five of a kind
        case 4: // Four of a kind
            return ordered_counts[0] + 1;
        case 3: // Full house and Three of a kind
            return ordered_counts[1] == 2 ? 4 : 3;
        case 2:  // Two pair and One pair
            return ordered_counts[1] == 2 ? 2 : 1;
        default:
            // High card
            return 0;
    }
}

int hands_type(const std::string &s) {
    std::unordered_map<char, int> card_counts{};
    std::array<int, 5> ordered_counts{};
    for (auto c: s) {
        card_counts[c]++;
    }
    for (int i{}; const auto& [c, n]: card_counts) {
        ordered_counts[i++] = n;
    }
    ranges::sort(ordered_counts, ranges::greater());
    return hands_type_from_counts(ordered_counts);
}

int hands_type2(const std::string &s) {
    std::unordered_map<char, int> card_counts{};
    std::array<int, 5> ordered_counts{};

    // For Part 2. Each J will increment the highest count
    for (auto c: s) {
        card_counts[c]++;
    }
    for (int i{}; auto [c, n]: card_counts) {
        if (c != 'J') {
            ordered_counts[i++] = n;
        }
    }
    ranges::sort(ordered_counts, ranges::greater());
    ordered_counts[0] += card_counts['J'];
    return hands_type_from_counts(ordered_counts);
}

std::unordered_map<char, long> card_strength;

void init_card_strengths() {
    const std::string values{"23456789TJQKA"};
    for (int r{1}; const auto &item: values) {
        card_strength[item] = r++;
    }
}

using HandBid = std::tuple<std::string, long>;

template<typename HandsTypeFn>
auto smaller_hand_comparator(HandsTypeFn &fn) {
    auto comparator = [&fn](const HandBid &s1, const HandBid &s2) -> bool {
        const auto &hand1{std::get<0>(s1)};
        const auto &hand2{std::get<0>(s2)};
        const int v1{fn(hand1)}, v2{fn(hand2)};
        if (v1 != v2) {
            return v1 < v2;
        }
        // Compare cards
        for (int n{0}; n < 5; ++n) {
            const char &c1{hand1[n]}, &c2{hand2[n]};
            if (c1 != c2) {
                return card_strength[c1] < card_strength[c2];
            }
        }
        return false;
    };
    return comparator;
}

std::vector<HandBid> parse_lines() {
    const std::vector<std::string> lines{ReadAllLines("assets/input7.txt")};
    std::vector<HandBid> result;
    for (auto &line: lines) {
        auto ss = std::stringstream{line};
        std::string hand;
        long bid;
        ss >> hand >> bid;
        result.emplace_back(hand, bid);
    }
    return result;
}

long total_winnings(std::vector<HandBid> hands, auto comparator) {
    auto smaller_hand = smaller_hand_comparator(comparator);
    std::sort(hands.begin(), hands.end(), smaller_hand);

    long total{};
    for (long rank{1}; auto &[hand, bid]: hands) {
        total += (rank++) * bid;
    }
    return total;
}

int main() {
    init_card_strengths();
    const auto hands{parse_lines()};
    std::cout << "Part 1: " << total_winnings(hands, hands_type) << '\n';

    // Part 2
    card_strength['J'] = 0;
    std::cout << "Part 2: " << total_winnings(hands, hands_type2) << '\n';
}