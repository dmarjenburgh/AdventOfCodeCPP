#include <array>
#include <compare>
#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>

struct Point {
  int x;
  int y;

  Point() : x(0), y(0) {}
  Point(int _x, int _y) : x(_x), y(_y) {}

  Point& operator+=(const Point& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  auto operator<=>(const Point&) const = default;

  friend Point operator+(const Point& lhs, const Point& rhs);

  friend std::ostream& operator<<(std::ostream& os, const Point& point);
};

Point operator+(const Point& lhs, const Point& rhs) { return Point{lhs.x + rhs.x, lhs.y + rhs.y}; }
template <>

struct std::hash<Point> {
  std::size_t operator()(const Point& point) const noexcept {
    std::size_t h1 = std::hash<int>{}(point.x);
    std::size_t h2 = std::hash<int>{}(point.y);
    return h1 ^ (h2 << 1);
  }
};

using Field = std::array<std::array<int, 10>, 10>;

class Grid {
  Field field;
  size_t width;
  size_t height;

 public:
  [[nodiscard]] std::vector<Point> neighbours(const Point& point) const {
    std::vector<Point> nbs;
    for (const int dx : {-1, 0, 1}) {
      for (const int dy : {-1, 0, 1}) {
        const Point npoint = point + Point(dx, dy);
        if ((dx or dy) and npoint.x >= 0 and npoint.x < width and npoint.y >= 0 and
            npoint.y < height) {
          nbs.emplace_back(npoint);
        }
      }
    }
    return nbs;
  }

  int& operator[](const Point& point) { return field[point.y][point.x]; }

  [[nodiscard]] std::vector<Point> points() const {
    std::vector<Point> r;
    for (int row{0}; row < height; ++row) {
      for (int col{0}; col < width; ++col) {
        r.emplace_back(col, row);
      }
    }
    return r;
  }

  friend std::istream& operator>>(std::istream& is, Grid& grid);
  friend std::ostream& operator<<(std::ostream& os, const Grid& grid);
};

std::istream& operator>>(std::istream& is, Grid& grid) {
  char ch;
  for (int row{0}; row < 10; ++row) {
    for (int col{0}; col < 10; ++col) {
      is >> ch;
      grid.field[row][col] = ch - '0';
    }
  }
  grid.width = 10;
  grid.height = 10;
  return is;
}

std::ostream& operator<<(std::ostream& os, const Grid& grid) {
  for (int row{0}; row < 10; ++row) {
    for (int col{0}; col < 10; ++col) {
      os << grid.field[row][col];
    }
    os << std::endl;
  }
  return os;
}

size_t step(Grid& grid) {
  std::unordered_set<Point> flashed;
  // Using a deque instead of a queue, because a queue is not a basic container, but a container
  // adapter and has very limited functionality.
  std::deque<Point> q;
  const auto gps{grid.points()};
  std::copy(std::make_move_iterator(gps.begin()), std::make_move_iterator(gps.end()),
            std::back_inserter(q));

  int newflashes{};
  while (!q.empty()) {
    const Point p{q.front()};
    q.pop_front();
    if (!flashed.contains(p)) grid[p]++;
    if (grid[p] > 9) {
      grid[p] = 0;
      flashed.insert(p);
      newflashes++;
      auto nbs = grid.neighbours(p);
      for (const Point& nb : nbs) {
        if (!flashed.contains(nb)) q.push_back(nb);
      }
    }
  }
  return flashed.size();
}

int main() {
  std::ifstream file{"assets/input11.txt"};
  Grid grid{};
  file >> grid;

  size_t num_flashes{};
  for (int n{0}; n < 100; ++n) {
    num_flashes += step(grid);
  }
  std::printf("Part 1: %lu\n", num_flashes);
  for (int n{100};; ++n) {
    if (step(grid) == 100) {
      std::printf("Part 2: %i\n", n + 1);
      break;
    }
  }
}
