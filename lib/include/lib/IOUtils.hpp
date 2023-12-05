#include <filesystem>
#include <vector>
#include <string>

std::vector<std::string> ReadAllLines(const std::filesystem::path &path);

std::string ReadFile(const std::filesystem::path &path);