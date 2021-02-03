#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <set>
#include <regex>

int num_matches(const std::string& molecule, const std::multimap<std::string, std::string>& rmap) {
    auto oss = std::ostringstream{};
    {
        auto it = rmap.begin();
        oss << it->first;
        ++it;
        for (; it != rmap.end(); ++it)
            oss << '|' << it->first;
    }

    auto r = std::regex{oss.str()};
    auto start = std::sregex_iterator{molecule.begin(), molecule.end(), r};
    auto end = std::sregex_iterator{};
    auto products = std::set<std::string>{};
    for (auto it = start; it != end; ++it) {
        auto range = rmap.equal_range(it->str());
        for (auto i = range.first; i != range.second; ++i) {
            auto s = molecule.substr(0, it->position()) + i->second + molecule.substr(it->position() + it->length());
            products.insert(s);
        }
    }
    return products.size();
}

std::string reduce(const std::string& molecule, const std::multimap<std::string, std::string>& rmap) {
    for (const auto& kv : rmap) {
        const size_t found = molecule.find(kv.second);
        if (found != std::string::npos) {
            std::ostringstream oss;
            oss << molecule.substr(0, found) << kv.first << molecule.substr(found + kv.second.length());
            return oss.str();
        }
    }
    return std::string();
}

int main() {
    std::ifstream input_file{"assets/2015/input19.txt"};
    std::string line;
    std::multimap<std::string, std::string> reactions;

    while (std::getline(input_file, line)) {
        if (line.empty()) break;
        std::istringstream s{line};
        std::string source, arrow, dest;
        s >> source >> arrow >> dest;
        reactions.insert({source, dest});
    }
    std::getline(input_file, line);
    std::string molecule{line};
    std::cout << "Part 1: " << num_matches(molecule, reactions) << '\n';
    auto steps = 0;
    while (molecule.length() > 1) {
        molecule = reduce(molecule, reactions);
        ++steps;
    }
    std::cout << "Part 2: " << steps << '\n';
}
