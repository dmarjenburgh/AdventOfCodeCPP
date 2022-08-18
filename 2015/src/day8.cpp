#include <iostream>
#include <fstream>


int main() {
    std::ifstream input_file{"assets/2015/input8.txt"};
    std::string line;
    int code_size{};
    int mem_size{};
    int enc_size{};
    while (std::getline(input_file, line)) {
        for (auto chp{line.begin()}; chp != line.end(); ++chp) {
            if (*chp == '\\') {
                enc_size += 2;
                ++chp;
                if (*chp == 'x') {
                    enc_size += 2;
                    chp += 2;
                } else if (*chp == '"' || *chp == '\\') {
                    ++enc_size;
                }
            } else if (*chp == '"')
                ++enc_size;
            ++enc_size;
            ++mem_size;
        }
        code_size += line.size();
        mem_size -= 2;
        enc_size += 2;
    }
    std::printf("Part 1: %d\n", code_size - mem_size);
    std::printf("Part 2: %d\n", enc_size - code_size);
}
