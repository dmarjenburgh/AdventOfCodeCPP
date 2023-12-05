#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <regex>
#include <unordered_set>
#include "lib/matrix.hpp"

enum class Direction {
    Up, Right, Left, Down
};

const std::unordered_map<Direction, std::string> directionText = {
        {Direction::Up,    "Up"},
        {Direction::Right, "Right"},
        {Direction::Left,  "Left"},
        {Direction::Down,  "Down"}
};

const std::unordered_map<char, Direction> directionMap = {
        {'U', Direction::Up},
        {'R', Direction::Right},
        {'L', Direction::Left},
        {'D', Direction::Down}
};

struct Instruction {
    Direction direction;
    int distance;
};

std::vector<Instruction> parseFile(const std::string &fileName) {
    std::regex pattern(R"(([URLD]) (\d+))");

    std::ifstream file(fileName);
    std::vector<Instruction> infos;
    std::string line;

    while (std::getline(file, line)) {
        std::smatch matches;
        if (std::regex_match(line, matches, pattern)) {
            char directionChar = matches[1].str()[0];
            int distance = std::stoi(matches[2].str());
            infos.push_back({directionMap.at(directionChar), distance});
        }
    }
    return infos;
}

void update_tail(const Point &head, Point &tail) {
    int dx{head.x - tail.x};
    int dy{head.y - tail.y};
    if (dx == 0) {
        if (dy == -2) { tail.y--; }
        else if (dy == 2) { tail.y++; }
    } else if (dy == 0) {
        if (dx == -2) { tail.x--; }
        else if (dx == 2) { tail.x++; }
    } else if (!(abs(dx) == 1 && abs(dy) == 1)) {
        tail.x += abs(dx) / dx;
        tail.y += abs(dy) / dy;
    }

}

std::unordered_set<Point> execute_instructions(const std::vector<Instruction> &instructions) {
    std::unordered_set<Point> visited_points;
    Point head{0, 0};
    Point tail{0, 0};

    visited_points.insert(tail);

    for (const Instruction &instruction: instructions) {
        switch (instruction.direction) {
            case Direction::Up:
                for (int i = 0; i < instruction.distance; ++i) {
                    head.y++;
                    update_tail(head, tail);
                    visited_points.insert(tail);
                }
                break;
            case Direction::Down:
                for (int i = 0; i < instruction.distance; ++i) {
                    head.y--;
                    update_tail(head, tail);
                    visited_points.insert(tail);
                }
                break;
            case Direction::Right:
                for (int i = 0; i < instruction.distance; ++i) {
                    head.x++;
                    update_tail(head, tail);
                    visited_points.insert(tail);
                }
                break;
            case Direction::Left:
                for (int i = 0; i < instruction.distance; ++i) {
                    head.x--;
                    update_tail(head, tail);
                    visited_points.insert(tail);
                }
                break;
        }
    }
    return visited_points;
}

int main() {
    const auto instructions{parseFile("assets/input9.txt")};
    std::cout << "Part 1: " << execute_instructions(instructions).size() << '\n';
}