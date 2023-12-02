#include "lib/IOUtils.hpp"
#include <iostream>
#include <numeric>
#include <ranges>
#include <regex>
#include <string>

struct Draw {
  int red{};
  int green{};
  int blue{};
};

struct Game {
  int id{};
  std::vector<Draw> draws;
};

Draw parseRGB(const std::string_view s) {
  std::regex re{R"((\d+) (\w+))"};
  using svi = std::string_view::iterator;
  auto mbegin = std::regex_iterator<svi>(s.cbegin(), s.cend(), re);
  auto mend = std::regex_iterator<svi>();
  std::array<int, 3> result;
  Draw d;
  for (auto it{mbegin}; it != mend; ++it) {
    const std::string clr{(*it)[2]};
    const int n{std::stoi((*it)[1])};
    if (clr == "red") {
      d.red = n;
    } else if (clr == "green") {
      d.green = n;
    } else if (clr == "blue") {
      d.blue = n;
    } else {
      throw std::runtime_error("Unknown colour: " + clr);
    }
  }
  return d;
}

Game parseGame(const std::string &s) {
  std::regex game_re{R"(Game (\d+): )"};
  std::smatch sm;
  std::regex_search(s, sm, game_re);
  int id{std::stoi(sm[1])};
  std::regex re{R"([^;]+)"};
  auto mbegin = std::sregex_iterator(s.begin() + sm.length(), s.end(), re);
  auto mend = std::sregex_iterator();
  std::vector<Draw> draws;
  for (auto it{mbegin}; it != mend; ++it) {
    draws.push_back(parseRGB(it->str()));
  }
  return Game{.id = id, .draws = draws};
}

bool possibleGame(const Game &g) {
  return std::ranges::all_of(g.draws, [](auto &&draw) {
    return draw.red <= 12 && draw.green <= 13 && draw.blue <= 14;
  });
}

int minimumPower(const Game &g) {
  int min_red =
      std::ranges::max(g.draws, {}, [](auto &&d) { return d.red; }).red;
  int min_green =
      std::ranges::max(g.draws, {}, [](auto &&d) { return d.green; }).green;
  int min_blue =
      std::ranges::max(g.draws, {}, [](auto &&d) { return d.blue; }).blue;
  return min_red * min_green * min_blue;
}

int main() {
  const auto lines{ReadAllLines("assets/input2.txt")};
  int sum{}, sum2{};
  for (const auto &item : lines) {
    const Game g{parseGame(item)};
    if (possibleGame(g)) {
      sum += g.id;
    }
    sum2 += minimumPower(g);
  }
  std::cout << "Part 1: " << sum << '\n';
  std::cout << "Part 2: " << sum2 << '\n';
}
