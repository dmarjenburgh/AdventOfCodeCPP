#include <iostream>
#include <fstream>
#include <regex>
#include <map>
#include <sstream>

enum OP {
    SET, AND, OR, LSHIFT, RSHIFT, NOT
};

struct Gate {
    OP op{};
    std::tuple<std::string, std::string> inputs{};
    std::string out{};
    ushort value{};
    bool resolved{false};
};

Gate parseline(const std::string &line) {
    std::stringstream in{line};
    std::string p1, p2, p3;

    Gate gate;
    in >> p1 >> p2 >> p3;
    if (p2 == "->") {
        gate.op = SET;
        gate.inputs = {p1, ""};
        gate.out = p3;
    } else if (p1 == "NOT") {
        gate.op = NOT;
        gate.inputs = {p2, ""};
        in >> gate.out;
    } else {
        if (p2 == "LSHIFT")
            gate.op = LSHIFT;
        else if (p2 == "RSHIFT")
            gate.op = RSHIFT;
        else if (p2 == "AND")
            gate.op = AND;
        else if (p2 == "OR")
            gate.op = OR;
        else {
            std::cerr << "Unsupported operation: " << line << "\n";
        }
        gate.inputs = {p1, p3};
        std::string arrow;
        in >> arrow >> gate.out;
    }
    return gate;
}

ushort resolve(const std::string &wire, std::map<std::string, Gate> &gates) {
    Gate &g = gates[wire];
    if (g.resolved)
        return g.value;

    auto v{std::get<0>(g.inputs)};
    auto w{std::get<1>(g.inputs)};
    ushort a = std::isalpha(v[0]) ? resolve(v, gates) : std::stoi(v);
    ushort b;
    switch (g.op) {
        case SET:
            g.value = std::isalpha(v[0]) ? resolve(v, gates) : std::stoi(v);
            break;
        case AND:
            b = std::isalpha(w[0]) ? resolve(w, gates) : std::stoi(w);
            g.value = a & b;
            break;
        case OR:
            b = std::isalpha(w[0]) ? resolve(w, gates) : std::stoi(w);
            g.value = a | b;
            break;
        case LSHIFT:
            b = std::stoi(w);
            g.value = a << b;
            break;
        case RSHIFT:
            b = std::stoi(w);
            g.value = a >> b;
            break;
        case NOT:
            g.value = ~a;
            break;
    }
    g.resolved = true;
    return g.value;
}

int main() {
    std::ifstream input_file{"assets/input7.txt"};
    std::map<std::string, Gate> gates{};
    std::string line;
    while (std::getline(input_file, line)) {
        Gate g{parseline(line)};
        gates[g.out] = g;
    }
    ushort answer1 = resolve("a", gates);

    for (auto &kv : gates) kv.second.resolved = false;
    Gate &b = gates["b"];
    b.value = answer1;
    b.resolved = true;
    ushort answer2 = resolve("a", gates);

    std::printf("Part 1: %d\n", answer1);
    std::printf("Part 2: %d\n", answer2);
}
