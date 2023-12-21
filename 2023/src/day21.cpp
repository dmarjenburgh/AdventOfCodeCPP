#include "lib/IOUtils.hpp"
#include "lib/matrix.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>

struct Grid {
    Point starting_point;
    std::unordered_set<Point> points;
    int height;
    int width;


    Grid(const std::vector<std::string> &lines) : height(lines.size()), width(lines[0].size()) {
        for (int r{}; r < height; ++r) {
            for (int c{}; c < width; ++c) {
                if (lines[r][c] != '#') {
                    points.insert({c, r});
                    if (lines[r][c] == 'S') {
                        starting_point = {c, r};
                    }
                }
            }
        }
    }
};

constexpr std::array<Point, 4> DIRECTIONS{{{0, -1}, {1, 0}, {0, 1}, {-1, 0}}};

long count_blocks(const Grid &grid, const std::unordered_set<Point> &points, int block_x, int block_y) {
    long count{};
    for (int r{}; r < grid.height; ++r) {
        for (int c{}; c < grid.width; ++c) {
            if (points.contains({c + block_x * grid.width, r + block_y * grid.height})) {
                count++;
            }
        }
    }
    return count;
}

std::unordered_set<Point> num_reachable_garden_plots(const Grid &grid, int num_steps) {
    std::unordered_set<Point> positions;
    Point starting_point{grid.starting_point};
    positions.insert(starting_point);
    for (int i{}; i < num_steps; ++i) {
        std::unordered_set<Point> next_positions;
        for (const auto &pos: positions) {
            for (const auto &dir: DIRECTIONS) {
                Point next_pos{pos + dir};
                if (grid.points.contains({((next_pos.x % grid.width) + grid.width) % grid.width,
                                          ((next_pos.y % grid.height) + grid.height) % grid.height})) {
                    next_positions.insert(next_pos);
                }
            }
        }
        positions = std::move(next_positions);
    }

    return positions;
}

long num_reachable_garden_plots_inf(const Grid &grid) {
    // The solution will only work because of a pattern in the input file.

    const long num_steps{26501365};
    // After 65 steps. A diamond/square shape is filled.
    // After each grid.width (w=131) steps, the diamond frontier has extended into the 'neighboring' garden tiles.

    // 26501365 = 65 + 2*w*101150
    // Deduce pattern for 65+2*w*N.
    const long N{(num_steps - 65) / (2 * grid.width)}; // N = 101150

    // All repeated parts are present for n=1
    const std::unordered_set<Point> n1_positions{num_reachable_garden_plots(grid, 65 + 2 * grid.width)};

    // Repeated parts frequencies as function of n:
    // Full+: (2n-1)^2
    // Full-: 4n^2
    // Arrows: 4, always 1 each
    // Large diagonal parts: 2n-1 (all 4)
    // Small diagonal parts: 2n (all 4)

    long full_even{count_blocks(grid, n1_positions, 0, 0)};
    long full_odd{count_blocks(grid, n1_positions, 1, 0)};
    long arrows{
            count_blocks(grid, n1_positions, 0, -2)
            + count_blocks(grid, n1_positions, 2, 0)
            + count_blocks(grid, n1_positions, 0, 2)
            + count_blocks(grid, n1_positions, -2, 0)
    };
    long diag_large{
            count_blocks(grid, n1_positions, 1, -1)
            + count_blocks(grid, n1_positions, 1, 1)
            + count_blocks(grid, n1_positions, -1, 1)
            + count_blocks(grid, n1_positions, -1, -1)
    };
    long diag_small{
            count_blocks(grid, n1_positions, 1, -2)
            + count_blocks(grid, n1_positions, 2, 1)
            + count_blocks(grid, n1_positions, -2, 1)
            + count_blocks(grid, n1_positions, -1, -2)
    };
    auto steps_formula = [&](long n) -> long {
        return (
                (2 * n - 1) * (2 * n - 1) * full_even
                + 4 * n * n * full_odd
                + arrows
                + diag_large * (2 * n - 1)
                + diag_small * 2 * n
        );
    };
    return steps_formula(N);
}

int main() {
    const Grid grid{ReadAllLines("assets/input21.txt")};
    std::cout << "Part 1: " << num_reachable_garden_plots(grid, 64).size() << '\n';
    std::cout << "Part 2: " << num_reachable_garden_plots_inf(grid) << '\n';
}
