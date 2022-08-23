#include <compare>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <vector>

using Grid = std::vector<std::vector<int>>;

// TODO: Re-use Point struct from day 11
struct Point {
  int x{};
  int y{};

  Point() = default;
  Point(const Point& point) = default;
  Point(int x, int y) : x(x), y(y) {}

  Point& operator+=(const Point& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }
  friend Point operator+(const Point& lhs, const Point& rhs);
  auto operator<=>(const Point&) const = default;
  friend std::ostream& operator<<(std::ostream& os, const Point& point) {
    return os << '(' << point.x << ',' << point.y << ')';
  }
};

template <>
struct std::hash<Point> {
  std::size_t operator()(const Point& point) const noexcept {
    std::size_t h1 = std::hash<int>{}(point.x);
    std::size_t h2 = std::hash<int>{}(point.y);
    return h1 ^ (h2 << 1);
  }
};

struct Field {
  Grid g;
  int width{};
  int height{};

  [[nodiscard]] std::vector<Point> neighbours(const Point& point) const {
    std::vector<Point> nbs;
    for (const Point& dp : {Point(0, -1), Point(-1, 0), Point(0, 1), Point(1, 0)}) {
      const Point npoint = point + dp;
      if (npoint.x >= 0 and npoint.x < width and npoint.y >= 0 and npoint.y < height) {
        nbs.emplace_back(npoint);
      }
    }
    return nbs;
  }
  int operator[](const Point& point) const { return g[point.y][point.x]; }
  friend std::istream& operator>>(std::istream& is, Field& field);
  friend std::ostream& operator<<(std::ostream& os, const Field& field);
};

std::ostream& operator<<(std::ostream& os, const Field& field) {
  for (int row{0}; row < field.height; ++row) {
    for (int col{0}; col < field.width; ++col) {
      os << field.g[row][col];
    }
    os << '\n';
  }
  return os;
}

std::istream& operator>>(std::istream& is, Field& field) {
  char ch;
  int h{};

  std::vector<int> row;
  while (is >> std::noskipws >> ch) {
    if (ch == '\n') {
      h++;
      field.g.push_back(row);
      row = {};
      continue;
    }
    row.push_back(ch - '0');
  }
  if (!row.empty()) {
    h++;
    field.g.push_back(row);
  }

  field.height = h;
  field.width = static_cast<int>(field.g[0].size());
  return is;
}

Point operator+(const Point& lhs, const Point& rhs) { return Point{lhs.x + rhs.x, lhs.y + rhs.y}; }

template <class Node, class CostFunction, class Neighbours, class Heuristic, class CostType = int>
CostType a_star(const Node& start, const Node& goal, const CostFunction& cost_function,
                const Neighbours& neighbours, const Heuristic& h) {
  // First element is the priority, so default ordering for pairs works
  using PQPair = std::pair<CostType, Node>;

  std::priority_queue<PQPair, std::vector<PQPair>, std::greater<>> frontier;
  std::unordered_map<Node, Node> came_from;
  std::unordered_map<Node, CostType> cost_so_far;

  frontier.emplace(CostType{}, start);
  came_from[start] = start;  // Need to fill it to prevent revisiting the start location
  cost_so_far[start] = CostType{};

  while (!frontier.empty()) {
    const auto [prio, current] = frontier.top();
    if (current == goal) {
      return cost_so_far[goal];
    } else {
      frontier.pop();  // This is why current can't be a reference
      for (const auto nb : neighbours(current)) {
        CostType new_cost{cost_so_far[current] + cost_function(nb)};
        if (!came_from.contains(nb) or new_cost < cost_so_far.at(nb)) {
          cost_so_far[nb] = new_cost;
          frontier.emplace(new_cost + h(nb), nb);
          came_from[nb] = current;
        }
      }
    }
  }

  return cost_so_far[goal];
}

int main() {
  std::ifstream file{"assets/input15.txt"};
  Field f;
  file >> f;

  const Point start{0, 0}, goal{f.width - 1, f.height - 1};
  const int lowest_risk1 = a_star(
      start, goal, [&f](const Point& p) { return f[p]; },
      [&f](const Point& p) { return f.neighbours(p); },
      [&goal](const Point& p) { return std::abs(goal.x - p.x) + std::abs(goal.y - p.y); });

  std::printf("Part 1: %i\n", lowest_risk1);

  // Dangerous, but will work with adapted cost function f.height *= 5;
  int orig_w{f.width}, orig_h{f.height};
  f.width *= 5;
  f.height *= 5;
  const Point goal2{f.width - 1, f.height - 1};
  const auto cost_fn2 = [&f, orig_w, orig_h](const Point& p) {
    int gx{p.x / orig_w}, gy{p.y / orig_h};
    int grid_dist{gx + gy};
    int cost{f[{p.x % orig_w, p.y % orig_h}] + grid_dist};
    if (cost > 9) cost -= 9;
    return cost;
  };

  const int lowest_risk2 = a_star(
      start, goal2, cost_fn2, [&f](const Point& p) { return f.neighbours(p); },
      [&goal2](const Point& p) { return std::abs(goal2.x - p.x) + std::abs(goal2.y - p.y); });
  std::printf("Part 2: %i\n", lowest_risk2);

}
