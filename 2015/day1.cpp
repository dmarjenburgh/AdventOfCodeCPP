#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream file("assets/2015/input1.txt", std::ifstream::in);
    std::string line;
    std::getline(file, line);
    int floor{};
    int i{};
    int firstTimeBasement{};
    for (const auto &c : line) {
        i++;
        floor += c == '(' ? 1 : -1;
        if (floor < 0 && !firstTimeBasement)
            firstTimeBasement = i;
    }
    std::printf("Level: %d\n",floor);
    std::printf("First time basement: %d\n",firstTimeBasement);
}
