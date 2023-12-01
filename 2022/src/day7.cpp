#include "lib/IOUtils.hpp"
#include <iostream>
#include <numeric>
#include <ranges>

namespace ranges = std::ranges;

constexpr int MAX_SPACE{70000000};
constexpr int SPACE_REQUIRED{30000000};

struct File {
  std::string name;
  int size;
};

struct Dir {
  std::string name;
  std::vector<Dir> dirs;
  std::vector<File> files;
  Dir *parent;
  int totalSize;
};

int du(Dir &dir) {
  if (dir.totalSize > 0) {
    return dir.totalSize;
  }
  int total_dir_size =
      std::accumulate(dir.dirs.begin(), dir.dirs.end(), 0,
                      [](int acc, auto &child) { return acc + du(child); });
  int total_file_size =
      std::accumulate(dir.files.begin(), dir.files.end(), 0,
                      [](int acc, auto &child) { return acc + child.size; });
  dir.totalSize = total_dir_size + total_file_size;
  return dir.totalSize;
}

int sum_of_dirs(const Dir &dir, const int threshold) {
  int subdirs{std::accumulate(dir.dirs.begin(), dir.dirs.end(), 0,
                              [&threshold](int acc, auto child) {
                                return acc + sum_of_dirs(child, threshold);
                              })};
  if (dir.totalSize <= threshold) {
    return dir.totalSize + subdirs;
  }
  return subdirs;
}

int size_of_smallest_dir(const Dir &dir, const int toFree) {
  if (dir.totalSize < toFree) {
    return INT32_MAX; // Directory too small
  }

  int subdirsize{std::transform_reduce(
      dir.dirs.begin(), dir.dirs.end(), INT32_MAX,
      [](auto d1, auto d2) { return std::min(d1, d2); },
      [&toFree](auto d) { return size_of_smallest_dir(d, toFree); })};
  return std::min(dir.totalSize, subdirsize);
}

int main() {
  const auto lines{ReadAllLines("assets/input7.txt")};
  auto root = Dir{.name = "/"};
  auto currentDir = &root;
  for (auto it{lines.begin() + 1}; it != lines.end(); ++it) {
    const auto &line{*it};
    const auto cmd{line.substr(2, 2)};
    if (cmd == "cd") {
      std::string new_dir{line.substr(5)};
      if (auto result{ranges::find_if(
              currentDir->dirs,
              [&new_dir](Dir &d) { return d.name == new_dir; })};
          result != currentDir->dirs.end()) {
        currentDir = &(*result);
      } else if (new_dir == "..") {
        currentDir = currentDir->parent;
      }
    } else if (cmd == "ls") {
      while ((it + 1) != lines.end() && !(it + 1)->starts_with('$')) {
        auto &l{*(++it)};
        if (l.starts_with("dir")) {
          Dir thenewdir{.name = l.substr(4), .parent = currentDir};
          currentDir->dirs.emplace_back(thenewdir);
        } else {
          auto e{l.find(' ')};
          int filesize{std::stoi(l.substr(0, e))};
          currentDir->files.emplace_back(
              File{.name = l.substr(e + 1), .size = filesize});
        }
      }
    }
  }

  const int totalSize{du(root)};
  std::cout << "Part 1: " << sum_of_dirs(root, 100000) << '\n';

  const int spaceAvailable{MAX_SPACE - totalSize};
  const int toFree{SPACE_REQUIRED - spaceAvailable};
  std::cout << "Part 2: " << size_of_smallest_dir(root, toFree) << '\n';
}