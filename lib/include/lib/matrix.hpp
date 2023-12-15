#ifndef ADVENTOFCODE_MATRIX_HPP
#define ADVENTOFCODE_MATRIX_HPP
#include <compare>
#include <functional>

struct Point {
  int x;
  int y;

    friend Point operator+(const Point& lhs, const Point& rhs);
    friend Point operator-(const Point& lhs, const Point& rhs);
    Point& operator+=(const Point& rhs);
    Point& operator-=(const Point& rhs);
    Point operator-() const;
  auto operator<=>(const Point&) const = default;
};

template <>
struct std::hash<Point> {
  std::size_t operator()(const Point& point) const noexcept;
};
#endif  // ADVENTOFCODE_MATRIX_HPP
