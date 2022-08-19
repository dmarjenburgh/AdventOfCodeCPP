#include <fstream>
#include <iostream>
#include <vector>

std::tuple<int, std::vector<int>> parseinput() {
  std::ifstream file("assets/input3.txt", std::ifstream::in);
  std::string line;
  std::vector<int> result;
  size_t width{};
  while (std::getline(file, line)) {
    if (!width) width = line.length();
    result.push_back(std::stoi(line, nullptr, 2));
  }
  return {width, result};
}

int majority_sift(std::vector<int> ints, const int width, const bool keep_majority) {
  int pos{width};
  while (ints.size() > 1) {
    --pos;
    const size_t count{ints.size()};
    const size_t sum =
        std::count_if(ints.cbegin(), ints.cend(), [pos](int n) { return (n >> pos) & 1; });
    const bool keep_ones = (keep_majority == (2 * sum >= count));
    auto keep = [=](int n) { return keep_ones ? ((n >> pos) & 1) ^ 1 : (n >> pos) & 1; };
    std::erase_if(ints, keep);
  }
  return ints[0];
}

int main() {
  const auto [width, ints] = parseinput();
  const size_t num_lines{ints.size()};
  int gamma_rate{};
  for (int pos{}; pos < width; ++pos) {
    const size_t sum =
        std::count_if(ints.cbegin(), ints.cend(), [pos](int n) { return (n >> pos) & 1; });
    gamma_rate |= static_cast<int>(sum > num_lines / 2) << pos;
  }
  // Epsilon rate is just the complement (with masking)
  int epsilon_rate = ~gamma_rate & ~(-1 << width);
  std::printf("gamma rate: %d, epsilon rate: %d\n", gamma_rate, epsilon_rate);
  std::cout << "Part 1: " << gamma_rate * epsilon_rate << std::endl;

  // Part 2
  const int oxygen_generator_rating{majority_sift(ints, width, true)};
  const int co2_scrubber_rating{majority_sift(ints, width, false)};
  std::printf("oxygen generator rating: %i\nco2 scrubber rating: %i\n", oxygen_generator_rating,
              co2_scrubber_rating);
  std::printf("Part 2: %d\n", oxygen_generator_rating * co2_scrubber_rating);
}
