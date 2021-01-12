#include <iostream>
#include <fstream>
#include <regex>
#include <set>

using point = std::pair<int, int>;
using grid = std::set<point>;

void turnon(grid &set, int x0, int y0, int x1, int y1) {
    for (int x = x0; x <= x1; ++x)
        for (int y = y0; y <= y1; ++y)
            set.insert({x, y});
}
void turnoff(grid &set, int x0, int y0, int x1, int y1) {
    for (int x = x0; x <= x1; ++x)
        for (int y = y0; y <= y1; ++y)
            set.erase({x, y});
}
void toggle(grid &set, int x0, int y0, int x1, int y1) {
    for (int x = x0; x <= x1; ++x)
        for (int y = y0; y <= y1; ++y)
            if (set.count({x, y}))
                set.erase({x, y});
            else
                set.insert({x, y});
}


void day6() {
    std::ifstream input_file{"assets/2015/input6.txt"};
    std::string line;
    grid lights;
    std::regex r{R"((turn on|toggle|turn off) (\d+),(\d+) through (\d+),(\d+))"};

    while (std::getline(input_file, line)) {
        std::cmatch m;
        std::regex_match(line.c_str(), m, r);
        int x0, y0, x1, y1;
        x0 = std::stoi(m[2], nullptr);
        y0 = std::stoi(m[3], nullptr);
        x1 = std::stoi(m[4], nullptr);
        y1 = std::stoi(m[5], nullptr);
        if (m[1] == "turn on")
            turnon(lights, x0, y0, x1, y1);
        else if (m[1] == "turn off")
            turnoff(lights, x0, y0, x1, y1);
        else if (m[1] == "toggle")
            toggle(lights, x0, y0, x1, y1);
        else
            std::cout << m[1] << std::endl;
    }
    input_file.close();
    std::printf("Part 1: %lu", lights.size());

}
