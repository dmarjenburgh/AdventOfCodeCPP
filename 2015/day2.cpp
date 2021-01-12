#include <string>
#include <fstream>
#include <iostream>
#include <regex>

std::vector<std::tuple<int, int, int>> readInput(const std::string &fname) {
    std::ifstream input_file;
    input_file.open(fname);

    auto r = std::regex{R"((\d+)x(\d+)x(\d+))"};
    std::vector<std::tuple<int, int, int>> lines{};

    std::string line;
    while (std::getline(input_file, line)) {
        std::smatch sm;
        std::regex_match(line, sm, r);
        lines.push_back(std::make_tuple(std::stoi(sm[1]), std::stoi(sm[2]), std::stoi(sm[3])));
    }
    input_file.close();
    return lines;
}

int wrappingPaperNeeded(const std::tuple<int, int, int> &dimensions) {
    int l, w, h;
    std::tie(l, w, h) = dimensions;
    int lw = l * w;
    int wh = w * h;
    int lh = l * h;
    int minDim = lw < wh ? (lw < lh ? lw : lh) : (wh < lh ? wh : lh);
    return (2 * lw) + (2 * wh) + (2 * lh) + minDim;
}

int ribbonLengthNeeded(const std::tuple<int, int, int> &dimensions) {
    int l, w, h;
    std::tie(l, w, h) = dimensions;
    int maxDim = l > w ? (l > h ? l : h) : (w > h ? w : h);
    return 2*(l + w + h - maxDim) + l*w*h;
}

int main() {
    auto data = readInput("assets/2015/input2.txt");
    int totalWrappingPaper{0};
    int totalRibbonLength{0};
    for (const auto &line: data) {
        totalWrappingPaper += wrappingPaperNeeded(line);
        totalRibbonLength += ribbonLengthNeeded(line);
    }
    std::printf("Total wrapping paper needed: %d\n", totalWrappingPaper);
    std::printf("Total ribbon length needed: %d\n", totalRibbonLength);
}
