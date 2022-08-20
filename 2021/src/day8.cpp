#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <unordered_map>
#include <vector>

using SignalPattern = std::string;
using PatternList = std::array<SignalPattern, 10>;
using OutputValue = std::array<SignalPattern, 4>;
using Mapping = std::unordered_map<char, char>;

const std::unordered_map<std::string, int> digit_map = {
    {"abcefg", 0}, {"cf", 1},     {"acdeg", 2}, {"acdfg", 3},   {"bcdf", 4},
    {"abdfg", 5},  {"abdefg", 6}, {"acf", 7},   {"abcdefg", 8}, {"abcdfg", 9}};

struct Pattern {
  PatternList patternlist;
  OutputValue output;

  friend std::istream& operator>>(std::istream& is, Pattern& pattern);
  friend std::ostream& operator<<(std::ostream& os, const Pattern& pattern);
};

std::istream& operator>>(std::istream& is, Pattern& pattern) {
  std::string signal_pattern;
  for (int i{0}; i < 10; ++i) {
    is >> signal_pattern;
    std::sort(signal_pattern.begin(), signal_pattern.end());
    pattern.patternlist[i] = std::move(signal_pattern);
  }
  char delim;
  is >> delim;
  if (delim != '|') {
    is.setstate(std::ios_base::badbit);
    return is;
  }
  for (int i{0}; i < 4; ++i) {
    is >> signal_pattern;
    std::sort(signal_pattern.begin(), signal_pattern.end());
    pattern.output[i] = std::move(signal_pattern);
  }
  return is;
}

std::ostream& operator<<(std::ostream& os, const Pattern& pattern) {
  for (auto& s : pattern.patternlist) os << s << ' ';
  os << '|';
  for (auto& s : pattern.output) os << ' ' << s;
  os << '\n';
  return os;
}

std::vector<Pattern> parseinput() {
  std::ifstream file("assets/input8.txt", std::ifstream::in);
  std::string line;
  std::vector<Pattern> result;
  while (std::getline(file, line)) {
    std::stringstream ss{line};
    Pattern p;
    ss >> p;
    result.push_back(p);
  }
  return result;
}

int count_1478(const Pattern& pat) {
  int sum{};
  for (const auto& s : pat.output) {
    switch (s.length()) {
      case 2:  // Digit 1
      case 3:  // Digit 7
      case 4:  // Digit 4
      case 7:  // Digit 8
        sum++;
        break;
    }
  }
  return sum;
}

std::unordered_map<char, int> char_counts(const PatternList& l) {
  std::unordered_map<char, int> counts;
  for (const auto& pat : l) {
    for (const auto& ch : pat) {
      counts[ch]++;
    }
  }
  return counts;
}

void set_remaining_mapping(const PatternList& l, Mapping& m, int len, char ch) {
  const auto sp =
      std::find_if(l.cbegin(), l.cend(), [len](const auto& s) { return s.length() == len; });
  const auto c = std::find_if(sp->cbegin(), sp->cend(), [&m](char c) { return !m.contains(c); });
  m[*c] = ch;
}

// Strategy:
// Find the segment with frequency 4, it maps to 'e'
// Find the segment with frequency 6, it maps to 'b'
// Find the segment with frequency 9, it maps to 'f'
// Find the pattern with 2 segments, that is the 1. The unmapped segment maps to 'c'
// Find the pattern with 3 segments, that is the 7 and the new segment maps to 'a'
// Find the pattern with 4 segments, that is the 4. The unmapped segment maps to 'd'
// Find the pattern with 7 segments, that is the 8. The unmapped segments maps to 'g'
Mapping create_mapping(const Pattern& pat) {
  Mapping mapping;
  const auto char_freqs{char_counts(pat.patternlist)};

  // Map 'e', 'b' and 'f' based on frequencies
  for (const auto& item : char_freqs) {
    switch (item.second) {
      case 4:
        mapping[item.first] = 'e';
        break;
      case 6:
        mapping[item.first] = 'b';
        break;
      case 9:
        mapping[item.first] = 'f';
        break;
    }
  }

  // Map the rest
  set_remaining_mapping(pat.patternlist, mapping, 2, 'c');
  set_remaining_mapping(pat.patternlist, mapping, 3, 'a');
  set_remaining_mapping(pat.patternlist, mapping, 4, 'd');
  set_remaining_mapping(pat.patternlist, mapping, 7, 'g');

  return mapping;
}

int translate_digit(SignalPattern sp, const Mapping& m) {
  std::for_each(sp.begin(), sp.end(), [&m](auto& c) { c = m.at(c); });
  std::sort(sp.begin(), sp.end());
  return digit_map.at(sp);
}

int translate_number(const Mapping& m, const OutputValue& out) {
  int n{};
  for (const auto& s : out) {
    n = 10 * n + translate_digit(s, m);
  }
  return n;
}

int main() {
  auto patterns = parseinput();
  int sum = std::accumulate(patterns.cbegin(), patterns.cend(), 0,
                            [](int acc, auto& p) { return acc + count_1478(p); });
  std::printf("Part 1: %i\n", sum);
  sum = 0;
  for (auto& p : patterns) {
    const Mapping m{create_mapping(p)};
    sum += translate_number(m, p.output);
  }
  std::printf("Part 2: %i\n", sum);
}
