#include "lib/IOUtils.hpp"
#include "lib/matrix.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>

using Grid = std::vector<std::string>;
namespace ranges = std::ranges;

enum class Direction {
    RIGHT,
    DOWN,
    LEFT,
    UP
};

constexpr std::array<Direction, 4> Directions = {Direction::RIGHT, Direction::DOWN, Direction::LEFT, Direction::UP};

constexpr Point getDeltaPos(const Direction dir) {
    switch (dir) {
        case Direction::RIGHT:
            return {1, 0};
        case Direction::DOWN:
            return {0, 1};
        case Direction::LEFT:
            return {-1, 0};
        case Direction::UP:
            return {0, -1};
    }
}

std::tuple<Point, Point, int, Point, int> iterInfo(const Grid &g, const Direction dir) {
    // Returns the starting position, first delta direction vector, first iteration dimensionality, second delta direction vector, second iteration dimensionality
    const int numRows{static_cast<int>(g.size())};
    const int numCols{static_cast<int>(g[0].size())};
    switch (dir) {
        case Direction::RIGHT:
            return {{0, 0}, getDeltaPos(dir), numCols, {0, 1}, numRows};
        case Direction::DOWN:
            return {{0, 0}, getDeltaPos(dir), numRows, {1, 0}, numCols};
        case Direction::LEFT:
            return {{numCols - 1, 0}, getDeltaPos(dir), numCols, {0, 1}, numRows};
        case Direction::UP:
            return {{0, numRows - 1}, getDeltaPos(dir), numRows, {1, 0}, numCols};
    }
}

size_t numVisibleTrees(const Grid &g) {
    std::unordered_set<const char *> visibleTrees{100 * 100};

    for (const Direction &d: Directions) {
        const auto &[startPos, deltaPos, dim1, deltaPos2, dim2]{iterInfo(g, d)};
        Point pos{startPos};
        for (int i{}; i < dim1; ++i) {
            int row{pos.y};
            int col{pos.x};
            int maxHeight{-1};
            for (int j{}; j < dim2; ++j) {
                const char *tree{&g[row][col]};
                int height{*tree - '0'};
                if (height > maxHeight) {
                    visibleTrees.insert(tree);
                    maxHeight = height;
                }
                row += deltaPos.y;
                col += deltaPos.x;
            }
            pos.x += deltaPos2.x;
            pos.y += deltaPos2.y;
        }
    }
    return visibleTrees.size();
}


int scenicScore(const Grid &g, const int treeHouseRow, const int treeHouseCol) {
    const int numRows{static_cast<int>(g.size())};
    const int numCols{static_cast<int>(g[0].size())};
    int score{1};
    int treeHouseHeight{g[treeHouseRow][treeHouseCol] - '0'};

    for (const Direction &d: Directions) {
        const Point dr{getDeltaPos(d)};
        int row{treeHouseRow + dr.y};
        int col{treeHouseCol + dr.x};
        int dirScore{};
        while (row >= 0 && row < numRows && col >= 0 && col < numCols) {
            int treeHeight{g[row][col] - '0'};
            dirScore++;
            if (treeHeight >= treeHouseHeight) { break; }
            row += +dr.y;
            col += dr.x;
        }
        score *= dirScore;
    }
    return score;
}

int maxScenicScore(const Grid &g) {
    int maxScore{};
    for (int r{}; r < g.size(); ++r) {
        for (int c{}; c < g[0].size(); ++c) {
            maxScore = std::max(maxScore, scenicScore(g, r, c));
        }
    }
    return maxScore;
}

int main() {
    const auto grid{ReadAllLines("assets/input8.txt")};
    const auto visibleTreeCount{numVisibleTrees(grid)};
    const auto score{maxScenicScore(grid)};

    std::cout << "Part 1: " << visibleTreeCount << '\n';
    std::cout << "Part 2: " << score << '\n';
}
