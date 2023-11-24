#include <fstream>
#include <iostream>
#include <set>
#include <cinttypes>
#include <stack>
#include <unordered_map>
#include <vector>

const std::set opening_delims = {'(', '[', '{', '<'};

const std::unordered_map<char, char> matching_delim = {
    {')', '('}, {']', '['}, {'}', '{'}, {'>', '<'}};
const std::unordered_map<char, char> matching_delim2 = {
    {'(', ')'}, {'[', ']'}, {'{', '}'}, {'<', '>'}};
const std::unordered_map<char, int> error_scoring_table = {
    {')', 3}, {']', 57}, {'}', 1197}, {'>', 25137}};

const std::unordered_map<char, int> completion_scoring_table = {
    {'(', 1}, {'[', 2}, {'{', 3}, {'<', 4}};

int main() {
  std::ifstream file{"assets/input10.txt"};
  int error_score{};
  std::vector<uint64_t> completion_scores{};
  std::stack<char> stack;
  char ch;
  while (file >> std::noskipws >> ch) {
    if (opening_delims.contains(ch)) {
      stack.push(ch);
    } else if (matching_delim.contains(ch)) {
      const char opening_delim{matching_delim.at(ch)};
      if (!stack.empty() and stack.top() == opening_delim) {
        stack.pop();
      } else {
        // Syntax error
        error_score += error_scoring_table.at(ch);
        stack = {};
        std::string line;
        std::getline(file, line);
      }
    } else if (ch == '\n') {
      // Line incomplete
      uint64_t completion_score{};
      while (!stack.empty()) {
        completion_score = completion_score * 5 + completion_scoring_table.at(stack.top());
        stack.pop();
      }
      completion_scores.push_back(completion_score);
    } else {
      std::cerr << "Invalid character: " << ch << '\n';
    }
  }
  std::sort(completion_scores.begin(), completion_scores.end());
  const uint64_t median_score{completion_scores[completion_scores.size() / 2]};
  std::printf("Part 1: %i\n", error_score);
  std::printf("Part 2: %" PRId64 "\n", median_score);
}
