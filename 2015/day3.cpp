#include <iostream>
#include <fstream>
#include <map>

using pos = std::pair<int, int>;

static int part1(const std::string &line) {
    int x{}, y{};
    std::map<pos, int> houses;
    houses[{x, y}] = 1;
    for (char c: line) {
        if (c == '^')
            y += 1;
        else if (c == '>')
            x += 1;
        else if (c == 'v')
            y -= 1;
        else if (c == '<')
            x -= 1;

        if (!houses[{x, y}])
            houses[{x, y}] = 0;
        houses[{x, y}] += 1;
    }
    return houses.size();
}

static int part2(const std::string &line) {
    int sx{}, sy{}, rx{}, ry{};
    std::map<pos, int> houses;
    houses[{sx, sy}] = 2;
    for (int i=0;i<line.size();i++) {
        const char c = line[i];
        int& x = i%2 == 0 ? sx : rx;
        int& y = i%2 == 0 ? sy : ry;
        if (c == '^')
            y += 1;
        else if (c == '>')
            x += 1;
        else if (c == 'v')
            y -= 1;
        else if (c == '<')
            x -= 1;

        if (!houses[{x, y}])
            houses[{x, y}] = 0;
        houses[{x, y}] += 1;
    }
    return houses.size();
}

int main() {
    std::string line;
    std::ifstream input_file{"assets/2015/input3.txt"};
    std::getline(input_file, line);
    input_file.close();

    int numHouses = part1(line);
    std::printf("Part 1: %d\n", numHouses);
    numHouses = part2(line);
    std::printf("Part 2: %d\n", numHouses);
}
