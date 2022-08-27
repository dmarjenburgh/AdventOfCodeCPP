#ifndef ADVENTOFCODE_MATRIX_HPP
#define ADVENTOFCODE_MATRIX_HPP
#include <compare>
#include <functional>

struct Point {
  int x;
  int y;

  auto operator<=>(const Point&) const = default;

  friend Point operator+(const Point& lhs, const Point& rhs);
  friend std::ostream& operator<<(std::ostream& os, const Point& point);
};

namespace std {
template <>
struct hash<Point> {
  size_t operator()(const Point& point) const {
    size_t h1 = std::hash<int>{}(point.x);
    size_t h2 = std::hash<int>{}(point.y);
    return h1 ^ (h2 << 1);
  }
};
}  // namespace std
#endif  // ADVENTOFCODE_MATRIX_HPP
