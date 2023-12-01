#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

int calculate_happiness(const std::vector<std::string> &names,
                        std::unordered_map<std::string, int> &lookup) {
  int total{};
  for (int i{}; i < names.size() - 1; ++i) {
    total += lookup[names[i] + names[i + 1]] + lookup[names[i + 1] + names[i]];
  }
  return total + lookup[names[names.size() - 1] + names[0]] +
         lookup[names[0] + names[names.size() - 1]];
}

int main() {
  std::ifstream input_file{"assets/input13.txt"};
  std::unordered_map<std::string, int> rels;
  std::string line;
  std::regex r{
      R"((\w+) would (\w+) (\d+) happiness units by sitting next to (\w+)\.)"};
  std::vector<std::string> names{};
  while (std::getline(input_file, line)) {
    std::smatch sm;
    std::regex_match(line, sm, r);
    std::string name{sm[1].str()};
    rels[name + sm[4].str()] = (sm[2] == "gain" ? 1 : -1) * std::stoi(sm[3]);
    if (names.empty() || names[names.size() - 1] != name)
      names.emplace_back(name);
  }
  int happiness{};
  do {
    happiness = std::max(happiness, calculate_happiness(names, rels));
  } while (std::next_permutation(names.begin() + 1, names.end()));

  names.emplace_back("Me");
  for (const auto &n : names) {
    rels["Me" + n] = 0;
    rels[n + "Me"] = 0;
  }
  std::cout << "Part 1: " << happiness << std::endl;
  happiness = 0;
  do {
    happiness = std::max(happiness, calculate_happiness(names, rels));
  } while (std::next_permutation(names.begin() + 1, names.end()));
  std::cout << "Part 2: " << happiness << std::endl;
}
