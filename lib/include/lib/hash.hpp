#ifndef ADVENTOFCODE_HASH_H
#define ADVENTOFCODE_HASH_H

#include <functional>

namespace Util {
    inline void hash_combine(std::size_t &seed, const auto &val) {
        std::hash<std::decay_t<decltype(val)>> hasher;
        seed ^= hasher(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
}

#endif //ADVENTOFCODE_HASH_H
