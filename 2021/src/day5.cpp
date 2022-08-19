#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

using Point2D = std::pair<int, int>;
using PointSet = std::vector<Point2D>;

int sgn(const int x) { return x ? x / abs(x) : x; }

std::pair<PointSet, PointSet> parseinput() {
  std::ifstream file("assets/input5.txt", std::ifstream::in);
  std::string line;
  PointSet points;
  PointSet points2;  // With diagonals
  while (std::getline(file, line)) {
    std::stringstream ss{line};
    int x1, y1, x2, y2;
    char _skip;
    ss >> x1 >> _skip >> y1 >> _skip >> _skip >> x2 >> _skip >> y2;
    const int dx{sgn(x2 - x1)}, dy{sgn(y2 - y1)};
    while (x1 != x2 || y1 != y2) {
      points2.push_back({x1, y1});
      if (!dx || !dy) points.push_back({x1, y1});
      x1 += dx;
      y1 += dy;
    }
    points2.push_back({x1, y1});
    if (!dx || !dy) points.push_back({x1, y1});
  }

  return {points, points2};
}

unsigned int num_overlaps(const PointSet& points) {
  std::map<Point2D, size_t> freqs;
  for (const auto& p : points) freqs[p] += 1;
  return std::count_if(freqs.cbegin(), freqs.cend(), [](const auto kv) { return kv.second > 1; });
}

int main() {
  const auto [points1, points2] = parseinput();
  std::cout << "Part 1: " << num_overlaps(points1) << std::endl;
  std::cout << "Part 2: " << num_overlaps(points2) << std::endl;
}
