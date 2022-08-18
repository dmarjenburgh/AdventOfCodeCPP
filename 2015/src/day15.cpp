#include <iostream>
#include<fstream>
#include <regex>
#include <array>
#include <numeric>

std::vector<std::vector<int>> partitions(const int sum, const int parts) {
    if (parts == 1) {
        return std::vector<std::vector<int>>{{sum}};
    }
    std::vector<std::vector<int>> res{};
    for (int x{}; x <= sum; ++x) {
        const auto y{partitions(sum - x, parts - 1)};
        for (const auto& xs : y) {
            std::vector<int> t;
            std::copy(xs.begin(), xs.end(), std::back_inserter(t));
            t.push_back(x);
            res.emplace_back(t);
        }
    }
    return res;
}

bool is500calorycookie(const std::vector<std::array<int, 5>>& ingredients, const std::vector<int>& pts) {
    int total{};
    for (int i{}; i < ingredients.size(); ++i) {
        const int w{pts[i]};
        total += w * ingredients[i][4];
    }
    return total == 500;
}

int score(const std::vector<std::array<int, 5>>& ingredients, const std::vector<int>& pts) {
    int total{1};
    for (int p{}; p < 4; ++p) {
        int sum{};
        for (int i{}; i < ingredients.size(); ++i) {
            const int w{pts[i]};
            sum += w * ingredients[i][p];
        }
        total *= sum > 0 ? sum : 0;
    }
    return total;
}

int main() {
    std::ifstream input_file{"assets/2015/input15.txt"};
    std::string line;
    std::regex r{R"(\w+: capacity (-?\d+), durability (-?\d+), flavor (-?\d+), texture (-?\d+), calories (-?\d+))"};
    std::vector<std::array<int, 5>> ingredients{};
    while (std::getline(input_file, line)) {
        std::smatch sm;
        assert(std::regex_match(line, sm, r));
        std::array<int, 5> i{std::stoi(sm[1]), std::stoi(sm[2]), std::stoi(sm[3]), std::stoi(sm[4]), std::stoi(sm[5])};
        ingredients.emplace_back(i);
    }
    auto all_partitions{partitions(100, ingredients.size())};

    int max_score{std::accumulate(all_partitions.begin(), all_partitions.end(), 0, [&ingredients](int acc, auto p) {
        return std::max(acc, score(ingredients, p));
    })};
    std::cout << "Part 1: " << max_score << std::endl;

    max_score = std::accumulate(all_partitions.begin(), all_partitions.end(), 0, [&ingredients](int acc, auto p) {
        return is500calorycookie(ingredients, p) ? std::max(acc, score(ingredients, p)) : acc;
    });
    std::cout << "Part 2: " << max_score << std::endl;
}
