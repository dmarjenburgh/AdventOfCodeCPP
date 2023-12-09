#ifndef ADVENTOFCODE_IOUTILS_H
#define ADVENTOFCODE_IOUTILS_H

#include <filesystem>
#include <vector>
#include <string>

std::vector<std::string> ReadAllLines(const std::filesystem::path &path);

std::string ReadFile(const std::filesystem::path &path);

#endif //ADVENTOFCODE_IOUTILS_H