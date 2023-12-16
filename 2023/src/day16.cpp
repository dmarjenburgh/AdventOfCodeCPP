#include "lib/IOUtils.hpp"
#include "lib/matrix.hpp"
#include <unordered_set>
#include <vector>
#include <string>
#include <stack>
#include <iostream>
#include <algorithm>

using Grid = std::vector<std::string>;

constexpr std::array<Point, 4> directions = {{{0, -1}, {1, 0}, {0, 1}, {-1, 0}}};

struct Photon {
    Point pos{};
    int dir{};

    bool operator==(const Photon &other) const {
        return pos == other.pos && dir == other.dir;
    }

    struct Hasher {
        std::size_t operator()(const Photon &p) const {
            std::size_t h1 = std::hash<Point>()(p.pos);
            std::size_t h2 = std::hash<int>()(p.dir);
            h1 ^= h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);
            return h1;
        }
    };
};

int walk(const Grid &grid, const Photon &start) {
    int w{static_cast<int>(grid[0].size())}, h{static_cast<int>(grid.size())};
    std::unordered_set<Photon, Photon::Hasher> visited;

    std::stack<Photon> photons;
    auto in_grid = [&](const Photon &p) -> bool {
        return p.pos.x >= 0 and p.pos.x < w and p.pos.y >= 0 and p.pos.y < h;
    };
    auto handle_slash = [](const Photon &p) -> std::vector<Photon> {
        int next_dir = (p.dir % 2 == 0) ? p.dir + 1 : p.dir - 1;
        return {{p.pos + directions[next_dir], next_dir}};
    };

    auto handle_backslash = [](const Photon &p) -> std::vector<Photon> {
        int next_dir = p.dir % 2 == 0 ? (p.dir + 3) % 4 : (p.dir + 1) % 4;
        return {{p.pos + directions[next_dir], next_dir}};
    };

    auto next_states = [&](const Photon &p) -> std::vector<Photon> {
        char tile = grid[p.pos.y][p.pos.x];
        if (tile == '/') {
            return handle_slash(p);
        } else if (tile == '\\') {
            return handle_backslash(p);
        } else if (tile == '-' and p.dir % 2 == 0) {
            return {
                    {p.pos + directions[1], 1},
                    {p.pos + directions[3], 3}
            };
        } else if (tile == '|' and p.dir % 2 == 1) {
            return {
                    {p.pos + directions[0], 0},
                    {p.pos + directions[2], 2}
            };
        } else {
            return {{p.pos + directions[p.dir], p.dir}};
        }
    };

    photons.push(start);
    while (!photons.empty()) {
        Photon p{photons.top()};
        photons.pop();
        if (visited.contains(p)) {
            continue;
        }
        visited.insert(p);
        auto next_photons{next_states(p)};
        for (const auto &np: next_photons) {
            if (!visited.contains(np) and in_grid(np)) {
                photons.push(np);
            }
        }
    }
    std::unordered_set<Point> tiles;
    for (const auto &p: visited) {
        tiles.insert(p.pos);
    }
    return static_cast<int>(tiles.size());
}

int max_energy(const Grid &grid) {
    int w{static_cast<int>(grid[0].size())}, h{static_cast<int>(grid.size())};

    std::vector<Photon> starts;
    for (int c{}; c < w; ++c) {
        starts.push_back({{c, 0}, 2});
        starts.push_back({{c, h - 1}, 2});
    }
    for (int r{}; r < h; ++r) {
        starts.push_back({{w - 1, r}, 3});
        starts.push_back({{0, r}, 3});
    }
    std::vector<int> energies;
    std::ranges::transform(
            starts,
            std::back_inserter(energies),
            [&](const auto &p) -> int { return walk(grid, p); }
    );
    return std::ranges::max(energies);
}

int main() {
    const auto grid{ReadAllLines("assets/input16.txt")};
    std::cout << "Part 1: " << walk(grid, Photon{{0, 0}, 1}) << '\n';
    std::cout << "Part 2: " << max_energy(grid) << '\n';
}
