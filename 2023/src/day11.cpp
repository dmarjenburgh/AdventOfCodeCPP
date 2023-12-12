#include "lib/IOUtils.hpp"
#include "lib/matrix.hpp"
#include <ranges>
#include <iostream>
#include <algorithm>

namespace ranges = std::ranges;

using Points = std::vector<Point>;

long manhattan_distance(const Point &p, const Point &q) {
    return abs(p.x - q.x) + abs(p.y - q.y);
}

Points find_galaxies(const std::vector<std::string> &image) {
    Points galaxies;
    for (int row{}; row < image.size(); ++row) {
        for (int col{}; col < image[row].size(); ++col) {
            if (image[row][col] == '#') {
                galaxies.push_back({col, row});
            }
        }
    }
    return galaxies;
}

long computeSumOfDistances(const std::vector<std::string> &image, long expansion_factor) {
    Points galaxies{find_galaxies(image)};
    std::vector<int> empty_row_indices, empty_column_indices;

    for (int i{}; i < image.size(); ++i) {
        if (image[i].find('#') == std::string::npos) {
            empty_row_indices.push_back(i);
        }
    }
    for (int col{}; col < image[0].size(); ++col) {
        bool found{false};
        for (int row{}; row < image.size(); ++row) {
            if (image[row][col] == '#') {
                found = true;
                break;
            }
        }
        if (!found) {
            empty_column_indices.push_back(col);
        }
    }

    long sum{};

    for (int i{}; i < galaxies.size(); ++i) {
        for (int j{i + 1}; j < galaxies.size(); ++j) {
            long empty_space_amount{};

            auto [min_y, max_y] = std::minmax(galaxies[i].y, galaxies[j].y);
            auto [min_x, max_x] = std::minmax(galaxies[i].x, galaxies[j].x);

            auto row_begin = ranges::lower_bound(empty_row_indices, min_y);
            auto row_end = ranges::lower_bound(empty_row_indices, max_y);
            empty_space_amount += ranges::distance(row_begin, row_end);

            auto col_begin = ranges::lower_bound(empty_column_indices, min_x);
            auto col_end = ranges::lower_bound(empty_column_indices, max_x);
            empty_space_amount += ranges::distance(col_begin, col_end);

            sum += manhattan_distance(galaxies[i], galaxies[j]) + empty_space_amount * (expansion_factor - 1);
        }
    }

    return sum;
}

int main() {
    const auto image{ReadAllLines("assets/input11.txt")};
    std::cout << "Part 1: " << computeSumOfDistances(image, 2) << '\n';
    std::cout << "Part 2: " << computeSumOfDistances(image, 1'000'000L) << '\n';
    return 0;
}
