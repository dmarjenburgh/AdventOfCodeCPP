#include <iostream>
#include <fstream>
#include <map>
#include <regex>
#include <array>
#include <sstream>

using distmap = std::map<std::string, int>;

int distance(const std::array<std::string, 8> &cities, distmap &distances) {
    int total{};
    for (const auto *p = cities.begin() + 1; p != cities.end(); ++p) {
        total += distances[{*(p - 1) + *p}];
    }
    return total;
}

int shortest_distance{0};

void permute(std::array<std::string, 8> &cities, int k, distmap &m) {
    if (k == 7) {
        int d = distance(cities, m);
        if (d < shortest_distance || shortest_distance == 0)
            shortest_distance = d;
        return;
    }
    for (int i = k; i < 8; ++i) {
        std::swap(cities[k], cities[i]);
        permute(cities, k + 1, m);
        std::swap(cities[k], cities[i]);
    }
}

int main() {
    std::ifstream input_file{"assets/2015/input9.txt"};
    std::string line{};
    std::map<std::string, int> distances;
    std::array<std::string, 8> cities{};
    std::vector<std::string> lines;
    int i{};
    while (std::getline(input_file, line)) {
        lines.push_back(line);
    }
    input_file.close();
    for (const std::string &l : lines) {
        std::istringstream s{l};
        std::string c1, to, c2, eq;
        int dist;
        s >> c1 >> to >> c2 >> eq >> dist;

        distances[c1 + c2] = dist;
        cities[i++] = c1;
    }
    std::cout << i << cities[0] << std::endl;

    permute(cities, 0, distances);
    std::printf("Part 1: %d\n", shortest_distance);
//    std::printf("Part 2: %d\n", enc_size - code_size);
}
