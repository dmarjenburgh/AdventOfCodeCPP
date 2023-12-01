#include <array>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <unordered_map>

using distmap = std::unordered_map<std::string, int>;

template <std::size_t SIZE>
int distance(const std::array<std::string, SIZE> &cities, distmap &distances) {
  int total{};
  for (const auto *p = cities.begin() + 1; p != cities.end(); ++p)
    total += distances[*(p - 1) + *p];
  return total;
}

int shortest_distance{INT_MAX};
int longest_distance{0};

template <std::size_t SIZE>
void permute(std::array<std::string, SIZE> &cities, int k, distmap &m) {
  if (k == SIZE - 1) {
    int d = distance(cities, m);
    if (d < shortest_distance)
      shortest_distance = d;
    if (d > longest_distance)
      longest_distance = d;
    return;
  }
  for (int i = k; i < SIZE; ++i) {
    std::swap(cities[k], cities[i]);
    permute(cities, k + 1, m);
    std::swap(cities[k], cities[i]);
  }
}

int main() {
  std::ifstream input_file{"assets/input9.txt"};
  std::string line{};
  std::unordered_map<std::string, int> dists;
  std::array<std::string, 8> cities{};
  int i{};
  while (std::getline(input_file, line)) {
    std::istringstream s{line};
    std::string c1, to, c2, eq;
    int dist;
    s >> c1 >> to >> c2 >> eq >> dist;
    std::string k1{};
    dists[c1 + c2] = dist;
    dists[c2 + c1] = dist;
    if (c1 != cities[i - 1])
      cities[i++] = c1;
    if (i == 7)
      cities[i] = c2;
  }

  permute(cities, 0, dists);
  std::printf("Part 1: %d\n", shortest_distance);
  std::printf("Part 2: %d\n", longest_distance);
}
