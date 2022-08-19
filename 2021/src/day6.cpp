#include <array>
#include <fstream>
#include <iostream>

using FishCounts = std::array<uint64_t, 9>;

FishCounts parseinput() {
  std::ifstream file("assets/input6.txt", std::ifstream::in);
  std::string num;
  FishCounts counts;
  counts.fill(0);
  while (std::getline(file, num, ',')) {
    counts[std::stoi(num)] += 1;
  }
  return counts;
}

void iteration(FishCounts& state) {
  uint64_t newfish{state[0]};
  for (int days_left{0}; days_left < 8; ++days_left) {
    state[days_left] = state[days_left + 1];
  }
  state[6] += newfish;
  state[8] = newfish;
}

uint64_t process_days(FishCounts state, const int days) {
  for (int d{0}; d < days; ++d) iteration(state);
  uint64_t sum{};
  std::for_each(state.cbegin(), state.cend(), [&sum](uint64_t n) { sum += n; });
  return sum;
}

int main() {
  auto numbers = parseinput();
  std::printf("Part 1: %llu\n", process_days(numbers, 80));
  std::printf("Part 2: %llu\n", process_days(numbers, 256));
}
