#include <fstream>
#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>

namespace ranges = std::ranges;

using Elf = std::vector<int>;

std::vector<Elf> parseInput() {
  std::ifstream file{"assets/input1.txt", std::ifstream::in};
  std::string num;
  std::vector<Elf> result;
  Elf elf;
  while (std::getline(file, num)) {
    if (num.empty()) {
      result.push_back(elf);
      elf = Elf{};
    } else {
      elf.push_back(std::stoi(num));
    }
  }
  result.push_back(elf);
  return result;
}

int sum_calories(const Elf& elf) {
  return std::reduce(elf.cbegin(), elf.cend());
}

int main() {
  const auto elfs{parseInput()};

  std::vector<int> calories;
  ranges::transform(elfs.cbegin(), elfs.cend(), std::back_inserter(calories), sum_calories);
  ranges::sort(calories.begin(), calories.end());
  const size_t num_elves{calories.size()};
  const int max_calories{calories[num_elves - 1]};
  const std::array top_three = {calories[num_elves - 1], calories[num_elves - 2], calories[num_elves - 3]};
  std::cout << "Part 1: " << max_calories << '\n';
  std::cout << "Part 2: " << top_three[0] + top_three[1] + top_three[2] << '\n';
}
