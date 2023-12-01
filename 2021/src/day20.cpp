#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "lib/matrix.hpp"

using Pixels = std::unordered_set<Point>;

struct Image {
  Pixels pixels;
  std::string algorithm;

  int min_x;
  int max_x;
  int min_y;
  int max_y;
  friend std::ostream &operator<<(std::ostream &, const Image &);
  friend std::istream &operator>>(std::istream &, Image &);

  void step() {
    pxbuffer.clear();

    for (int r{min_y - 1}; r < max_y + 1; ++r) {
      for (int c{min_x - 1}; c < max_x + 1; ++c) {
        if (get_output_pixel(c, r) == '#')
          pxbuffer.insert({c, r});
      }
    }
    std::swap(pixels, pxbuffer);

    // Increase bounds by 1
    min_x--;
    max_x++;
    min_y--;
    max_y++;

    if (out_of_bound_px == '#')
      out_of_bound_px = *algorithm.crbegin();
    else
      out_of_bound_px = algorithm[0];
  }

  [[nodiscard]] size_t num_pixels_lit() const { return pixels.size(); }

private:
  Pixels pxbuffer;
  char out_of_bound_px;

  [[nodiscard]] char get_output_pixel(const int x, const int y) const {
    uint16_t out{};
    for (int dy : {-1, 0, 1}) {
      for (int dx : {-1, 0, 1}) {
        out <<= 1;
        if (x + dx < min_x || x + dx >= max_x || y + dy < min_y ||
            y + dy >= max_y) {
          out |= out_of_bound_px == '#';
        } else {
          out |= pixels.contains({x + dx, y + dy});
        }
      }
    }
    return algorithm[out];
  }
};

std::ostream &operator<<(std::ostream &os, const Image &im) {
  for (int c{im.min_x - 1}; c <= im.max_x; ++c)
    os << im.out_of_bound_px;
  os << '\n';
  for (int r{im.min_y}; r < im.max_y; ++r) {
    os << im.out_of_bound_px;
    for (int c{im.min_x}; c < im.max_x; ++c) {
      os << (im.pixels.contains(Point{c, r}) ? '#' : '.');
    }
    os << im.out_of_bound_px << '\n';
  }
  for (int c{im.min_x - 1}; c <= im.max_x; ++c)
    os << im.out_of_bound_px;
  os << '\n';

  return os;
}

std::istream &operator>>(std::istream &is, Image &im) {
  std::getline(is, im.algorithm);
  is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  char px;
  int r{}, c{}, max_x{};
  im.pixels.reserve(1024);
  im.pixels.max_load_factor(10);
  while (is >> std::noskipws >> px) {
    if (px == '#') {
      im.pixels.insert({c, r});
    } else if (px == '\n') {
      r++;
      max_x = std::max(max_x, c);
      c = 0;
      continue;
    }
    c++;
  }
  im.min_x = 0;
  im.max_x = max_x;
  im.min_y = 0;
  im.max_y = r;
  im.pxbuffer.reserve(im.pixels.bucket_count());
  im.pxbuffer.max_load_factor(im.pixels.max_load_factor());
  im.out_of_bound_px = '.';
  return is;
}

int main() {
  std::fstream file("assets/input20.txt");
  Image im;
  file >> im;
  for (int i{0}; i < 2; ++i) {
    im.step();
  }
  std::cout << "Part 1: " << im.num_pixels_lit() << '\n';
  for (int i{2}; i < 50; ++i) {
    im.step();
  }
  std::cout << "Part 2: " << im.num_pixels_lit() << '\n';
}
