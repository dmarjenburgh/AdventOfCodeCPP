#ifndef ADVENTOFCODE_IOUTILS_H
#define ADVENTOFCODE_IOUTILS_H

#include <filesystem>
#include <vector>
#include <string>

std::vector<std::string> ReadAllLines(const std::filesystem::path &path);

std::vector<int> ParseInts(const std::string &s, int num_separators = 1);

std::string ReadFile(const std::filesystem::path &path);

#endif //ADVENTOFCODE_IOUTILS_H