#include "lib/IOUtils.hpp"
#include "lib/matrix.hpp"
#include <string>
#include <vector>
#include <unordered_set>
#include <iostream>
#include <stack>
#include <optional>

namespace ranges = std::ranges;
using Grid = std::vector<std::string>;
using Points = std::unordered_set<Point>;
constexpr std::array<Point, 4> directions = {{{0, -1}, {1, 0}, {0, 1}, {-1, 0}}};

Point find_starting_position(const Grid &grid) {
    for (int i = 0; i < grid.size(); i++) {
        if (const auto pos = grid[i].find('S'); pos != std::string::npos) {
            return {(int) pos, i};
        }
    }
    throw std::runtime_error("No starting position found.");
}

bool contains(const std::string &s, char c) {
    return s.find(c) != std::string::npos;
}

void update_direction(int &dir, int &turns, char pipe) {
    std::array<std::string, 4> pipe_turns = {"|F 7", "J-7 ", " L|J", "L F-"};
    int turn_idx{static_cast<int>(pipe_turns[dir].find(pipe))};
    if (turn_idx != std::string::npos) {
        int cw_turns = static_cast<int>(turn_idx - dir + 4) % 4;
        dir = turn_idx;
        turns += cw_turns == 3 ? -1 : cw_turns;
    }
}

int initial_direction(const Grid &grid, const Point &start) {
    int dir;
    if (start.y > 0 and contains("7|F", grid[start.y - 1][start.x])) {
        dir = 0;
    } else if (start.x + 1 < grid[0].size() and contains("J-7", grid[start.y][start.x + 1])) {
        dir = 1;
    } else if (start.y - 1 < grid.size() and contains("L|J", grid[start.y + 1][start.x])) {
        dir = 2;
    } else if (start.x > 0 and contains("F-L", grid[start.y][start.x - 1])) {
        dir = 3;
    } else {
        throw std::runtime_error("No connecting pipe to starting position.");
    }
    return dir;
}

std::tuple<Points, int> walk_loop(const Grid &grid) {
    const Point start{find_starting_position(grid)};
    const int start_dir{initial_direction(grid, start)};
    Points points;

    Point pos{start};
    int dir{start_dir};
    int num_turns{};
    do {
        pos += directions[dir];
        update_direction(dir, num_turns, grid[pos.y][pos.x]);
        points.insert(pos);
    } while (grid[pos.y][pos.x] != 'S');
    int final_cw_turns = (start_dir - dir + 4) % 4;
    num_turns += final_cw_turns == 3 ? -1 : final_cw_turns;
    // Num turns should be +4 for positive orientation or -4 for negative orientation.
    // For a negative orientation, we go in the opposite direction as we ended.
    return {points, num_turns > 0 ? start_dir : (dir + 2) % 4};
}

Points connected_component(const Grid &grid, const Points &boundary, const Point &start) {
    Points component;
    std::stack<Point> stack;
    stack.push(start);

    while (!stack.empty()) {
        const Point pos{stack.top()};
        stack.pop();
        if (!boundary.contains(pos)) {
            component.insert(pos);
            for (const auto &dir: directions) {
                Point neighbour{pos + dir};
                if (neighbour.x >= 0 and neighbour.x < grid[0].size() and neighbour.y >= 0 and
                    neighbour.y < grid.size() and !component.contains(neighbour)) {
                    stack.push(neighbour);
                }
            }
        }
    }
    return component;
}

constexpr std::string_view corner_pipes{"JLF7"};

void expand_interior_point_set(const Grid &grid, const Points &loop, Points &interior, const Point &p) {
    if (loop.contains(p) or interior.contains(p)) {
        return;
    }
    Points component{connected_component(grid, loop, p)};
    interior.insert(component.begin(), component.end());
}

std::optional<std::array<int, 2>> get_neighbouring_interior_points(const int dir, const char pipe) {
    if (corner_pipes[dir] == pipe) {
        return std::optional<std::array<int, 2>>{{(dir + 1) % 4, (dir + 2) % 4}};
    }
    return std::nullopt;
}

Points find_interior_points(const Grid &grid, const Points &loop, const int start_dir) {
    const Point start{find_starting_position(grid)};
    Points interior;
    Point pos{start};
    int dir{start_dir};
    int num_turns{};

    do {
        pos += directions[dir];
        char pipe{grid[pos.y][pos.x]};
        update_direction(dir, num_turns, pipe);
        auto int_points{get_neighbouring_interior_points(dir, pipe)};
        if (int_points) {
            for (auto d: int_points.value()) {
                const Point p{pos + directions[d]};
                expand_interior_point_set(grid, loop, interior, p);
            }
        }

    } while (grid[pos.y][pos.x] != 'S');
    return interior;
}

int main() {
    // Algorithm
    // 1) Walk the loop and keep track of the length and orientation (cw or ccw). Solution to part 1 is length/2.
    // 2) We walk the loop again in the positive orientation (clockwise). Points to the right are interior.
    //    We only need to consider corner-pipes. When a new interior point is found, expand to all connected points
    //    that are not part of the loop. The solution to part 1 is the count of the interior points set.
    const auto grid{ReadAllLines("assets/input10.txt")};
    const auto &[loop, start_dir] = walk_loop(grid);
    std::cout << "Part 1: " << loop.size() / 2 << '\n';
    const auto interior{find_interior_points(grid, loop, start_dir)};
    std::cout << "Part 2: " << interior.size() << '\n';
    return 0;
}
