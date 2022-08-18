#include <iostream>
#include <vector>

std::pair<int, int> split(int p, int n) {
    int exp = 1;
    for (; n % p == 0; n /= p) exp *= p;
    return std::make_pair(exp, n);
}

int sum_divisors(int n) {
    static std::vector<int> cache{1, 1};
    for (int i{2}; i < n; ++i)
        if (n % i == 0) {
            auto p1 = n / i;
            auto[hp, rem_n] = split(i, n);
            auto result = cache[p1] + hp * cache[rem_n];
            cache.push_back(result);
            return result;
        }
    cache.push_back(n + 1);
    return n + 1;
}

int main() {
    const auto input = 29000000;
    const auto target = input / 10;
    for (int i{2}; i < target; ++i) {
        if (sum_divisors(i) >= target) {
            std::cout << "Part 1: " << i << '\n';
            break;
        }
    }
    constexpr int NUM_HOUSES = 1000000;
    int housenumbers[NUM_HOUSES] = {};
    for (int elf = 1; elf < input; ++elf) {
        int visits = 0;
        const int num_presents = 11 * elf;
        for (int house = elf; house < NUM_HOUSES; house += elf) {
            housenumbers[house] += num_presents;
            if (++visits == 50)
                break;
        }
        if (housenumbers[elf] >= input) {
            std::cout << "Part 2: " << elf << '\n';
            break;
        }
    }
    return 0;
}
