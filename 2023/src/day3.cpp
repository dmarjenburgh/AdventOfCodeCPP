#include "lib/IOUtils.hpp"
#include "lib/matrix.hpp"
#include <iostream>
#include <numeric>
#include <regex>
#include <unordered_set>
#include <vector>

using Grid = std::vector<std::string>;

std::unordered_set<Point> filter_grid_positions(const Grid &g, std::predicate<char> auto pred) {
  std::unordered_set<Point> points;
  for (int y = 0; y < g.size(); y++) {
    for (int x = 0; x < g[y].length(); x++) {
      if (pred(g[y][x])) {
        points.insert({x, y});
      }
    }
  }
  return points;
}

int sumOfPartNumbers(const Grid &g) {
  auto is_symbol = [](char c) { return !(std::isdigit(c) || c == '.'); };
  const auto symbols{filter_grid_positions(g, is_symbol)};
  auto adjacent_to_symbol = [&symbols](int r, const std::smatch &m) {
    int min_c{static_cast<int>(m.position() - 1)};
    int max_c{static_cast<int>(m.position() + m.length())};
    for (int row : {r - 1, r + 1}) {
      for (int c{min_c}; c <= max_c; ++c) {
        if (symbols.contains({c, row})) {
          return true;
        };
      }
    }
    return (symbols.contains({min_c, r}) || symbols.contains({max_c, r}));
  };
  std::regex number_regex("\\d+");
  std::vector<int> part_numbers;
  for (int r{}; r < g.size(); ++r) {
    const std::string &line{g[r]};
    auto r_begin{std::sregex_iterator(line.begin(), line.end(), number_regex)};
    auto r_end{std::sregex_iterator()};
    for (auto it{r_begin}; it != r_end; ++it) {
      const std::smatch &m = *it;
      if (adjacent_to_symbol(r, m)) {
        part_numbers.push_back(std::stoi(m.str()));
      }
    }
  }
  return std::accumulate(part_numbers.begin(), part_numbers.end(), 0, std::plus());
}

std::tuple<int, int> getNumber(const Grid &g, int row, int col) {
  int start = col, end = col;
  while (start > 0 && isdigit(g[row][start - 1]))
    --start;
  while (end < g[row].size() - 1 && isdigit(g[row][end + 1]))
    ++end;
  return {std::stoi(g[row].substr(start, end - start + 1)), end};
}

int sumOfGearRatios(const Grid &g) {
  auto is_gear = [](char c) { return c == '*'; };
  const auto gears{filter_grid_positions(g, is_gear)};
  auto gear_value = [&g](int r, int c) {
    int gear_ratio{};
    for (int row : {r - 1, r, r + 1}) {
      for (int col{c - 1}; col <= c + 1; ++col) {
        if (std::isdigit(g[row][col])) {
          auto [n, right_i] = getNumber(g, row, col);
          if (gear_ratio) {
            return gear_ratio * n;
          } else {
            gear_ratio = n;
          }
          col = right_i;
        }
      }
    }
    return 0;
  };

  std::regex number_regex("\\d+");
  int sum{};
  for (const auto p : gears) {
    if (int n{gear_value(p.y, p.x)}; n > 0) {
      sum += n;
    }
  }
  return sum;
}

int main() {
  const auto lines{ReadAllLines("assets/input3.txt")};
  std::cout << "Part 1: " << sumOfPartNumbers(lines) << '\n';
  std::cout << "Part 2: " << sumOfGearRatios(lines) << '\n';
}
