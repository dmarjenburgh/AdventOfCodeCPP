#include <fstream>
#include <iostream>
#include <vector>

std::vector<int> readlines() {
  std::ifstream file("assets/input1.txt", std::ifstream::in);
  std::string line;
  std::vector<int> result;
  while (std::getline(file, line)) {
    result.push_back(std::stoi(line));
  }
  return result;
}

int main() {
  const auto depths = readlines();
  int num_increases{};

  // Part 1
  for (auto it = depths.begin() + 1; it != depths.end(); ++it) {
    if (*it > *(it - 1)) num_increases++;
  }
  std::cout << "Part 1: " << num_increases << std::endl;

  // Part 2
  num_increases = 0;
  for (auto it = depths.begin() + 3; it != depths.end(); ++it) {
    // Two consecutive windows share two numbers, so we only need to compare the two remaining ones.
    if (*it > *(it - 3)) num_increases++;
  }
  std::cout << "Part 2: " << num_increases << std::endl;
}