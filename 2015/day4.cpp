#include <iostream>
#include <map>
#include "hl_md5wrapper.h"


int findHashWithPrefix(const std::string &base, const std::string &prefix) {
    int i{};
    int n = prefix.size();
    md5wrapper m = md5wrapper();
    while (true) {
        const std::string &hash = m.getHashFromString(base + std::to_string(i));
        if (hash.substr(0, n) == prefix)
            return i;
        ++i;
    }
}

void day4() {
    std::string input{"yzbqklnj"};
    std::printf("Part 1: %d\n", findHashWithPrefix(input, "00000"));
    std::printf("Part 2: %d\n", findHashWithPrefix(input, "000000"));
}
