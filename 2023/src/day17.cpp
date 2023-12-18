#include <vector>
#include <string>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <ranges>
#include <numeric>
#include "lib/IOUtils.hpp"
#include "lib/matrix.hpp"
#include "lib/hash.hpp"

using Grid = std::vector<std::vector<int>>;

constexpr std::array<Point, 4> DIRECTIONS{{{0, -1}, {1, 0}, {0, 1}, {-1, 0}}};

struct Crucible {
    Point pos{};
    int dir{};
    int no_turn_counter{};

    auto operator<=>(const Crucible &other) const = default;

    struct Hasher {
        std::size_t operator()(const Crucible &c) const {
            std::size_t seed = 0;
            Util::hash_combine(seed, c.pos.x);
            Util::hash_combine(seed, c.pos.y);
            Util::hash_combine(seed, c.dir);
            Util::hash_combine(seed, c.no_turn_counter);
            return seed;
        }
    };
};

template<typename NeighboursFn, typename CostFn>
int bfs(const Point &goal,
        const int min_straight,
        const NeighboursFn &neighbours,
        const CostFn &cost
) {
    using PQPair = std::pair<int, Crucible>;
    std::priority_queue<PQPair, std::vector<PQPair>, std::greater<>> pq;
    Crucible start1{{0, 0}, 1, 1};
    Crucible start2{{0, 0}, 2, 1};
    pq.emplace(0, start1);
    pq.emplace(0, start2);
    std::unordered_map<Crucible, int, Crucible::Hasher> cost_so_far;
    std::unordered_set<Crucible, Crucible::Hasher> visited;
    cost_so_far[start1] = 0;
    cost_so_far[start2] = 0;
    Crucible last_pos;
    while (!pq.empty()) {
        const auto [heat_loss, current] = pq.top();
        pq.pop();
        if (visited.contains(current)) {
            continue;
        }
        visited.insert(current);

        if (current.pos == goal and current.no_turn_counter >= min_straight) {
            return heat_loss;
        }

        int current_cost{cost_so_far[current]};
        for (const Crucible &nb: neighbours(current)) {
            int new_cost{current_cost + cost(nb)};
            if (!cost_so_far.contains(nb) or new_cost < cost_so_far[nb]) {
                cost_so_far[nb] = new_cost;
                pq.emplace(new_cost, nb);
            }
        }
    }
    throw std::runtime_error("Goal could not be reached.");
}

int min_heat_loss(const Grid &grid, int min_straight, int max_straight) {
    int w{static_cast<int>(grid[0].size())};
    int h{static_cast<int>(grid.size())};

    Point goal{w - 1, h - 1};
    auto in_grid = [&w, &h](const Point &p) -> bool {
        return p.x >= 0 and p.x < w and p.y >= 0 and p.y < h;
    };
    auto neighbours = [&](const Crucible &c) {
        std::vector<Crucible> nbs;
        for (const auto &d: {0, 1, 2, 3}) {
            if (d != c.dir and c.no_turn_counter < min_straight) { continue; }
            if (d == c.dir and c.no_turn_counter >= max_straight) { continue; }
            if ((c.dir + 2) % 4 == d) { continue; }

            Crucible nb{c.pos + DIRECTIONS[d], d, d == c.dir ? c.no_turn_counter + 1 : 1};

            if (!in_grid(nb.pos)) { continue; }
            nbs.push_back(nb);
        }
        return nbs;
    };

    auto cost_fn = [&grid](const Crucible &c) { return grid[c.pos.y][c.pos.x]; };
    return bfs(goal, min_straight, neighbours, cost_fn);
}

Grid parse_grid(const std::vector<std::string> &lines) {
    std::vector<std::vector<int>> result;
    for (int i{}; i < lines.size(); ++i) {
        result.emplace_back();
        for (int j{}; j < lines[i].size(); ++j) {
            result[i].push_back((lines[i][j] - '0'));
        }
    }
    return result;
}

int main() {
    const auto grid{parse_grid(ReadAllLines("assets/input17.txt"))};
    std::cout << "Part 1: " << min_heat_loss(grid, 1, 3) << '\n';
    std::cout << "Part 2: " << min_heat_loss(grid, 4, 10) << '\n';
}
