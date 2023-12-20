#include "lib/IOUtils.hpp"
#include "lib/math.hpp"
#include <unordered_map>
#include <ranges>
#include <algorithm>
#include <deque>
#include <memory>
#include <iostream>

namespace rng = std::ranges;

struct Pulse {
    std::string source;
    std::string target;
    bool high{};

    auto operator<=>(const Pulse &) const = default;

};

struct Module {
    std::string name;
    std::vector<std::string> outputs;

    virtual std::vector<Pulse> receive(const Pulse &pulse) = 0;

    virtual void reset() = 0;

    virtual ~Module() = default;
};

struct Broadcaster : Module {

    std::vector<Pulse> receive(const Pulse &pulse) override {
        std::vector<Pulse> out_pulses;
        out_pulses.reserve(outputs.size());
        for (const auto &out: outputs) {
            out_pulses.emplace_back(name, out, pulse.high);
        }
        return out_pulses;
    }

    void reset() override {}
};

struct FlipFlop : Module {
    bool on{};

    std::vector<Pulse> receive(const Pulse &pulse) override {
        std::vector<Pulse> out_pulses;
        if (!pulse.high) {
            on = !on;
            for (const auto &out: outputs) {
                out_pulses.emplace_back(name, out, on);
            }
        }
        return out_pulses;
    }

    void reset() override {
        on = false;
    }
};

struct Conjunction : Module {
    std::unordered_map<std::string, bool> last_pulses;

    std::vector<Pulse> receive(const Pulse &pulse) override {
        std::vector<Pulse> out_pulses;
        last_pulses[pulse.source] = pulse.high;

        bool out_high{!rng::all_of(last_pulses, [](const auto &p) -> bool { return p.second; })};
        out_pulses.reserve(outputs.size());
        for (const auto &out: outputs) {
            out_pulses.emplace_back(name, out, out_high);
            if (out == "rx" and !out_high) {
                std::cout << out_high << ' ';
            }
        }
        return out_pulses;
    }

    void reset() override {
        for (auto &kv: last_pulses) {
            kv.second = false;
        }
    }
};

using ModulesMap = std::unordered_map<std::string, std::unique_ptr<Module>>;

void reset_modules(ModulesMap &m) {
    for (auto &[_, mod]: m) {
        mod->reset();
    }
}

std::vector<std::string> parse_outputs(const std::string &s) {
    std::vector<std::string> result;
    std::istringstream iss{s};
    std::string name;
    while (std::getline(iss, name, ',')) {
        result.push_back(name);
        iss >> std::ws;
    }
    return result;
}

ModulesMap parse_modules(const std::vector<std::string> &lines) {
    std::unique_ptr<Broadcaster> broadcaster{std::make_unique<Broadcaster>()};
    std::vector<std::unique_ptr<FlipFlop>> flip_flops;
    std::vector<std::unique_ptr<Conjunction>> conjunctions;
    for (const auto &line: lines) {
        int arrow = static_cast<int>(line.find(" -> "));
        std::string name{line.substr(0, arrow)};
        if (name == "broadcaster") {
            broadcaster->name = "broadcaster";
            broadcaster->outputs = parse_outputs(std::string{line.begin() + arrow + 4, line.end()});
        } else if (name[0] == '%') {
            auto f{std::make_unique<FlipFlop>()};
            f->name = name.substr(1);
            f->outputs = parse_outputs(std::string{line.begin() + arrow + 4, line.end()});
            flip_flops.push_back(std::move(f));
        } else if (name[0] == '&') {
            auto c{std::make_unique<Conjunction>()};
            c->name = name.substr(1);
            c->outputs = parse_outputs(std::string{line.begin() + arrow + 4, line.end()});
            conjunctions.push_back(std::move(c));
        } else {
            throw std::runtime_error("Could not parse input");
        }
    }
    ModulesMap result;

    for (auto &f: flip_flops) {
        // Set conjunction inputs
        for (const auto &out: f->outputs) {
            for (auto &c: conjunctions) {
                if (out == c->name) {
                    c->last_pulses[f->name] = false;
                }
            }
        }
        result[f->name] = std::move(f);
    }
    for (auto &c: conjunctions) {
        result[c->name] = std::move(c);
    }
    result["broadcaster"] = std::move(broadcaster);
    return result;
}

std::vector<std::string> find_inputs_for(const ModulesMap &m, const std::string &name) {
    std::vector<std::string> inputs;
    for (const auto &[n, mod]: m) {
        for (const auto &out: mod->outputs) {
            if (out == name) {
                inputs.push_back(n);
            }
        }
    }
    return inputs;
}

std::array<long, 2> push_button(ModulesMap &modules_map, const Pulse &stop_pulse) {
    std::array<long, 2> pulse_counts{};
    std::deque<Pulse> q;
    q.emplace_back("button", "broadcaster", false);
    pulse_counts[0]++;
    while (!q.empty()) {
        auto pulse = q.back();
        q.pop_back();
        if (!modules_map.contains(pulse.target)) {
            continue;
        }
        auto &target_module{modules_map.at(pulse.target)};
        auto responses{target_module->receive(pulse)};
        for (const auto &response: responses) {
            if (stop_pulse == response) {
                return {-1, -1};
            }
            pulse_counts[response.high]++;
            q.push_front(response);
        }
    }
    return pulse_counts;
}

std::array<long, 2> push_button(ModulesMap &modules_map) {
    return push_button(modules_map, Pulse{});
}

int find_period(ModulesMap &modules_map, const Pulse &stop_pulse) {
    reset_modules(modules_map);
    int num_pushes{};

    std::vector<int> hits;
    while (true) {
        ++num_pushes;
        if (push_button(modules_map, stop_pulse)[0] == -1) {
            return num_pushes;
        }
    }
}

long push_1000_times(ModulesMap &modules_map) {
    std::array<long, 2> pulse_counts{};
    for (int i{}; i < 1000; ++i) {
        const auto &res{push_button(modules_map)};
        pulse_counts[0] += res[0];
        pulse_counts[1] += res[1];
    }
    return pulse_counts[0] * pulse_counts[1];
}

long steps_to_rx_low_pulse(ModulesMap &modules_map) {
    // Find all modules having 'rx' as output
    const auto &rx_out_modules{find_inputs_for(modules_map, "rx")};
    std::string rx_out_module{rx_out_modules[0]}; // ns module in our input
    // Find all modules having the previous module as output (2-steps away from 'rx')
    const auto &rx2_out_modules{find_inputs_for(modules_map, rx_out_module)};

    // For each of the rx2_out_modules. Find the 'period'.
    std::vector<long> periods;
    for (const auto &name: rx2_out_modules) {
        Pulse stop_pulse{name, rx_out_module, true};

        int period{find_period(modules_map, stop_pulse)};
        periods.push_back(period);
    }
    return lcm(periods);
}

int main() {
    const auto lines{ReadAllLines("assets/input20.txt")};
    auto modules{parse_modules(lines)};
    std::cout << "Part 1: " << push_1000_times(modules) << '\n';
    std::cout << "Part 2: " << steps_to_rx_low_pulse(modules) << '\n';

}
