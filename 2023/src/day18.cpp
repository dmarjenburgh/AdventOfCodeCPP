#include "lib/IOUtils.hpp"
#include "lib/matrix.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

constexpr std::array<Point, 4> DIRECTIONS{{{0, -1}, {1, 0}, {0, 1}, {-1, 0}}};

struct Edge {
    int dir;
    int length;
};

long det(const Point &p, const Point &q) {
    long x1{p.x}, y1{p.y}, x2{q.x}, y2{q.y}; // Prevent overflow
    return x1 * y2 - x2 * y1;
}

long lagoon_area(const std::vector<Edge> &edges) {
    const Point start{};
    Point cur_pos{start};
    std::vector<Point> points;
    points.push_back(start);
    bool last_rturn{(edges.back().dir + 1) % 4 == edges[0].dir};
    for (int i{}; i < edges.size() - 1; ++i) {
        Edge edge{edges[i]};
        Edge next_edge{edges[i + 1]};
        bool rturn{(edge.dir + 1) % 4 == next_edge.dir};
        int length_modifier{(last_rturn && rturn) ? 1 : ((!last_rturn && !rturn) ? -1 : 0)};
        int edge_length{edge.length + length_modifier};
        const Point next_point{cur_pos + DIRECTIONS[edge.dir] * edge_length};
        points.push_back(next_point);
        cur_pos = next_point;
        last_rturn = rturn;
    }
    points.push_back(points[0]); // Close the loop

    // Shoelace formula
    long sum{};
    for (int i{}; i < points.size() - 1; ++i) {
        sum += det(points[i], points[i + 1]);
    }
    return sum / 2;
}

std::vector<Edge> parse_input(const std::vector<std::string> &lines, bool part2) {
    std::unordered_map<char, int> dir_lookup{
            {'U', 0},
            {'R', 1},
            {'D', 2},
            {'L', 3}};

    std::vector<Edge> edges;
    char dir;
    int length;
    std::string col;
    for (const auto &line: lines) {
        std::istringstream iss{line};
        iss >> dir >> length >> col;
        if (!part2) {
            edges.emplace_back(dir_lookup.at(dir), length);
        } else {
            edges.emplace_back(
                    (col[col.size() - 2] - '0' + 1) % 4,
                    std::stoi(col.substr(2, 5), nullptr, 16)
            );
        }
    }
    return edges;
}

int main() {
    const auto lines{ReadAllLines("assets/input18.txt")};
    std::cout << "Part 1: " << lagoon_area(parse_input(lines, false)) << '\n';
    std::cout << "Part 2: " << lagoon_area(parse_input(lines, true)) << '\n';
}
