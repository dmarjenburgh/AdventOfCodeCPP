#include <compare>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <unordered_set>
#include <vector>

// Use a sparse representation for the dots
struct Dot {
  Dot(int x, int y) : x(x), y(y) {}
  int x{};
  int y{};

  auto operator<=>(const Dot&) const = default;
};

struct DotHash {
  size_t operator()(const Dot& d) const {
    std::size_t h1 = std::hash<int>{}(d.x);
    std::size_t h2 = std::hash<int>{}(d.y);
    return h1 ^ (h2 << 1);
  }
};

struct FoldInstruction {
  enum class FoldDirection { horizontal, vertical };
  FoldDirection dir;
  int distance;
  FoldInstruction(FoldDirection dir, int distance) : dir(dir), distance(distance) {}
};

class Paper {
  std::unordered_set<Dot, DotHash> dots;
  std::vector<FoldInstruction> fold_instructions;

  void doFold(const FoldInstruction& fi) {
    std::unordered_set<Dot, DotHash> newdots;
    for (const auto& dot : dots) {
      if (fi.dir == FoldInstruction::FoldDirection::horizontal and dot.y > fi.distance) {
        Dot new_dot{dot.x, 2 * fi.distance - dot.y};
        newdots.insert(new_dot);
      } else if (fi.dir == FoldInstruction::FoldDirection::vertical and dot.x > fi.distance) {
        Dot new_dot{2 * fi.distance - dot.x, dot.y};
        newdots.insert(new_dot);
      } else {
        newdots.insert(dot);
      }
    }
    std::swap(dots, newdots);
  }

 public:
  friend std::istream& operator>>(std::istream& is, Paper& paper);
  friend std::ostream& operator<<(std::ostream& os, const Paper& paper);

  size_t dotsVisibleAfterFirstFoldInstruction() {
    doFold(fold_instructions[0]);
    return dots.size();
  }

  void startFolding() {
    for (const auto& fi : fold_instructions) {
      doFold(fi);
    }
  }
};

std::istream& operator>>(std::istream& is, Paper& paper) {
  std::string line;
  while (std::getline(is, line)) {
    if (line.empty()) break;
    std::stringstream ss{line};
    int x, y;
    char delim;
    ss >> x >> delim >> y;
    paper.dots.emplace(x, y);
  }

  while (std::getline(is, line)) {
    std::regex r{R"(fold along (\w)=(\d+))"};
    std::smatch sm;
    if (std::regex_search(line, sm, r)) {
      FoldInstruction::FoldDirection dir = (sm[1] == 'x')
                                               ? FoldInstruction::FoldDirection::vertical
                                               : FoldInstruction::FoldDirection::horizontal;
      paper.fold_instructions.emplace_back(dir, std::stoi(sm[2]));
    }
  }
  return is;
}

std::ostream& operator<<(std::ostream& os, const Paper& paper) {
  const auto width{std::max_element(paper.dots.cbegin(), paper.dots.cend(),
                                    [](const Dot& d1, const Dot& d2) { return d1.x < d2.x; })};
  const auto height{std::max_element(paper.dots.cbegin(), paper.dots.cend(),
                                     [](const Dot& d1, const Dot& d2) { return d1.y < d2.y; })};
  const int max_x{width->x}, max_y{height->y};
  for (int row{0}; row <= max_y; ++row) {
    for (int col{0}; col <= max_x; ++col) {
      os << (paper.dots.contains({col, row}) ? '#' : '.');
    }
    os << '\n';
  }
  return os;
}

int main() {
  std::ifstream file{"assets/input13.txt"};
  Paper paper;
  file >> paper;
  std::printf("Part 1: %lu\n", paper.dotsVisibleAfterFirstFoldInstruction());
  paper.startFolding(); // Does first fold again, but folds are idempotent anyway.
  std::cout << "Part 2:\n" << paper << '\n';
}
