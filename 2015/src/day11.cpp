#include <iostream>
#include <regex>
#include <string>

char nextchar(const char c) {
  if (c == 122)
    return 97;
  else if (c == 104 || c == 107 || c == 110)
    return c + 2;
  return c + 1;
}

void incstring(std::string &s) {
  bool reset = false;
  for (int i{0}; i < s.length(); i++) {
    const char c{s[i]};
    if (reset) {
      s[i] = 'a';
    } else if (c == 105 || c == 108 || c == 111) {
      s[i] = nextchar(c);
      reset = true;
    }
  }
  if (reset)
    return;

  int i = s.length() - 1;
  while (i >= 0) {
    char c{s[i]};
    char nc{nextchar(c)};
    s[i] = nc;
    if (nc == 'a') {
      --i;
    } else {
      break;
    }
  }
}

bool validpassword(const std::string &s) {
  std::regex r{R"((\w)\1\w*(\w)\2)"};
  std::smatch sm;
  if (!std::regex_search(s, sm, r))
    return false;
  if (sm[1] == sm[2])
    return false;

  for (int i{0}; i < s.size() - 2; ++i) {
    std::string tri{s[i], s[i + 1], s[i + 2]};
    if (tri[1] == tri[0] + 1 && tri[2] == tri[1] + 1)
      return true;
  }
  return false;
}

std::string nextpassword(std::string s) {
  for (incstring(s); !validpassword(s); incstring(s))
    ;
  return s;
}

int main() {
  std::string input{"hxbxwxba"};
  std::string p1{nextpassword(input)};
  std::cout << "Part 1: " << p1 << std::endl;
  std::string p2{nextpassword(p1)};
  std::cout << "Part 2: " << p2 << std::endl;
}
