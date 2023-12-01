#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

using Chain = std::unordered_map<std::string, size_t>;
using Mapping = std::unordered_map<std::string, char>;

void step(Chain &chain, const Mapping &m) {
  Chain nextchain;
  for (const auto &kv : chain) {
    const auto it = m.find(kv.first);
    if (it != std::end(m)) {
      nextchain[{kv.first[0], it->second}] += kv.second;
      nextchain[{it->second, kv.first[1]}] += kv.second;
    } else {
      nextchain[kv.first] += kv.second;
    }
  }
  std::swap(chain, nextchain);
}

size_t score(Chain &chain) {
  std::vector<size_t> v(26, 0);
  for (const auto &[s, n] : chain) {
    v[s[1] - 'A'] += n;
  }
  v[chain.begin()->first[0] - 'A'] += chain.begin()->second;
  size_t min{std::numeric_limits<size_t>::max()}, max{};
  for (const auto &c : v) {
    if (c) {
      if (c > max)
        max = c;
      else if (c < min)
        min = c;
    }
  }
  return max - min;
}

// Another constant space solution.
// Keep track of the number of pairs, and update the counts in each step.
// Similar to day 6
int main() {
  std::ifstream file{"assets/input14.txt"};
  std::string polymer_template;
  file >> polymer_template;
  std::string k;
  Mapping m;
  while (file >> std::skipws >> k) {
    std::string _;
    char ch;
    file >> _ >> ch;
    m[k] = ch;
  }

  Chain chain;
  for (int i{1}; i < polymer_template.size(); ++i) {
    chain[std::string{polymer_template[i - 1], polymer_template[i]}]++;
  }

  // Perform 10 steps
  for (int i{0}; i < 10; ++i)
    step(chain, m);
  std::printf("Part 1: %lu\n", score(chain));

  // Perform 30 more steps
  for (int i{0}; i < 30; ++i)
    step(chain, m);
  std::printf("Part 2: %lu\n", score(chain));
}
