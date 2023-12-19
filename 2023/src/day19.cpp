#include "lib/IOUtils.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <regex>

struct Part {
    std::unordered_map<char, int> values;

    [[nodiscard]] int rating() const {
        return values.at('x') + values.at('m') + values.at('a') + values.at('s');
    }
};

using PartRange = std::unordered_map<char, std::pair<int, int>>;

struct Rule {
    char attr;
    char op;
    int value;
    std::string target;
};


using Parts = std::vector<Part>;
using Workflows = std::unordered_map<std::string, std::vector<Rule>>;

std::string apply_rules(const std::vector<Rule> &rules, const Part &p) {
    auto it{std::ranges::find_if(rules, [&p](const Rule &r) {
        switch (r.op) {
            case '<':
                return p.values.at(r.attr) < r.value;
            case '>':
                return p.values.at(r.attr) > r.value;
            default:
                return true;
        }
    })};
    return it->target;
}

int execute_workflows(const Workflows &workflows, const Parts &parts) {
    int sum{};
    for (const auto &part: parts) {
        std::string current_workflow{"in"};
        std::string result;
        while (result != "A" && result != "R") {
            const std::vector<Rule> &rules{workflows.at(current_workflow)};
            result = apply_rules(rules, part);
            if (result == "A") {
                sum += part.rating();
                break;
            }
            if (result == "R") {
                break;
            }
            current_workflow = result;
        }
    }
    return sum;
}

long find_accepted_ranges(const Workflows &workflows) {
    PartRange start = PartRange{
            {{'x', {0, 4000}},
             {'m', {0, 4000}},
             {'a', {0, 4000}},
             {'s', {0, 4000}}}};
    std::stack<std::pair<std::string, PartRange>> stack;
    stack.emplace("in", start);
    long sum{};
    while (!stack.empty()) {
        auto [wf, current] = stack.top();
        stack.pop();
        if (wf == "A") {
            long combinations{1};
            for (const auto &[attr, interval]: current) {
                combinations *= (interval.second - interval.first);
            }
            sum += combinations;
            continue;
        }
        if (wf == "R") {
            continue;
        }

        // Apply rules
        const auto &rules{workflows.at(wf)};

        for (const auto &rule: rules) {
            const char attr{rule.attr};
            if (attr == '*') {
                stack.emplace(rule.target, current);
                break;
            }
            auto interval{current.at(attr)};
            if (interval.first < rule.value && rule.value <= interval.second) {
                int split_point{rule.value - (rule.op == '<')};
                std::pair<int, int> lower{interval.first, split_point};
                std::pair<int, int> upper{split_point, interval.second};

                PartRange new_range{current};
                new_range[attr] = rule.op == '<' ? lower : upper;
                stack.emplace(rule.target, new_range);
                current[attr] = rule.op == '<' ? upper : lower;
            }
        }
    }
    return sum;
}

Part parse_part(const std::string &line) {
    char attr, token;
    int val;
    std::istringstream iss{line.substr(1)};
    std::unordered_map<char, int> m;

    while (iss >> attr >> token >> val) {
        m[attr] = val;
        iss >> token;
    }
    return Part{m};
}

std::vector<Rule> parse_rules(const std::string &line) {
    const std::regex rule_regex{R"(\w([<>])(\d+):(\w+))"};
    std::istringstream iss{line};
    std::string rule;
    std::vector<Rule> rules;
    while (std::getline(iss, rule, ',')) {
        std::smatch sm;
        if (std::regex_match(rule, sm, rule_regex)) {
            char attr{rule[0]};
            char op_char{sm[1].str()[0]};
            int val{std::stoi(sm[2])};
            std::string target{sm[3].str()};
            rules.emplace_back(attr, op_char, val, target);
        } else {
            rules.emplace_back('*', '0', 0, rule);
        }
    }
    return rules;
}

std::pair<Workflows, Parts> parse_input(const std::vector<std::string> &lines) {

    Workflows workflows;
    Parts parts;
    int line_nr{};
    for (const auto &line: lines) {
        if (line.empty()) {
            break;
        }
        size_t lbrace{line.find('{')};
        std::string name{line.substr(0, lbrace)};

        workflows[name] = parse_rules(std::string{line.begin() + (long) lbrace + 1, line.end() - 1});
        ++line_nr;
    }


    for (int i{line_nr + 1}; i < lines.size(); ++i) {
        parts.emplace_back(parse_part(lines[i]));
    }
    return {workflows, parts};
}

int main() {
    const auto lines{ReadAllLines("assets/input19.txt")};
    auto [workflows, parts] = parse_input(lines);
    std::cout << "Part 1: " << execute_workflows(workflows, parts) << '\n';
    std::cout << "Part 2: " << find_accepted_ranges(workflows) << '\n';

}