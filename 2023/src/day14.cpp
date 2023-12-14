#include "lib/IOUtils.hpp"
#include "lib/matrix.hpp"

#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <iostream>

using Platform = std::vector<std::string>;

size_t total_load(const Platform &platform) {
    size_t total{};
    for (int r{}; r < platform.size(); ++r) {
        total += std::ranges::count(platform[r], 'O') * (platform.size() - r);
    }
    return total;
}

Platform step(const Platform &platform, const Point start, const Point &dir1, const Point &dir2) {
    Platform next{platform};
    Point cur{start - dir1};
    while (cur.x >= 0 and cur.x < platform[0].size() and cur.y >= 0 and cur.y < platform.size()) {
        Point pos{cur};
        while (pos.x >= 0 and pos.x < platform[0].size() and pos.y >= 0 and pos.y < platform.size()) {
            if (platform[pos.y][pos.x] == 'O' and platform[pos.y + dir1.y][pos.x + dir1.x] == '.') {
                next[pos.y + dir1.y][pos.x + dir1.x] = 'O';
                next[pos.y][pos.x] = '.';
            }
            pos -= dir1;
        }
        cur += dir2;
    }
    return next;
}

Platform tilt_platform(Platform platform, const int dir) {
    Point start{}, dir1{}, dir2{};
    switch (dir) {
        case 0: // North
            start = {0, 0};
            dir1 = {0, -1};
            dir2 = {1, 0};
            break;
        case 1: // East
            start = {static_cast<int>(platform[0].size() - 1), 0};
            dir1 = {1, 0};
            dir2 = {0, 1};
            break;
        case 2: // South
            start = {0, static_cast<int>(platform.size() - 1)};
            dir1 = {0, 1};
            dir2 = {1, 0};
            break;
        case 3: // West
            start = {0, 0};
            dir1 = {-1, 0};
            dir2 = {0, 1};
            break;
        default:
            break;
    }
    while (true) {
        Platform next{step(platform, start, dir1, dir2)};
        if (next == platform) {
            break;
        }
        platform = std::move(next);
    }
    return platform;
}

Platform rotate_platform(Platform platform, int revolutions = 1) {
    for (int i{}; i < revolutions; ++i) {
        platform = tilt_platform(tilt_platform(tilt_platform(tilt_platform(platform, 0), 3), 2), 1);
    }
    return platform;
}

int find_min_required_rotations(const Platform &platform) {
    std::vector<Platform> platforms;
    Platform p{platform};
    platforms.push_back(p);
    while (true) {
        p = rotate_platform(p);
        if (std::ranges::find(platforms, p) != platforms.end()) {
            platforms.push_back(p);
            break;
        }
        platforms.push_back(p);
    }
    long pre_rotations{std::distance(platforms.begin(), std::ranges::find(platforms, platforms.back()))};
    size_t period{platforms.size() - pre_rotations - 1};
    return static_cast<int>(pre_rotations + (1'000'000'000L - pre_rotations) % period);

}

int main() {
    const auto lines{ReadAllLines("assets/input14.txt")};
    const Platform &platform{lines};
    std::cout << "Part 1: " << total_load(tilt_platform(platform, 0)) << '\n';
    std::cout << "Part 2: " << total_load(rotate_platform(platform, find_min_required_rotations(platform))) << '\n';
}