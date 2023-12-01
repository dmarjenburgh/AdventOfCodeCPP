#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

enum class Command { forward, up, down };

std::vector<std::tuple<Command, int>> parseinput() {
  std::ifstream file("assets/input2.txt", std::ifstream::in);
  std::string line;
  std::vector<std::tuple<Command, int>> result;
  while (std::getline(file, line)) {
    std::stringstream ss{line};
    std::string command;
    int amount;
    ss >> command >> amount;
    if (command == "forward")
      result.emplace_back(Command::forward, amount);
    else if (command == "up")
      result.emplace_back(Command::up, amount);
    else if (command == "down")
      result.emplace_back(Command::down, amount);
    else
      std::cout << "Unknown command: " << command << std::endl;
  }
  return result;
}

int main() {
  std::string line;
  int hor_pos{0}, depth{0};

  // Part 1
  for (const auto [command, amount] : parseinput()) {
    switch (command) {
    case Command::forward:
      hor_pos += amount;
      break;
    case Command::up:
      depth -= amount;
      break;
    case Command::down:
      depth += amount;
      break;
    }
  }

  std::cout << "Part 1: " << hor_pos * depth << std::endl;

  // Part 2
  int aim{};
  hor_pos = 0;
  depth = 0;
  for (const auto [command, amount] : parseinput()) {
    switch (command) {
    case Command::forward:
      hor_pos += amount;
      depth += aim * amount;
      break;
    case Command::up:
      aim -= amount;
      break;
    case Command::down:
      aim += amount;
      break;
    }
  }
  std::cout << "Part 2: " << hor_pos * depth << std::endl;
}
