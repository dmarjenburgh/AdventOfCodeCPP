#include "lib/IOUtils.hpp"
#include "lib/matrix.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <functional>
#include <unordered_map>

bool can_match(const std::string &record, const int i, const int block_length) {
    for (int j{}; j < block_length; ++j) {
        if (record[i + j] == '.' or i + j == record.size()) {
            return false;
        }
    }
    if (record.size() > i + block_length and record[i + block_length] == '#') {
        return false;
    }
    return true;
}

// I need to map two ints to a result for the cache. It's more natural to use
// std::pair<int, int>, but since Point is essentially the same and already hashable, I just use that.
// Point::x is the group index, Point::y the record index
long calculate_num_configurations(const std::string &record, const std::vector<int> &nums) {

    std::unordered_map<Point, long> memo;

    std::function<long(Point)> f;
    f = [&](const Point &p) {

        if (memo.contains(p)) { return memo[p]; }
        if (p.y >= record.size()) { return memo[p] = p.x == nums.size(); }
        if (p.x == nums.size()) { return memo[p] = (record.find('#', p.y) == std::string::npos); }

        long dot_case{}, hash_case{};

        if (record[p.y] != '.') {
            int block_length{nums[p.x]};
            hash_case = can_match(record, p.y, block_length) ? f({p.x + 1, p.y + block_length + 1}) : 0;
        }
        if (record[p.y] != '#') {
            dot_case = f({p.x, p.y + 1});
        }

        return memo[p] = dot_case + hash_case;
    };
    return f({0, 0});
}


std::pair<std::string, std::vector<int>> parse_record_line(const std::string &record) {
    auto split = record.find(' ');
    std::string springs{record.substr(0, split)};
    std::vector<int> nums{ParseInts(record.substr(split + 1))};
    return {springs, nums};
}

long num_configurations(const std::string &record) {
    auto [springs, nums]{parse_record_line(record)};
    return calculate_num_configurations(springs, nums);
}

long num_expanded_configurations(const std::string &record) {
    auto [springs, nums]{parse_record_line(record)};
    std::vector<int> repeated_nums;
    std::ostringstream oss;
    for (int i{}; i < 5; ++i) {
        if (i != 0) { oss << "?"; }
        oss << springs;
        repeated_nums.insert(repeated_nums.end(), nums.begin(), nums.end());
    }

    std::string repeated_springs(oss.str());
    return calculate_num_configurations(repeated_springs, repeated_nums);
}

int main() {
    const auto &records{ReadAllLines("assets/input12.txt")};
    long sum{}, sum2{};
    for (const auto &record: records) {
        sum += num_configurations(record);
        sum2 += num_expanded_configurations(record);
    }
    std::cout << "Part 1: " << sum << '\n';
    std::cout << "Part 2: " << sum2 << '\n';
}
// Part 1: 7541
// Part 2: 17485169859432