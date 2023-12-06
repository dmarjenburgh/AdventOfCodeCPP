#include "lib/IOUtils.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <ranges>


namespace ranges = std::ranges;

struct Range {
    long start;
    long end;
};

struct ResourceMapping {
    std::vector<std::array<long, 3>> mappings;
};

auto create_mapper_fn(const ResourceMapping &resource_mapping) {
    return [&resource_mapping](long v) -> long {
        for (auto [dest_start, source_start, length]: resource_mapping.mappings) {
            if (v >= source_start && v < source_start + length) {
                return dest_start + (v - source_start);
            }
        }
        return v;
    };
}

std::vector<Range> map_range(const Range &r, const ResourceMapping &rm) {
    std::vector<Range> result;
    for (const auto [dest_start, source_start, length]: rm.mappings) {
        // Part before mapping range, from r.start to source_start
        if (r.start < source_start && r.end >= source_start) {
            result.push_back({r.start, source_start});
        }
        // Overlapping part, from max(source_start, r.start) to min(source_start+length, r.end)
        {
            long range_start{std::max(source_start, r.start)};
            long range_end{std::min(source_start + length, r.end)};
            if (range_start < range_end) {
                result.push_back({range_start, range_end});
            }
        }
        // Trailing part, from source_start + length to r.end
        if (source_start + length < r.end) {
            result.push_back({source_start + length, r.end});
        }
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
                currentMapping->mappings.push_back({a, b, c});
            }
        }
    }

    return {seeds, mappings};
}

long seed_to_location(long seed_value, const std::vector<ResourceMapping> &resource_mappings) {
    long result{seed_value};
    for (const auto &rm: resource_mappings) {
        result = create_mapper_fn(rm)(result);
    }
    return result;
}

std::vector<long> expand_seeds(const std::vector<long> &seeds) {
    std::vector<long> expanded;

    for (long i{}; i < seeds.size(); i += 2) {
        long start_value = seeds[i];
        long length = seeds[i + 1];

        for (long j{}; j < length; ++j) {
            expanded.push_back(start_value + j);
        }
    }

    return expanded;
}

int main() {
    const auto &lines{ReadAllLines("assets/input5.txt")};
    auto [seeds, resource_mappings] = parse_lines(lines);

    auto transformed_seeds = seeds | std::views::transform([&resource_mappings](long seed) {
        return seed_to_location(seed, resource_mappings);
    });

    std::cout << "Part 1: " << std::ranges::min(transformed_seeds) << '\n';

    const auto expanded_seeds{expand_seeds(seeds)};
    auto transformed_seeds2 = expanded_seeds | std::views::transform([&resource_mappings](long seed) {
        return seed_to_location(seed, resource_mappings);
    });
    std::cout << "Part 2: " << std::ranges::min(transformed_seeds2) << '\n';

}
