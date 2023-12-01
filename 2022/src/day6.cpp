#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

int find_marker(const std::string_view &line, int num_chars) {
  int i{};
  for (auto it{line.begin()}; it != line.end() - num_chars + 1; ++it, ++i) {
    std::unordered_set<char> chars{it, it + num_chars};
    if (chars.size() == num_chars)
      break;
  }
  return i + num_chars;
}

int main() {
  std::string input;
  std::fstream f{"assets/input6.txt"};
  std::getline(f, input);

  std::cout << "Part 1: " << find_marker(input, 4) << '\n';
  std::cout << "Part 2: " << find_marker(input, 14) << '\n';
}
