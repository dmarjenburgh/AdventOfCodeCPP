#include "lib/matrix.hpp"
#include <iostream>

Point Point::operator-() const {
    return Point(-x, -y);
}

Point operator+(const Point &lhs, const Point &rhs) { return Point{lhs.x + rhs.x, lhs.y + rhs.y}; }

Point operator-(const Point &lhs, const Point &rhs) { return Point{lhs.x - rhs.x, lhs.y - rhs.y}; }

Point &Point::operator+=(const Point &rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Point &Point::operator-=(const Point &rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Point &point) {
    os << '(' << point.x << "," << point.y << ')';
    return os;
}

size_t std::hash<Point>::operator()(const Point &point) const noexcept {
    size_t h1 = std::hash<int>{}(point.x);
    size_t h2 = std::hash<int>{}(point.y);
    return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
}
Point operator*(const Point & point, const int scalar){
    return Point{point.x*scalar, point.y*scalar};
}
