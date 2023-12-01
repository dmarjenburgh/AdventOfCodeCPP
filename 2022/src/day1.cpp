#include "lib/IOUtils.hpp"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

namespace ranges = std::ranges;

std::vector<int> parseInput() {
  std::vector<int> result;
  // Count total calories per elf
  const auto lines{ReadAllLines("assets/input1.txt")};
  for (auto it{lines.begin()}; it < lines.end(); ++it) {
    int calories{};
    while (it != lines.end() && !(*it).empty()) {
      calories += std::stoi(*it);
      ++it;
    }
    result.emplace_back(calories);
  }
  return result;
}

int main() {
  auto calorieCounts{parseInput()};
  ranges::sort(calorieCounts.begin(), calorieCounts.end());

  const size_t num_elves{calorieCounts.size()};
  const int max_calories{calorieCounts[num_elves - 1]};
  const std::array top_three = {calorieCounts[num_elves - 1],
                                calorieCounts[num_elves - 2],
                                calorieCounts[num_elves - 3]};
  std::cout << "Part 1: " << max_calories << '\n';
  std::cout << "Part 2: " << top_three[0] + top_three[1] + top_three[2] << '\n';
}
