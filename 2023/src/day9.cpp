#include "lib/IOUtils.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

namespace ranges = std::ranges;

std::vector<std::vector<int>> parse_input() {
    const auto &lines{ReadAllLines("assets/input9.txt")};
    std::vector<std::vector<int>> result;
    for (const auto &line: lines) {
        std::stringstream ss{line};
        std::vector<int> numbers;
        int number;
        while (ss >> number) {
            numbers.push_back(number);
        }
        result.push_back(numbers);
    }

    return result;
}

std::pair<int, int> extrapolate(const std::vector<int> &nums) {
    std::vector<int> diffs;
    for (int i = 1; i < nums.size(); ++i) {
        diffs.push_back(nums[i] - nums[i - 1]);
    }
    if (ranges::all_of(diffs, [](int x) { return x == 0; })) {
        return {nums.front(), nums.back()};
    } else {
        const auto &[l, r] = extrapolate(diffs);
        return {nums.front() - l, nums.back() + r};
    }
}


int main() {
    const auto &number_histories{parse_input()};
    int sum_l{}, sum_r{};
    for (const auto &nums: number_histories) {
        const auto &[l, r] = extrapolate(nums);
        sum_l += l;
        sum_r += r;
    }
    std::cout << "Part 1: " << sum_r << '\n';
    std::cout << "Part 2: " << sum_l << '\n';
}