#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include "lib/IOUtils.hpp"

std::vector<std::string> ReadAllLines(const std::filesystem::path &path) {
    std::ifstream f{path};
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(f, line)) {
        lines.push_back(line);
    }
    return lines;
}