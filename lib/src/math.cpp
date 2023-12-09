#include "lib/math.hpp"

long gcd(long a, long b) {
    while (b != 0) {
        long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

long lcm(long a, long b) {
    return (a * b) / gcd(a, b);
}

