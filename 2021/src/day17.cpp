#include <fstream>
#include <iostream>
#include <regex>

using Range = std::pair<int, int>;

struct Region {
  Range xrange;
  Range yrange;

  friend std::istream& operator>>(std::istream& is, Region& region) {
    std::string line;
    std::regex re{R"(target area: x=(-?\d+)\.\.(-?\d+), y=(-?\d+)\.\.(-?\d+))"};
    std::smatch sm;
    std::getline(is, line);
    std::regex_match(line, sm, re);
    region.xrange = std::make_pair(std::stoi(sm[1]), std::stoi(sm[2]));
    region.yrange = std::make_pair(std::stoi(sm[3]), std::stoi(sm[4]));
    return is;
  }

  friend std::ostream& operator<<(std::ostream& os, const Region& region) {
    os << '[' << region.xrange.first << ',' << region.xrange.second << "] x ["
       << region.yrange.first << ',' << region.yrange.second << ']';
    return os;
  }
};

bool hits_target_area(const Region& r, int vx, int vy) {
  std::pair<int, int> point{0, 0};
  for (;;) {
    if (point.first > r.xrange.second) return false;
    if (point.second < r.yrange.first && vy < 0) return false;
    if (point.first >= r.xrange.first and point.first <= r.xrange.second and
        point.second >= r.yrange.first and point.second <= r.yrange.second)
      return true;
    point.first += vx;
    point.second += vy;
    if (vx > 0) vx--;
    vy--;
  }
}

std::tuple<Range, int> viable_range(const Region& r) {
  // A trajectory with vx reaches a distance of vx*(vx+1)/2
  double vx_min_d{(-1 + sqrt(1 + 8 * abs(r.xrange.first))) / 2};
  int vx_max{r.xrange.second};

  // After vy reaches 0, the y coordinates are identical to the y-values in the upward phase
  // In particular, it will always reach 0 again. vy must be smaller than the distance of the
  // furthest part of the y-range.
  int vy_max{std::max(abs(r.yrange.first), abs(r.yrange.second))};
  return {{static_cast<int>(vx_min_d), vx_max}, vy_max};
}

std::tuple<int, int> aim(const Region& region) {
  // Scan the x-range, starting with the max vy and going down until we hit the target area
  const auto [xrange, vy_max] = viable_range(region);
  const auto& [vx_min, vx_max] = xrange;
  int vy{vy_max};
  int hit_count{}, max_height{};
  while (vy >= -vy_max) {
    for (int vx{vx_min}; vx <= vx_max; ++vx) {
      if (hits_target_area(region, vx, vy)) {
        const int height{vy >= 0 ? vy * (vy + 1) / 2 : 0};
        max_height = std::max(max_height, height);
        hit_count++;
      }
    }
    vy--;
  }
  return {max_height, hit_count};
}

int main() {
  std::ifstream file{"assets/input17.txt"};
  Region region;
  file >> region;

  // Assert that the y-range does not contain 0. You could reach infinite height if it does.
  assert(region.yrange.first * region.yrange.second > 0);
  // We also require the x-range to exclude 0. Assuming that to be an invalid puzzle input :P
  assert(region.xrange.second < 0 or region.xrange.first > 0);

  // Problem is symmetric wrt y-axis. Use this to normalize the target area to the x > 0 region.
  if (region.xrange.first < 0) {
    region.xrange.first *= -1;
    region.xrange.second *= -1;
    std::swap(region.xrange.first, region.xrange.second);
  }

  const auto [max_height, hit_count] = aim(region);
  std::printf("Part 1: %i\n", max_height);
  std::printf("Part 2: %i\n", hit_count);
}
