#include <iostream>
#include <fstream>
#include <regex>
#include <set>

void turnon(short *g, short *g2, int x0, int y0, int x1, int y1) {
    for (int x = x0; x <= x1; ++x)
        for (int y = y0; y <= y1; ++y) {
            g[y * 1000 + x] = 1;
            ++g2[y * 1000 + x];
        }
}

void turnoff(short *g, short *g2, int x0, int y0, int x1, int y1) {
    for (int x = x0; x <= x1; ++x)
        for (int y = y0; y <= y1; ++y) {
            g[y * 1000 + x] = 0;
            g2[y * 1000 + x] -= g2[y * 1000 + x] ? 1 : 0;
        }
}

void toggle(short *g, short *g2, int x0, int y0, int x1, int y1) {
    for (int x = x0; x <= x1; ++x)
        for (int y = y0; y <= y1; ++y) {
            g[y * 1000 + x] = (g[y * 1000 + x] == 0) ? 1 : 0;
            g2[y * 1000 + x] += 2;
        }
}

int sum_values(const short *g) {
    int total{};
    for (int x = 0; x < 1000; ++x)
        for (int y = 0; y < 1000; ++y) {
            total += g[y * 1000 + x];
        }
    return total;
}

int main() {
    std::ifstream input_file{"assets/input6.txt"};
    std::string line;
    short g[1000000]{};
    short g2[1000000]{};
    for (int i = 0; i < 1000000; ++i)
        if (g2[i] != 0) std::printf("ERROR! %d %d", i, g[i]);
    std::regex r{R"((turn on|toggle|turn off) (\d+),(\d+) through (\d+),(\d+))"};
    while (std::getline(input_file, line)) {
        std::cmatch m;
        std::regex_match(line.c_str(), m, r);
        int x0, y0, x1, y1;
        x0 = std::stoi(m[2]);
        y0 = std::stoi(m[3]);
        x1 = std::stoi(m[4]);
        y1 = std::stoi(m[5]);
        if (m[1] == "turn on") {
            turnon(g, g2, x0, y0, x1, y1);
        } else if (m[1] == "turn off") {
            turnoff(g, g2, x0, y0, x1, y1);
        } else if (m[1] == "toggle") {
            toggle(g, g2, x0, y0, x1, y1);
        }
    }
    input_file.close();
    std::printf("Part 1: %d\n", sum_values(g));
    std::printf("Part 2: %d\n", sum_values(g2));
}
