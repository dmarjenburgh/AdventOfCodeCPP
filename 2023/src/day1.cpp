#include "lib/IOUtils.hpp"
#include <array>
#include <cctype>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>

namespace ranges = std::ranges;

constexpr std::array<std::string_view, 18> digits = {
    "1",   "2",   "3",     "4",    "5",    "6",   "7",     "8",     "9",
    "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

int find_digit(std::string_view s) {
  auto m{ranges::find_if(digits, [&s](auto &d) { return s.starts_with(d); })};
  if (m == digits.end()) {
    return -1;
  }
  int i = std::distance(digits.begin(), m);
  return (i > 8) ? i - 8 : i + 1;
}

int calibrationValue(const std::string &s) {
  auto first{*std::find_if(s.begin(), s.end(),
                           [](char c) { return std::isdigit(c); })};
  auto last{*std::find_if(s.rbegin(), s.rend(),
                          [](char c) { return std::isdigit(c); })};
  return std::stoi(std::string{} + first + last);
}

int realCalibrationValue(const std::string_view s) {
  std::vector<int> found_digits;
  for (auto it{s.begin()}; it != s.end(); ++it) {
    int digit{find_digit(std::string_view{it, s.end()})};
    if (digit >= 0) {
      found_digits.push_back(digit);
    };
  }
  return found_digits.front() * 10 + found_digits.back();
};

int sumValues(const std::vector<std::string> &lines, auto f) {
  return std::accumulate(lines.begin(), lines.end(), 0,
                         [&f](int acc, auto &l) { return acc + f(l); });
}

int main() {
  const auto lines{ReadAllLines("assets/input1.txt")};
  std::cout << "Part 1: " << sumValues(lines, &calibrationValue) << '\n';
  std::cout << "Part 2: " << sumValues(lines, &realCalibrationValue) << '\n';
}