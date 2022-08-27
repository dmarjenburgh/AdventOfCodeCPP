#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <stack>
#include <vector>
#include "lib/matrix.hpp"

using Grid = std::vector<std::vector<int>>;

struct HeightMap {
  Grid grid;
  size_t width{};
  size_t height{};

  int operator[](const Point& point) const { return grid[point.y][point.x]; }

  [[nodiscard]] std::vector<Point> neighbours(const Point& point) const {
    std::vector<Point> ns;
    if (point.x > 0) ns.emplace_back(Point{point.x - 1, point.y});
    if (point.y > 0) ns.emplace_back(Point{point.x, point.y - 1});
    if (point.x < width - 1) ns.emplace_back(Point{point.x + 1, point.y});
    if (point.y < height - 1) ns.emplace_back(Point{point.x, point.y + 1});
    return ns;
  }
  [[nodiscard]] std::vector<Point> points() const {
    std::vector<Point> p;
    for (int row{0}; row < height; ++row) {
      for (int col{0}; col < width; ++col) {
        p.emplace_back(Point{col, row});
      }
    }
    return p;
  }
};

std::istream& operator>>(std::istream& is, HeightMap& map) {
  char ch;
  std::vector<int> row;
  while (is >> std::noskipws >> ch) {
    if (std::isspace(ch)) {
      map.grid.push_back(row);
      map.width = row.size();
      map.height++;
      row = {};
    } else {
      row.push_back(ch - '0');
    }
  }
  return is;
}

size_t flood_fill(const HeightMap& h, std::set<Point>& visited, const Point& starting_point) {
  std::stack<Point> stack;
  stack.push(starting_point);
  int basin_size{};
  while (!stack.empty()) {
    const Point p{stack.top()};
    stack.pop();
    if (visited.contains(p)) continue;
    visited.insert(p);
    basin_size++;
    for (const Point& nb : h.neighbours(p)) {
      if (!visited.contains(nb) and h[nb] != 9) {
        stack.push(nb);
      }
    };
  }
  return basin_size;
}

HeightMap parseinput() {
    std::ifstream file{"assets/input9.txt"};
//  std::stringstream file{"2199943210\n3987894921\n9856789892\n8767896789\n9899965678\n"};
  HeightMap h;
  file >> h;
  return h;
}

bool is_low_point(const HeightMap& h, const Point& p) {
  const auto neighbours{h.neighbours(p)};
  const int height{h[p]};
  return std::all_of(neighbours.cbegin(), neighbours.cend(),
                     [height, &h](const auto& q) { return height < h[q]; });
}

int main() {
  auto height_map = parseinput();
  int risk_score{};
  for (const Point& p : height_map.points()) {
    if (is_low_point(height_map, p)) {
      risk_score += height_map[p] + 1;
    }
  }
  std::printf("Part 1: %i\n", risk_score);

  std::set<Point> visited;
  std::vector<size_t> basin_sizes;
  for (const Point& p : height_map.points()) {
    if (!visited.contains(p) and height_map[p] != 9) {
      const size_t bs{flood_fill(height_map, visited, p)};
      basin_sizes.push_back(bs);
    }
  }
  std::sort(basin_sizes.begin(), basin_sizes.end(), std::greater());
  std::printf("Part 2: %lu\n", basin_sizes[0] * basin_sizes[1] * basin_sizes[2]);
}
