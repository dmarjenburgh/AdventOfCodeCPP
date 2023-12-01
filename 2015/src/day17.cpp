#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#define TARGET 150

template <typename FI>
int numsubsets(const FI from, const FI to, int target, const int size,
               std::vector<int> &sols) {
  if (target == 0) {
    sols.push_back(size);
    return 1;
  }
  if (target < 0 || from == to)
    return 0;

  int first{*from};
  int left{target - first};
  int num_with_first{numsubsets(from + 1, to, left, size + 1, sols)};
  int num_without_first{numsubsets(from + 1, to, target, size, sols)};
  return num_with_first + num_without_first;
}

int main() {
  std::ifstream input_file{"assets/input17.txt"};
  std::vector<int> sizes;
  std::string line;
  while (std::getline(input_file, line)) {
    sizes.push_back(std::stoi(line));
  }
  std::vector<int> sols;
  const int num_combinations{
      numsubsets(sizes.begin(), sizes.end(), TARGET, 0, sols)};
  const int min_size{*std::min_element(sols.begin(), sols.end())};
  const int amount = std::count(sols.begin(), sols.end(), min_size);

  std::cout << "Part 1: " << num_combinations << std::endl;
  std::cout << "Part 2: " << amount << std::endl;
}
