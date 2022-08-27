#include <iostream>
#include <fstream>
#include <regex>

static bool isNiceString(const std::string &s) {
    std::regex vowel_re{"[aeiou]"};
    auto vowels_start = std::sregex_iterator(s.begin(), s.end(), vowel_re);
    auto vowels_end = std::sregex_iterator();
    std::regex double_re{"(\\w)\\1"};
    std::regex forbidden_re{"ab|cd|pq|xy"};

    return std::distance(vowels_start, vowels_end) >= 3 &&
           std::regex_search(s, double_re) &&
           !std::regex_search(s, forbidden_re);
}

static bool isNiceString2(const std::string &s) {
    std::regex r1{R"((\w\w)\w*\1)"};
    std::regex r2{R"((\w)\w\1)"};

    return std::regex_search(s, r1) && std::regex_search(s, r2);
}


int main() {
    std::ifstream input_file{"assets/input5.txt"};
    std::string line;
    int nicestrings = 0;
    int nicestrings2 = 0;
    while (std::getline(input_file, line)) {
        if (isNiceString(line))
            ++nicestrings;
        if (isNiceString2(line))
            ++nicestrings2;
    }
    input_file.close();
    std::printf("Part 1: %d\n", nicestrings);
    std::printf("Part 2: %d\n", nicestrings2);
}
