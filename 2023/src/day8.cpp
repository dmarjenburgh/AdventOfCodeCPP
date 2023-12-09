#include "lib/IOUtils.hpp"
#include "lib/math.hpp"
#include <string>
#include <regex>
#include <unordered_map>
#include <iostream>

using Graph = std::unordered_map<std::string, std::tuple<std::string, std::string>>;

// Returns the number of steps after which a state is repeated
int find_period(const std::string &instructions, const Graph &g, std::string node) {
    const int n{static_cast<int>(instructions.size())};
    std::string target_node;
    int target_index{-1};
    int total_steps{}, until_first_target{};
    for (int i{};; i = (i + 1) % n) {
        total_steps++;
        const auto &branches{g.at(node)};
        node = instructions[i] == 'L' ? std::get<0>(branches) : std::get<1>(branches);
        if (node.ends_with('Z')) {
            if (node == target_node && i == target_index) {
                return total_steps - until_first_target;
            } else {
                target_node = node;
                target_index = i;
                until_first_target = total_steps;
            }
        }
    }
}

int main() {
    const auto &lines{ReadAllLines("assets/input8.txt")};
    const std::string instructions{lines[0]};
    const std::regex re(R"((\w+) = \((\w+), (\w+)\))");
    Graph graph;

    for (int i{2}; i < lines.size(); ++i) {
        std::smatch sm;
        std::regex_match(lines[i], sm, re);
        graph.insert({sm[1], {sm[2], sm[3]}});
    }

    std::string current_node{"AAA"};
    int n{static_cast<int>(instructions.size())};
    int counter{};
    for (int i{};; i = (i + 1) % n) {
        counter++;
        const auto &branches{graph.at(current_node)};
        current_node = instructions[i] == 'L' ? std::get<0>(branches) : std::get<1>(branches);
        if (current_node == "ZZZ") {
            break;
        }
    }
    std::cout << "Part 1: " << counter << '\n';

    std::vector<std::string> nodes{};
    for (const auto &[k, _]: graph) {
        if (k.ends_with('A')) {
            nodes.push_back(k);
        }
    }
    std::vector<long> periods;
    periods.reserve(nodes.size());
    for (const auto &node: nodes) {
        periods.push_back(find_period(instructions, graph, node));
    }
    std::cout << "Part 2: " << lcm(periods) << '\n';
}
