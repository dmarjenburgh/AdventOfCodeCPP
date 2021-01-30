#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <vector>
#include <numeric>

struct Reindeer {
    int speed;
    int duration;
    int rest;
};

struct ReinderState {
    enum { running = 0, resting = 1 } action;
    int t_left;
    int distance;
    int points;
};

template<typename T, typename U>
U max_fn(const T* states, int to, U func(U, T)) {
    return std::accumulate(&states[0], &states[to], 0, func);
}

int max_dist(int acc, ReinderState r) { return std::max(acc, r.distance); }
int max_points(int acc, ReinderState r) { return std::max(acc, r.points); }

std::pair<int, int> race(const std::vector<Reindeer>& reindeers, int t_end) {
    ReinderState states[reindeers.size()];
    for (int i{}; i < reindeers.size(); ++i) {
        states[i].action = ReinderState::running;
        states[i].t_left = reindeers[i].duration;
        states[i].distance = 0;
        states[i].points = 0;
    }
    for (int t{0}; t < t_end; ++t) {
        for (int i{}; i < reindeers.size(); ++i) {
            if (states[i].t_left) {
                if (states[i].action == ReinderState::running)
                    states[i].distance += reindeers[i].speed;
            } else {
                if (states[i].action == ReinderState::running) {
                    states[i].action = ReinderState::resting;
                    states[i].t_left = reindeers[i].rest;
                } else {
                    states[i].action = ReinderState::running;
                    states[i].t_left = reindeers[i].duration;
                    states[i].distance += reindeers[i].speed;
                }
            }
            --states[i].t_left;
        }
        int mxdist{max_fn(states, reindeers.size(), max_dist)};
        for (auto& st : states) {
            if (st.distance == mxdist)
                ++st.points;
        }
    }
    int mxdist{max_fn(states, reindeers.size(), max_dist)};
    int mxpoints{max_fn(states, reindeers.size(), max_points)};
    return {mxdist, mxpoints};
}

int main() {
    std::ifstream input_file{"assets/2015/input14.txt"};
    std::string line;
    std::regex r{R"(\w+ can fly (\d+) km/s for (\d+) seconds, but then must rest for (\d+) seconds.)"};
    std::vector<Reindeer> reindeers{};
    while (std::getline(input_file, line)) {
        std::smatch sm;
        std::regex_match(line, sm, r);
        reindeers.emplace_back(Reindeer{std::stoi(sm[2]), std::stoi(sm[3]), std::stoi(sm[4])});
    }
    auto[p1, p2]=race(reindeers, 2503);
    std::cout << "Part 1: " << p1 << std::endl;
    std::cout << "Part 2: " << p2 << std::endl;
}
