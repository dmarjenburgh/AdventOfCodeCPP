#include "lib/IOUtils.hpp"
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

std::vector<std::string> ReadAllLines(const std::filesystem::path &path) {
    std::ifstream f{path};
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(f, line)) {
        lines.push_back(line);
    }
    return lines;
}

std::string ReadFile(const std::filesystem::path &path) {
    std::ifstream f{path};
    std::string str{std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>()};
    return str;
}
