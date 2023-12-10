#include "lib/IOUtils.hpp"
#include "lib/matrix.hpp"
#include <string>
#include <vector>
#include <unordered_set>
#include <iostream>
#include <stack>
#include <algorithm>

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
    return {points, num_turns > 0 ? start_dir : (start_dir + 2) % 4};
}

Points connected_component(const Grid &grid, const Points &boundary, const Point &start) {
    Points component;
    if (boundary.contains(start)) {
        return Points{boundary};
    }
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

std::vector<Points> find_connected_components(const Grid &grid, const Points &boundary) {
    std::vector<Points> all_components;
    for (int row{}; row < grid.size(); row++) {
        for (int col{}; col < grid[row].size(); col++) {
            if (!boundary.contains({col, row}) and
                !ranges::any_of(all_components, [&row, &col](auto &c) { return c.contains({col, row}); })) {
                all_components.push_back(connected_component(grid, boundary, {col, row}));
            }
        }
    }
    return all_components;
}

void merge_component(const std::vector<Points> &components, std::unordered_set<Point> &set, const Point &p) {
    for (const auto &component: components) {
        if (component.contains(p)) {
            if (!set.contains(p)) {
                set.insert(component.begin(), component.end());
            }
            return;
        }
    }
}

std::tuple<bool, Point, Point> find_int_ext_points(const int dir, const char pipe) {
    // Points to the 'left' are exterior. Points to the 'right' are interior.
    if (pipe == 'J') {
        if (dir == 0) {
            return {true, {1, 0}, {0, 1}};
        } else if (dir == 3) {
            return {false, {1, 0}, {0, 1}};
        }
    } else if (pipe == 'L') {
        if (dir == 0) {
            return {false, {-1, 0}, {0, 1}};
        } else if (dir == 1) {
            return {true, {-1, 0}, {0, 1}};
        }
    } else if (pipe == 'F') {
        if (dir == 1) {
            return {false, {-1, 0}, {-1, 0}};
        } else if (dir == 2) {
            return {true, {-1, 0}, {-1, 0}};
        }
    } else if (pipe == '7') {
        if (dir == 2) {
            return {false, {1, 0}, {0, -1}};
        } else if (dir == 3) {
            return {true, {1, 0}, {0, -1}};
        }
    }
    throw std::runtime_error("Wrong pipe");
}

std::pair<std::unordered_set<Point>, std::unordered_set<Point>>
identify_interior_exterior_components(
        const Grid &grid,
        const Points &loop,
        const int start_dir,
        const std::vector<Points> &components) {
    const Point start{find_starting_position(grid)};

    std::unordered_set<Point> exterior;
    std::unordered_set<Point> interior;

    Point pos{start};
    int dir{start_dir};
    int num_turns{};
    do {
        pos += directions[dir];
        char pipe{grid[pos.y][pos.x]};
        update_direction(dir, num_turns, pipe);
        if (contains("JFL7", pipe)) {
            auto [is_interior, p1, p2] = find_int_ext_points(dir, pipe);
            if (!loop.contains(pos + p1)) {
                merge_component(components, is_interior ? interior : exterior, pos + p1);
            }
            if (!loop.contains(pos + p2)) {
                merge_component(components, is_interior ? interior : exterior, pos + p2);
            }
        }
    } while (grid[pos.y][pos.x] != 'S');
    return {interior, exterior};
}

int main() {
    // Algorithm
    // 1) Walk the loop and keep track of the length and orientation (cw or ccw). Solution to part 1 is length/2.
    // 2) Go through the whole grid and find the connected components, using the loop as a boundary.
    // 3) Now we need to find which components are interior and exterior.
    //    We walk the loop again. Points to the right are interior, and points to the left are exterior.
    //    Gather all the components with points in the interior/exterior sets. Solution to part 2 is
    //    the count of the interior points set.
    const auto &grid{ReadAllLines("assets/input10.txt")};
    const auto &[loop, start_dir] = walk_loop(grid);
    std::cout << "Part 1: " << loop.size() / 2 << '\n';
    const auto &all_connected_components{find_connected_components(grid, loop)};
    const auto &[interior, exterior] = identify_interior_exterior_components(grid, loop, start_dir,
                                                                             all_connected_components);
    std::cout << "Part 2: " << interior.size() << '\n';
}