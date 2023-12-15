#include "lib/IOUtils.hpp"
#include <string>
#include <iostream>
#include <array>
#include <vector>
#include <utility>
#include <ranges>

int hash_algorithm(const std::string &step) {
    int value{};
    for (const auto &ch: step) {
        value = (17 * (value + ch)) % 256;
    }
    return value;
}


class HASHMAP {
    using KVPair = std::pair<std::string, int>;
    using LookupMap = std::array<std::vector<KVPair>, 256>;

public:
    void set(const std::string &label, int value) {
        int box{hash_algorithm(label)};
        std::vector<KVPair> &vec{m_map[box]};
        if (auto r{std::ranges::find_if(vec, [&label](const auto &kv) { return kv.first == label; })}; r != vec.end()) {
            r->second = value;
        } else {
            vec.emplace_back(label, value);
        }
    }

    void rm(const std::string &label) {
        int box{hash_algorithm(label)};
        std::vector<KVPair> &vec{m_map[box]};
        if (auto r{std::ranges::find_if(vec, [&label](const auto &kv) { return kv.first == label; })}; r != vec.end()) {
            vec.erase(r);
        }
    }

    [[nodiscard]] int focusing_power() const {
        int pow{};
        for (int i{}; i < 256; ++i) {
            for (int j{}; j < m_map[i].size(); ++j) {
                pow += (i + 1) * (j + 1) * m_map[i][j].second;
            }
        }
        return pow;
    }

private:
    LookupMap m_map;
};

HASHMAP run_init_procedure(const std::vector<std::string> &initialization_sequence) {
    HASHMAP h;
    for (const auto &step: initialization_sequence) {
        if (step.back() == '-') {
            h.rm(step.substr(0, step.size() - 1));
        } else {
            auto eq{step.find('=')};
            std::string label{step.substr(0, eq)};
            int value{std::stoi(step.substr(eq + 1))};
            h.set(label, value);
        }
    }
    return h;
}

std::vector<std::string> parse_initialization_sequence(const std::string &input) {
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string token;

    while (std::getline(ss, token, ',')) {
        result.push_back(token);
    }

    return result;
}

int main() {
    const std::string input{ReadAllLines("assets/input15.txt")[0]};
    const std::vector<std::string> init_sequence{parse_initialization_sequence(input)};
    int sum{};

    for (const auto &s: init_sequence) {
        sum += hash_algorithm(s);
    }
    std::cout << "Part 1: " << sum << '\n';

    const HASHMAP hm{run_init_procedure(init_sequence)};
    std::cout << "Part 2: " << hm.focusing_power() << '\n';
}
