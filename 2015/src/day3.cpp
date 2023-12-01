#include <fstream>
#include <iostream>
#include <map>

using pos = std::pair<int, int>;

static size_t part1(const std::string &line) {
  int x{}, y{};
  std::map<pos, int> houses;
  houses[{x, y}] = 1;
  for (char c : line) {
    if (c == '^')
      y += 1;
    else if (c == '>')
      x += 1;
    else if (c == 'v')
      y -= 1;
    else if (c == '<')
      x -= 1;

    if (!houses[{x, y}])
      houses[{x, y}] = 0;
    houses[{x, y}] += 1;
  }
  return houses.size();
}

static size_t part2(const std::string &line) {
  int sx{}, sy{}, rx{}, ry{};
  std::map<pos, int> houses;
  houses[{sx, sy}] = 2;
  for (int i = 0; i < line.size(); i++) {
    const char c = line[i];
    int &x = i % 2 == 0 ? sx : rx;
    int &y = i % 2 == 0 ? sy : ry;
    if (c == '^')
      y += 1;
    else if (c == '>')
      x += 1;
    else if (c == 'v')
      y -= 1;
    else if (c == '<')
      x -= 1;

    if (!houses[{x, y}])
      houses[{x, y}] = 0;
    houses[{x, y}] += 1;
  }
  return houses.size();
}

int main() {
  std::string line;
  std::ifstream input_file{"assets/input3.txt"};
  std::getline(input_file, line);
  input_file.close();

  size_t numHouses = part1(line);
  std::printf("Part 1: %lu\n", numHouses);
  numHouses = part2(line);
  std::printf("Part 2: %lu\n", numHouses);
}
