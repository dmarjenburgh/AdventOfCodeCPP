#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Storing the cave ids as ints for performance. String checking was a bit too
// slow.
using Pos = int;
using Path = std::deque<Pos>;
enum { START = 0, END = -1 };

class Caves {
  std::unordered_map<Pos, std::vector<Pos>> g;

public:
  [[nodiscard]] const std::vector<Pos> &connected_caves(const Pos &c) const {
    return g.at(c);
  }
  friend std::istream &operator>>(std::istream &, Caves &);
  friend std::ostream &operator<<(std::ostream &os, const Caves &caves);
};

std::istream &operator>>(std::istream &is, Caves &caves) {
  std::string k, v;
  int id{};
  std::unordered_map<std::string, Pos> m = {{"start", START}, {"end", END}};
  while (std::getline(is, k, '-')) {
    std::getline(is, v);
    if (!m.contains(k))
      m[k] = ++id + (islower(k[0]) ? 0 : 100); // Big caves have id > 100
    if (!m.contains(v))
      m[v] = ++id + (islower(v[0]) ? 0 : 100);

    int ki{m.at(k)}, vi{m.at(v)};
    caves.g[ki].push_back(vi);
    caves.g[vi].push_back(ki);
  }
  return is;
}

// std::ostream& operator<<(std::ostream& os, const Caves& caves) {
//   for (const auto& [k, v] : caves.g) {
//     os << k << " <-> ";
//     for (const auto& x : v) {
//       os << x << ' ';
//     }
//     os << '\n';
//   }
//   return os;
// }

size_t recursive_pathfinding(const Caves &caves, Path &path,
                             bool can_revisit = false) {
  const auto &curPos{path.back()};
  if (curPos == END)
    return 1;
  size_t num_paths{};
  for (const auto &nb : caves.connected_caves(curPos)) {
    bool next_revisit{can_revisit};
    if (nb < 100) {
      // Check if visit is possible for small cave
      if (nb == START)
        continue;
      const bool already_visited{std::find(path.cbegin(), path.cend(), nb) !=
                                 std::end(path)};
      if (already_visited and !can_revisit)
        continue;
      if (already_visited)
        next_revisit = false;
    }
    path.push_back(nb);
    num_paths += recursive_pathfinding(caves, path, next_revisit);
    path.pop_back();
  }
  return num_paths;
}

// Checked manually that no two big caves are connected, so you don't get into
// infinite loops
int main() {
  std::ifstream file{"assets/input12.txt"};
  Caves caves;
  file >> caves;
  Path path = {START};
  std::printf("Part 1: %lu\n", recursive_pathfinding(caves, path));
  std::printf("Part 2: %lu\n", recursive_pathfinding(caves, path, true));
}
