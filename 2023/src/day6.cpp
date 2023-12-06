#include "lib/IOUtils.hpp"
#include <tuple>
#include <cmath>
#include <iostream>
#include <vector>

// Just use the abc-formula
std::tuple<long, long> bounds(long duration, long record) {
    double sqrt_d{std::sqrt(duration * duration - 4 * record)};
    return {std::floor((duration - sqrt_d) / 2 + 1), std::ceil((duration + sqrt_d) / 2 - 1)};
}

struct Race {
    long duration{};
    long record{};
};

#include <regex>

std::tuple<std::vector<Race>, Race> parseInput() {
    const auto &lines{ReadAllLines("assets/input6.txt")};
    std::vector<Race> races;
    std::regex re("\\d+");

    auto duration_it = std::sregex_iterator(lines[0].begin(), lines[0].end(), re);
    auto record_it = std::sregex_iterator(lines[1].begin(), lines[1].end(), re);
    auto end = std::sregex_iterator();

    std::string duration2, record2;
    for (; duration_it != end; ++duration_it, ++record_it) {
        long duration_value = std::stol(duration_it->str());
        long record_value = std::stol(record_it->str());
        races.emplace_back(duration_value, record_value);
        duration2 += duration_it->str();
        record2 += record_it->str();
    }

    return {races, {std::stol(duration2), std::stol(record2)}};
}

long number_of_ways(const std::vector<Race> &races) {
    long result{1};
    for (const auto &race: races) {
        auto [dur_min, dur_max] = bounds(race.duration, race.record);
        result *= (dur_max - dur_min + 1);
    }
    return result;
}

int main() {
    auto [races, actual_race] = parseInput();
    std::cout << "Part 1: " << number_of_ways(races) << '\n';
    std::cout << "Part 2: " << number_of_ways({actual_race}) << '\n';
}