#include "lib/IOUtils.hpp"
#include <iostream>
#include <numeric>
#include <ranges>
#include <regex>
#include <string>
#include <tuple>

struct Draw {
  int red{};
  int green{};
  int blue{};
};

struct Game {
  int id{};
  std::vector<Draw> draws;
};

const std::unordered_map<std::string, int Draw::*> colorMap{
    {"red", &Draw::red}, {"green", &Draw::green}, {"blue", &Draw::blue}};

Draw parseRGB(const std::string_view s) {
  std::regex re{R"((\d+) (\w+))"};
  using svi = std::string_view::iterator;
  auto mbegin = std::regex_iterator<svi>(s.cbegin(), s.cend(), re);
  auto mend = std::regex_iterator<svi>();

  Draw d;
  for (auto it{mbegin}; it != mend; ++it) {
    const std::string clr{(*it)[2]};
    const int n{std::stoi((*it)[1])};

    d.*(colorMap.at(clr)) = n;
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
  auto members = std::tuple(&Draw::red, &Draw::green, &Draw::blue);

  int minPower = 1;
  std::apply([&](auto... member) {
    ((minPower *= std::ranges::max(g.draws, {}, [member](auto&& draw) { return draw.*member; }).*member), ...);
  }, members);

  return minPower;
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
