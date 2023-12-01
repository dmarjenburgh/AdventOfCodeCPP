#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <unordered_map>

using Map = std::unordered_map<std::string, int>;

const Map ref_aunt{{"children", 3},    {"cats", 7},   {"samoyeds", 2},
                   {"pomeranians", 3}, {"akitas", 0}, {"vizslas", 0},
                   {"goldfish", 5},    {"trees", 3},  {"cars", 2},
                   {"perfumes", 1}};

bool is_match1(const Map &m1) {
  return std::all_of(m1.begin(), m1.end(), [](auto &kv) {
    return kv.first == "nr" || ref_aunt.at(kv.first) == kv.second;
  });
}

bool is_match2(const Map &m1) {
  return std::all_of(m1.begin(), m1.end(), [](auto &kv) {
    const auto key{kv.first};
    if (key == "cats" or key == "trees")
      return kv.second > ref_aunt.at(key);
    else if (key == "pomeranians" || key == "goldfish")
      return kv.second < ref_aunt.at(key);
    else if (key == "nr")
      return true;
    else
      return ref_aunt.at(kv.first) == kv.second;
  });
}

int main() {
  std::ifstream input_file{"assets/input16.txt"};
  std::string line;
  std::regex r{R"(Sue (\d+): (\w+): (\d+), (\w+): (\d+), (\w+): (\d+))"};
  std::vector<Map> aunts;
  while (std::getline(input_file, line)) {
    std::smatch sm;
    std::regex_match(line, sm, r);
    Map m;
    m["nr"] = std::stoi(sm[1].str());
    m[sm[2].str()] = std::stoi(sm[3]);
    m[sm[4].str()] = std::stoi(sm[5]);
    m[sm[6].str()] = std::stoi(sm[7]);
    aunts.push_back(m);
  }
  const auto a1 = std::find_if(aunts.begin(), aunts.end(), &is_match1);
  std::cout << "Part 1: " << a1->at("nr") << std::endl;
  const auto a2 = std::find_if(aunts.begin(), aunts.end(), &is_match2);
  std::cout << "Part 2: " << a2->at("nr") << std::endl;
}
