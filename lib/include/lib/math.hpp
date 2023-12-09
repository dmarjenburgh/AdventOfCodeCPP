#ifndef ADVENTOFCODE_MATH_H
#define ADVENTOFCODE_MATH_H

#include <vector>
#include <iterator>

long gcd(long a, long b);

long lcm(long a, long b);

template<typename T>
long lcm(const T &arr) {
    long ans = *arr.begin();
    for (auto it = std::next(arr.begin()); it != arr.end(); it++) {
        ans = lcm(*it, ans);
    }
    return ans;
}

#endif //ADVENTOFCODE_MATH_H
