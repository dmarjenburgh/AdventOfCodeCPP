#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

using CrabPositions = std::vector<int>;

CrabPositions parseinput() {
  std::ifstream file("assets/input7.txt", std::ifstream::in);
  std::string num;
  CrabPositions positions;
  while (std::getline(file, num, ',')) {
    positions.push_back(std::stoi(num));
  }
  return positions;
}

template <class DistanceFunction>
int fuel_cost(const CrabPositions& p, const int align_pos, DistanceFunction dist) {
  return std::accumulate(
      p.cbegin(), p.cend(), 0,
      [&align_pos, &dist](const auto& acc, const auto& x) { return acc + dist(x, align_pos); });
}

template <class DistanceFunction>
int least_fuel_cost(const CrabPositions& p, DistanceFunction dist) {
  const auto [min_pos, max_pos] = std::minmax_element(p.cbegin(), p.cend());
  std::vector<int> distances(*max_pos - *min_pos);
  std::iota(distances.begin(), distances.end(), *min_pos);
  std::vector<int> costs(distances.size());
  std::transform(distances.cbegin(), distances.cend(), costs.begin(),
                 [&p, &dist](const int& d) { return fuel_cost(p, d, dist); });
  const auto min_fuel = std::min_element(costs.cbegin(), costs.cend());
  const auto best_pos_idx = std::distance(costs.cbegin(), min_fuel);
  return *min_fuel;
}

int main() {
  auto positions = parseinput();
  std::printf("Part 1: %i\n", least_fuel_cost(positions, [](int a, int b) { return abs(b - a); }));
  std::printf("Part 2: %i\n", least_fuel_cost(positions, [](int a, int b) {
                const int n{abs(b - a)};
                return n * (n + 1) / 2;
              }));
}
