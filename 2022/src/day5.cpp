#include "lib/IOUtils.hpp"
#include <iostream>
#include <numeric>
#include <ranges>
#include <regex>
#include <vector>

using Stack = std::string;
using Stacks = std::vector<Stack>;

namespace ranges = std::ranges;

struct Instruction {
  int from;
  int to;
  int amount;
};

size_t numStacks(const std::string &line) {
  const auto it{ranges::find_if(line.rbegin(), line.rend(),
                                [](auto c) { return std::isdigit(c); })};
  const char result{*it};
  return result - '0';
}

std::tuple<Stacks, std::vector<Instruction>> parseInput() {
  const auto lines{ReadAllLines("assets/input5.txt")};
  auto div{ranges::find(lines, "")};
  size_t num_stacks{numStacks(*(div - 1))};

  // Box characters are on indices: 1 + 4n
  Stacks stacks{num_stacks};
  for (auto it{lines.begin()}; it != div; ++it) {
    Stack s;
    for (int idx{}; idx < num_stacks; ++idx) {
      const char c{(*it)[1 + 4 * idx]};
      if (std::isalpha(c)) {
        stacks[idx] += c;
      }
    }
  }
  // Reverse stacks
  ranges::for_each(stacks, ranges::reverse);

  // Parse instructions
  std::vector<Instruction> instructions;
  const std::regex re{R"(move (\d+) from (\d+) to (\d+))"};
  for (auto it{div + 1}; it != lines.end(); ++it) {
    std::smatch sm;
    std::regex_match(*it, sm, re);
    instructions.emplace_back(Instruction{.from = std::stoi(sm[2]) - 1,
                                          .to = std::stoi(sm[3]) - 1,
                                          .amount = std::stoi(sm[1])});
  }
  return {stacks, instructions};
}

void makeMove(Stacks &state, const Instruction &instruction) {
  for (int i{}; i < instruction.amount; ++i) {
    const char last_char{state[instruction.from].back()};
    state[instruction.to] += last_char;
    state[instruction.from].pop_back();
  }
}

void makeMove2(Stacks &state, const Instruction &instruction) {
  size_t from{state[instruction.from].size() - instruction.amount};
  state[instruction.to].append(state[instruction.from].substr(from));
  state[instruction.from].erase(from);
}

std::string topCrates(const Stacks &state) {
  std::stringstream ss;
  ranges::for_each(state, [&ss](auto &crate) { ss << crate.back(); });
  return ss.str();
}

int main() {
  auto [stacks, instructions]{parseInput()};
  std::vector<Stack> stacks2{stacks};

  for (const auto &item : instructions) {
    makeMove(stacks, item);
    makeMove2(stacks2, item);
  }
  std::cout << "Part 1: " << topCrates(stacks) << '\n';
  std::cout << "Part 2: " << topCrates(stacks2) << '\n';
}
