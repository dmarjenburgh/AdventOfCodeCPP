#include <compare>
#include <iostream>

struct Point {
  int x;
  int y;

  Point& operator+=(const Point& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  auto operator<=>(const Point&) const = default;
};

Point operator+(const Point& lhs, const Point& rhs) { return Point{lhs.x + rhs.x, lhs.y + rhs.y}; }

std::ostream& operator<<(std::ostream& os, const Point& point) {
  os << '(' << point.x << "," << point.y << ')';
  return os;
}

