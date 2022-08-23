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
  std::size_t operator()(const Point& point) const noexcept { return point.x ^ (point.y << 1); }
};

struct Field {
  Grid g;
  int width{};
  int height{};

  [[nodiscard]] std::vector<Point> neighbours(const Point& point) {
    if (!_nbs_cache[point.y][point.x].empty()) {
      return _nbs_cache[point.y][point.x];
    }

    std::vector<Point> nbs;
    nbs.reserve(4); // Prevent memory re-allocations
    for (const Point& dp : {Point(0, -1), Point(-1, 0), Point(0, 1), Point(1, 0)}) {
      const Point npoint = point + dp;
      if (npoint.x >= 0 and npoint.x < width and npoint.y >= 0 and npoint.y < height) {
        nbs.emplace_back(npoint);
      }
    }
    _nbs_cache[point.y][point.x] = nbs;
    return nbs;
  }

  void init_neighbours_cache() {
    _nbs_cache = std::vector<std::vector<std::vector<Point>>>(
        height, std::vector<std::vector<Point>>(width));
  }

  int operator[](const Point& point) const { return g[point.y][point.x]; }
  friend std::istream& operator>>(std::istream& is, Field& field);
  friend std::ostream& operator<<(std::ostream& os, const Field& field);

 private:
  std::vector<std::vector<std::vector<Point>>> _nbs_cache;
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

template <class CostFunction, class Heuristic, class CostType = int>
CostType a_star(Field& f, const Point& start, const Point& goal, const CostFunction& cost_function,
                const Heuristic& h) {
  // First element is the priority, so default ordering for pairs works
  using PQPair = std::pair<CostType, Point>;

  std::vector<std::vector<CostType>> cost(
      f.height, std::vector<CostType>(f.width, std::numeric_limits<CostType>::max()));
  std::priority_queue<PQPair, std::vector<PQPair>, std::greater<>> frontier;
  std::vector<std::vector<Point>> came_from(f.height, std::vector<Point>(f.width, Point()));

  cost[start.y][start.x] = 0;
  frontier.emplace(CostType{}, start);
  came_from[start.y][start.x] = start;  // Need to fill it to prevent revisiting the start location

  while (!frontier.empty()) {
    const auto [_, current] = frontier.top();
    frontier.pop();  // This is why current can't be a reference
    if (current == goal) {
      return cost[goal.y][goal.x];
    } else {
      for (const Point& nb : f.neighbours(current)) {
        CostType new_cost{cost[current.y][current.x] + cost_function(nb)};
        if (new_cost < cost[nb.y][nb.x]) {
          cost[nb.y][nb.x] = new_cost;
          frontier.emplace(new_cost + h(nb, goal), nb);
          came_from[nb.y][nb.x] = current;
        }
      }
    }
  }

  return cost[goal.y][goal.x];
}

int manhattan(const Point& a, const Point& b) { return std::abs(b.x - a.x) + std::abs(b.y - a.y); }

int main() {
  std::ifstream file{"assets/input15.txt"};
  Field f;
  file >> f;
  f.init_neighbours_cache();

  const Point start{0, 0}, goal{f.width - 1, f.height - 1};
  const int lowest_risk1 = a_star(
      f, start, goal, [&f](const Point& p) { return f[p]; }, manhattan);

  std::printf("Part 1: %i\n", lowest_risk1);

  // Dangerous, but will work with adapted cost function f.height *= 5;
  int orig_w{f.width}, orig_h{f.height};
  f.width *= 5;
  f.height *= 5;
  f.init_neighbours_cache();

  const Point goal2{f.width - 1, f.height - 1};
  const auto cost_fn2 = [&f, orig_w, orig_h](const Point& p) {
    int gx{p.x / orig_w}, gy{p.y / orig_h};
    int cost{f[{p.x % orig_w, p.y % orig_h}] + gx + gy};
    if (cost > 9) cost -= 9;
    return cost;
  };

  const int lowest_risk2 = a_star(f, start, goal2, cost_fn2, manhattan);
  std::printf("Part 2: %i\n", lowest_risk2);
}
