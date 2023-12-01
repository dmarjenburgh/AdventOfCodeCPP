#include <iostream>
#include <sstream>
#include <string>

void lookandsay(const std::string &input, std::string &out) {
  std::ostringstream s;
  int count{};
  char lastchar{input[0]};
  for (const auto &c : input) {
    if (c == lastchar)
      ++count;
    else {
      s << count << lastchar;
      lastchar = c;
      count = 1;
    }
  }
  s << count << lastchar;
  out = s.str();
}

int main() {
  std::string input{"1321131112"};
  for (int i{0}; i < 40; ++i) {
    lookandsay(input, input);
  }
  std::cout << "Part 1: " << input.length() << std::endl;
  for (int i{0}; i < 10; ++i) {
    lookandsay(input, input);
  }
  std::cout << "Part 2: " << input.length() << std::endl;
}
