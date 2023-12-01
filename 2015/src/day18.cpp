#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#define GRID_SIZE 100

using Point = std::pair<signed char, signed char>;
using Grid = std::set<Point>;

constexpr int N{GRID_SIZE - 1};
constexpr Point corners[4] = {{0, 0}, {0, N}, {N, 1}, {N, N}};

std::vector<Point> neighbours(const Grid &g, const Point &p) {
  const auto [x, y] = p;
  std::vector<Point> res;
  for (signed char dx = -1; dx < 2; ++dx)
    for (signed char dy = -1; dy < 2; ++dy) {
      const int nx{x + dx}, ny{y + dy};
      if ((dx || dy) && 0 <= nx && nx < GRID_SIZE && 0 <= ny && ny < GRID_SIZE)
        res.emplace_back(nx, ny);
    }
  return res;
}

std::map<Point, int> frequencies(const std::vector<Point> &points) {
  std::map<Point, int> res;
  for (const auto &p : points) {
    if (res.find(p) != res.end())
      ++res[p];
    else
      res.emplace(p, 1);
  }
  return res;
}

Grid step(Grid &g) {
  std::vector<Point> points{};
  points.reserve(g.size() * 2); // rough estimation
  Grid res;
  for (const auto &p : g) {
    const auto neighs{neighbours(g, p)};
    std::move(neighs.begin(), neighs.end(), std::back_inserter(points));
  }
  const auto freqs{frequencies(points)};
  for (const auto &pair : freqs)
    if (pair.second == 3 || (g.find(pair.first) != g.end() && pair.second == 2))
      res.insert(pair.first);

  return res;
}

void print_grid(const Grid &g) {
  for (int row{}; row < GRID_SIZE; ++row) {
    std::cout << '\n';
    for (int col{}; col < GRID_SIZE; ++col)
      std::cout << (g.find(Point(col, row)) != g.end() ? '#' : '.');
  }
  std::cout << std::endl;
}

int main() {
  std::ifstream input_file{"assets/input18.txt"};
  int row{};
  std::string line;
  Grid g;
  while (std::getline(input_file, line)) {
    for (int col{}; col < line.size(); ++col)
      if (line[col] == '#')
        g.emplace(col, row);
    ++row;
  }
  Grid g2 = g;
  for (int i{}; i < 100; ++i)
    g = step(g);
  std::cout << "Part 1: " << g.size() << std::endl;

  for (int i{}; i < 100; ++i) {
    g2 = step(g2);
    g2.insert(corners, corners + 4);
  }
  std::cout << "Part 2: " << g2.size() << std::endl;
}
