#include "lib/IOUtils.hpp"
#include <filesystem>
#include <charconv>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>

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

std::vector<int> ParseInts(const std::string &s, int num_separators) {
    std::vector<int> result;
    std::istringstream iss{s};
    int num;
    char sep;
    while (iss >> num) {
        result.push_back(num);
        for (int i{}; i < num_separators; ++i) iss >> sep;
    }
    return result;
}
