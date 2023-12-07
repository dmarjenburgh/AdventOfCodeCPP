#include "lib/IOUtils.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <ranges>

namespace ranges = std::ranges;
using ResourceMapping  = std::vector<std::array<long, 3>>;

struct Range {
    long start;
    long end;
};

std::vector<Range> map_range(Range r, const ResourceMapping &rm) {
    std::vector<Range> result;
    for (const auto [dest_start, source_start, length]: rm) {
        if (r.start >= r.end) {
            break;
        }
        // Part before mapping range, from r.start to source_start
        long r_bound{std::min(r.end, source_start)};
        if (r.start < source_start) {
            result.push_back({r.start, r_bound});
            r.start = r_bound;
        }
        // Overlapping part, from max(source_start, r.start) to min(source_start+length, r.end)
        if (r.start < source_start + length && r.end > source_start) {
            long range_start{std::max(source_start, r.start)};
            long range_end{std::min(source_start + length, r.end)};
            result.push_back({range_start + dest_start - source_start, range_end + dest_start - source_start});
            r.start = range_end;
        }
    }
    // Trailing part
    if (r.start < r.end) {
        result.push_back({r.start, r.end});
    }
    return result;
}

std::pair<std::vector<long>, std::vector<ResourceMapping>> parse_lines(const std::vector<std::string> &lines) {
    // Parse seeds
    std::vector<long> seeds;
    std::istringstream seedStream(lines[0].substr(lines[0].find(':') + 1));
    long seed;
    while (seedStream >> seed) {
        seeds.push_back(seed);
    }

    // Parse mappings
    std::vector<ResourceMapping> mappings;
    ResourceMapping *currentMapping = nullptr;
    for (const std::string &line: lines) {
        if (line.find("map:") != std::string::npos) {
            mappings.emplace_back();
            currentMapping = &mappings.back();
        } else if (currentMapping != nullptr) {
            std::istringstream mappingStream(line);
            long a, b, c;
            if (!(mappingStream >> a >> b >> c)) {
                currentMapping = nullptr;
            } else {
                currentMapping->push_back({a, b, c});
            }
        }
    }
    for (auto &m: mappings) {
        ranges::sort(m, {}, [](auto &m) { return m[1]; });
    }
    return {seeds, mappings};
}

std::vector<Range> to_ranges(const std::vector<long> &nums) {
    std::vector<Range> result;
    ranges::transform(nums, std::back_inserter(result), [](long n) { return Range{n, n + 1}; });
    ranges::sort(result, {}, [](auto &r) { return r.start; });
    return result;
}

long seed_to_location(const std::vector<Range> &seeds, const std::vector<ResourceMapping> &resource_mappings) {
    std::vector<Range> acc{seeds};
    for (const auto &mapping: resource_mappings) {
        auto next_r = acc | std::views::transform([&mapping](auto &r) {
            return map_range(r, mapping);
        }) | std::views::join | std::views::common;
        acc = std::vector<Range>{next_r.begin(), next_r.end()};
        ranges::sort(acc, {}, [](auto &r) { return r.start; });
    }
    return acc[0].start;
}

std::vector<Range> expand_seeds(const std::vector<long> &seeds) {
    std::vector<Range> result;

    for (long i{}; i < seeds.size(); i += 2) {
        long start_value = seeds[i];
        long length = seeds[i + 1];
        result.emplace_back(start_value, start_value + length);
    }
    ranges::sort(result, {}, [](auto &r) { return r.start; });
    return result;
}

int main() {
    const auto &lines{ReadAllLines("assets/input5.txt")};
    auto [seeds, resource_mappings] = parse_lines(lines);
    std::cout << "Part 1: " << seed_to_location(to_ranges(seeds), resource_mappings) << '\n';
    std::cout << "Part 2: " << seed_to_location(expand_seeds(seeds), resource_mappings) << '\n';
}
