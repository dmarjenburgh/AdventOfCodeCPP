#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

// I thought just using string manipulation would be the easiest way
// I think I was wrong

using SnailFishNumber = std::string; // Har har

/**
 * Returns the indices of the opening and closing bracket of the next exploding
 * pair or (npos, npos) if not found.
 */
std::pair<size_t, size_t> find_explode_pos(const SnailFishNumber &s) {
  int depth{}, i{};
  size_t start;

  for (const char ch : s) {
    if (ch == '[') {
      depth++;
    } else if (ch == ']') {
      depth--;
    } else {
      if (depth > 4) {
        start = i;
        size_t end{s.find(']', start)};
        return {start - 1, end};
      }
    }
    i++;
  }
  return {std::string::npos, std::string::npos};
}

std::pair<int, int> pair_numbers(const std::string &pair_string) {
  std::pair<int, int> p;
  int x{};
  std::for_each(pair_string.cbegin() + 1, pair_string.cend() - 1,
                [&p, &x](const char &ch) {
                  if (ch == ',') {
                    p.first = x;
                    x = 0;
                  } else {
                    x = x * 10 + (ch - '0');
                  }
                });
  p.second = x;
  return p;
}

void add_number(SnailFishNumber &s, int x, size_t from, bool reverse) {
  int dir{reverse ? -1 : 1};

  size_t i{from};
  while (i and i < s.size() and !std::isdigit(s[i]))
    i += dir;

  if (!i or i == s.size())
    return;

  size_t first_pos = i;

  i = static_cast<int>(first_pos);
  while (std::isdigit(s[i]))
    i += dir;

  size_t second_pos{i - dir};
  if (first_pos > second_pos)
    std::swap(first_pos, second_pos);
  size_t l{second_pos - first_pos + 1};
  int v{std::stoi(s.substr(first_pos, l))};
  s.replace(first_pos, l, std::to_string(v + x));
}

bool explode(SnailFishNumber &s) {
  const auto [start, end] = find_explode_pos(s);
  if (start == std::string::npos) {
    return false;
  }

  const auto [left, right] = pair_numbers(s.substr(start, end - start + 1));
  s.replace(start, end - start + 1, "0");
  // start now points to the 0
  add_number(s, right, start + 1, false);
  add_number(s, left, start - 1, true);
  return true;
}

bool split(SnailFishNumber &s) {
  bool prev{static_cast<bool>(std::isdigit(s[0]))};
  const auto send{s.end()};
  for (auto it = s.begin() + 1; it != send; it++) {
    bool cur{static_cast<bool>(std::isdigit(*it))};
    if (prev and cur) {
      int v{std::stoi(std::string{it - 1, it + 1})};
      int l{v / 2}, r{v - l};
      s.replace(it - 1, it + 1,
                "[" + std::to_string(l) + "," + std::to_string(r) + "]");
      return true;
    }
    prev = cur;
  }
  return false;
}

bool reduce_step(SnailFishNumber &s) { return explode(s) or split(s); }

void reduce(SnailFishNumber &s) {
  while (reduce_step(s)) {
  }
}

SnailFishNumber add(const SnailFishNumber &s1, const SnailFishNumber &s2) {
  if (s1.empty())
    return s2;

  SnailFishNumber sum{"[" + s1 + "," + s2 + "]"};
  reduce(sum);
  return sum;
}

size_t find_comma(const SnailFishNumber &s) {
  // Haha, this is horrible
  size_t depth{}, i{};
  for (const char ch : s) {
    if (ch == '[')
      depth++;
    else if (ch == ']')
      depth--;
    else if (depth == 1 and ch == ',')
      return i;
    i++;
  }
  return std::string::npos;
}

size_t magnitude(const SnailFishNumber &s) {
  if (std::isdigit(s[0]))
    return s[0] - '0';
  else {
    size_t comma_pos{find_comma(s)};
    return 3 * magnitude(s.substr(1, comma_pos - 1)) +
           2 * magnitude(s.substr(comma_pos + 1));
  }
}

size_t largest_magnitude_sum(const std::vector<SnailFishNumber> &v) {
  size_t max_mag{};
  for (int i{0}; i < v.size(); ++i) {
    for (int j{i + 1}; j < v.size(); ++j) {
      if (size_t m{magnitude(add(v[i], v[j]))}; m > max_mag)
        max_mag = m;
      if (size_t m{magnitude(add(v[j], v[i]))}; m > max_mag)
        max_mag = m;
    }
  }
  return max_mag;
}

int main() {
  std::ifstream file{"assets/input18.txt"};
  std::vector<SnailFishNumber> v;
  SnailFishNumber line;
  while (std::getline(file, line))
    v.emplace_back(std::move(line));

  SnailFishNumber sn;
  for (const auto &next : v) {
    sn = add(sn, next);
  }

  std::printf("Part 1: %lu\n", magnitude(sn));
  std::printf("Part 2: %lu\n", largest_magnitude_sum(v));
}
