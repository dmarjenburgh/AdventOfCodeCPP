#include <fstream>
#include <iostream>
#include <regex>
#include <string>

int add_numbers(const std::string &s) {
  std::regex digits{"-?\\d+"};
  auto db = std::sregex_iterator(s.begin(), s.end(), digits);
  auto dbe = std::sregex_iterator();
  int total{};
  for (std::sregex_iterator i = db; i != dbe; ++i) {
    std::string x{i->str()};
    total += std::stoi(x);
  }
  return total;
}

int main() {

  std::ifstream input_file{"assets/input12.txt"};
  std::string line;
  std::getline(input_file, line);
  input_file.close();

  int total = add_numbers(line);

  int level{};
  int flagged_lvl{-1};
  enum class container { ARRAY, OBJECT };
  std::stack<std::pair<container, int>> containers{};
  std::vector<std::pair<int, int>> skips{};
  for (int i{}; i < line.length(); ++i) {
    if (line[i] == '{') {
      containers.push({container::OBJECT, i});
      ++level;
    } else if (line[i] == '[') {
      containers.push({container::ARRAY, i});
    } else if (line[i] == ']') {
      containers.pop();
    } else if (line[i] == '}') {
      std::pair<container, int> &start = containers.top();
      if (level == flagged_lvl) {
        skips.emplace_back(start.second, i);
        std::fill(&line[start.second], &line[i + 1], ' ');
        flagged_lvl = -1;
      }
      containers.pop();
      --level;
    } else if (line[i] == 'r' && line[i + 1] == 'e' && line[i + 2] == 'd') {
      if (containers.top().first == container::OBJECT && flagged_lvl < 0) {
        flagged_lvl = level;
      }
    }
  }

  std::cout << "Part 1: " << total << std::endl;
  std::cout << "Part 2: " << add_numbers(line) << std::endl;
}
