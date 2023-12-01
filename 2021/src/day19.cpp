#include <array>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>

template <class T, size_t L> struct Vector {
  std::array<T, L> elems;

  T &operator[](size_t i) { return elems[i]; }
  const T &operator[](size_t i) const { return elems[i]; }
  bool operator<(const Vector &w) const { return this->elems < w.elems; }
  bool operator>(const Vector &w) const { return this->elems > w.elems; }
  bool operator==(const Vector &w) const { return this->elems == w.elems; }
  Vector operator+(const Vector &w) const {
    std::array<T, L> res{};
    std::transform(elems.cbegin(), elems.cend(), w.elems.cbegin(), res.begin(),
                   std::plus<>());
    return {res};
  }
  void operator+=(const Vector &w) {
    for (size_t i{0}; i < L; ++i) {
      elems[i] += w[i];
    }
  }
  Vector operator-(const Vector &w) const {
    std::array<T, L> res{};
    std::transform(elems.cbegin(), elems.cend(), w.elems.cbegin(), res.begin(),
                   std::minus<>());
    return {res};
  }

  Vector operator-() const {
    std::array<T, L> res{};
    std::transform(elems.cbegin(), elems.cend(), res.begin(), std::negate<>());
    return {res};
  }

  friend std::ostream &operator<<(std::ostream &os, const Vector &vector);
  friend std::istream &operator>>(std::istream &is, Vector<T, L> &vector);
};

using Vec = Vector<int, 3>;
using Scanner = std::vector<Vec>;

template <class T, size_t L>
std::ostream &operator<<(std::ostream &os, const Vector<T, L> &vector) {
  os << "[ ";
  for (const auto &x : vector.elems) {
    os << x << ' ';
  }
  os << "]";
  return os;
}

std::ostream &operator<<(std::ostream &os, const Vec &vector) {
  os << "[ ";
  for (const auto &x : vector.elems) {
    os << x << ' ';
  }
  os << "]";
  return os;
}

template <class T, size_t L>
std::istream &operator>>(std::istream &is, Vector<T, L> &vector) {
  return is;
}

std::istream &operator>>(std::istream &is, Vec &vector) {
  char delim;
  is >> vector.elems[0] >> delim >> vector.elems[1] >> delim >> vector.elems[2];
  return is;
}

std::istream &operator>>(std::istream &is, Scanner &s) {
  std::string line;
  while (std::getline(is, line)) {
    if (line.empty()) {
      return is;
    } else if (line.starts_with("--")) {
      continue;
    }
    Vec v;
    std::stringstream{line} >> v;
    s.push_back(v);
  }
  return is;
}

std::ostream &operator<<(std::ostream &os, const Scanner &s) {
  os << "--- scanner ---\n";
  for (const auto &v : s) {
    os << v << '\n';
  }
  return os;
}

template <class T, size_t M, size_t N> class Matrix {
  std::array<int, M * N> _elems{};

public:
  constexpr static Matrix identity() {
    static_assert(M == N, "Identity matrix must be a square matrix");
    Matrix m;
    for (int i{0}; i < M; ++i) {
      m(i, i) = 1;
    }
    return m;
  }

  constexpr T &operator()(size_t m, size_t n) { return _elems[m * N + n]; }
  constexpr T operator()(size_t m, size_t n) const {
    return _elems[m * N + n];
  };

  Matrix operator+(const Matrix &m) {
    Matrix<T, M, N> sum;
    for (int i{0}; i < M; ++i) {
      for (int j{0}; j < N; ++j) {
        sum(i, j) = (*this)(i, j) + m(i, j);
      }
    }
    return sum;
  };

  template <size_t K, size_t L>
  constexpr Matrix<T, M, L> operator*(const Matrix<T, K, L> &m) const {
    static_assert(N == K, "Matrix dimensions don't match for multiplication");
    Matrix<T, M, L> product;
    for (int i{0}; i < M; ++i) {
      for (int j{0}; j < L; ++j) {
        product(i, j) = [&]() -> T {
          T sum{};
          for (int k{0}; k < N; ++k)
            sum += (*this)(i, k) * m(k, j);
          return sum;
        }();
      }
    }
    return product;
  };

  Vector<T, M> operator*(const Vector<T, M> &v) const {
    Vector<T, M> res{};
    for (int r{0}; r < N; ++r) {
      for (int c{0}; c < M; ++c) {
        res.elems[r] += this->operator()(r, c) * v.elems[c];
      }
    }
    return res;
  }

  template <size_t K, size_t L>
  constexpr bool operator==(const Matrix<T, K, L> &m) const {
    return M == K and N == L and this->_elems == m._elems;
  }

  friend std::ostream &operator<<(std::ostream &os, const Matrix &matrix) {
    for (int r{0}; r < M; ++r) {
      os << "[ ";
      for (int c{0}; c < N; ++c) {
        os << matrix(r, c) << ' ';
      }
      os << "]\n";
    }
    return os;
  }
};

using Mat3D = Matrix<int, 3, 3>;

constexpr Mat3D fromCols(std::array<std::array<int, 3>, 3> cols) {
  Mat3D m;
  for (int i{0}; i < 3; ++i) {
    for (int j{0}; j < 3; ++j) {
      m(i, j) = cols[j][i];
    }
  }
  return m;
}

std::array<Mat3D, 24> rotMatrices() {
  using Col3 = std::array<std::array<int, 3>, 3>;

  const auto det = [](const Mat3D &m) {
    return m(0, 0) * (m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) +
           m(0, 1) * (m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) +
           m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0));
  };
  const auto flipsign = [](Col3 arr, std::array<bool, 3> flips) {
    for (int col{0}; col < 3; ++col) {
      if (flips[col]) {
        for (int row{0}; row < 3; ++row) {
          arr[row][col] *= -1;
        }
      }
    }
    return arr;
  };

  size_t i{};
  std::array<Mat3D, 24> r;
  for (bool flip1 : {false, true})
    for (bool flip2 : {false, true})
      for (bool flip3 : {false, true}) {
        Col3 arr{{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}};
        do {
          auto m2 = fromCols(flipsign(arr, {flip1, flip2, flip3}));
          if (det(m2) == 1) {
            r[i] = m2;
            i++;
          }
        } while (
            std::next_permutation(arr.begin(), arr.end(), std::greater<>()));
      }
  return r;
}

static std::array<Mat3D, 24> isometries{rotMatrices()};

Scanner operator*(const Mat3D &r, const Scanner &s) {
  Scanner res;
  res.reserve(s.size());
  std::transform(s.cbegin(), s.cend(), std::back_inserter(res),
                 [&r](const auto &p) { return r * p; });
  return res;
}

// This allows out = r * s without allocating memory in the *-operator. Could
// not find a simple different way.
void transform_scanner(const Mat3D &r, const Scanner &s, Scanner &out) {
  out.resize(s.size());
  std::transform(s.cbegin(), s.cend(), out.begin(),
                 [&r](const auto &p) { return r * p; });
}

void transform_scanner(const Vec &v, const Scanner &s, Scanner &out) {
  out.resize(s.size());
  std::transform(s.cbegin(), s.cend(), out.begin(),
                 [&v](const auto &p) { return v + p; });
}

Scanner operator-(const Scanner &s, const Vec &v) {
  Scanner t;
  t.reserve(s.size());
  std::transform(s.cbegin(), s.cend(), std::back_inserter(t),
                 [&v](const auto &w) { return w - v; });
  return t;
}
Scanner operator+(const Scanner &s, const Vec &v) {
  Scanner t;
  t.reserve(s.size());
  std::transform(s.cbegin(), s.cend(), std::back_inserter(t),
                 [&v](const auto &w) { return w + v; });
  return t;
}

void operator+=(Scanner &s, const Vec &w) {
  for (auto &v : s) {
    v += w;
  }
}

// This functions dominates execution time
bool common_points(const Scanner &s1, const Scanner &s2) {
  size_t i{}, j{}, n{};

  while (i < s1.size() && j < s2.size() && n < 12) {
    if (s1[i] == s2[j]) {
      n++;
      i++;
      j++;
    } else if (s1[i] < s2[j]) {
      i++;
    } else {
      j++;
    }
  }
  return n == 12;
}

std::optional<Vec> match_beacons(const Scanner &s1, const Scanner &s2) {
  static Scanner s1c, s2c, s2tl; // attempt to re-use allocated memory
  static Vec d;
  s1c = s1;
  s2c = s2;
  std::partial_sort_copy(s1.begin(), s1.end(), s1c.begin(), s1c.end());
  std::partial_sort_copy(s2.begin(), s2.end(), s2c.begin(), s2c.end());

  for (size_t i{0}; i < s1.size(); ++i) {
    for (size_t j{0}; j < s2.size(); ++j) {
      d = s1c[i] - s2c[j]; // This is the vector from s1 to s2, in s1's frame
      s2tl = s2c;
      s2tl += d;
      if (common_points(s1c, s2tl)) {
        // s1[i] and s2[j] are the same beacon.
        return d;
      }
    }
  }
  return std::nullopt;
}

/**
 * Returns the displacement vector d from s1 to s2 if found.
 * We mutate s2 into the coordinate-frame of s1 when the beacons overlap.
 * If v1 in s1 and v2 in s2 point to the same beacon, then d = v1 - r*v2
 * where r is the rotation matrix.
 */
std::optional<Vec> match_scanners(const Scanner &s1, Scanner &s2) {
  static Scanner rotated_sc2; // Re-use allocated memory across invocations
  for (const auto &r : isometries) {
    transform_scanner(r, s2, rotated_sc2);
    const auto m{match_beacons(s1, rotated_sc2)};
    if (m.has_value()) {
      transform_scanner(m.value(), rotated_sc2, rotated_sc2);
      std::swap(s2, rotated_sc2);
      return m.value();
    }
  }
  return std::nullopt;
}

std::vector<Scanner> parse_input(const std::string &fname) {
  std::fstream file{fname};
  std::string line;
  std::vector<Scanner> scanners;
  scanners.reserve(10);
  Scanner sc;
  while (file >> sc) {
    scanners.push_back(std::move(sc));
  }
  scanners.push_back(std::move(sc));

  return scanners;
}

template <class T, size_t L>
size_t manhattan_dist(const Vector<T, L> &v, const Vector<T, L> &w) {
  size_t d{};
  for (size_t i{}; i < L; ++i) {
    d += abs(v[i] - w[i]);
  }
  return d;
}

int main() {
  std::vector<Scanner> scanners{parse_input("assets/input19.txt")};

  std::vector<bool> processed(scanners.size());
  std::stack<size_t> stack;
  processed[0] = true;
  stack.push(0);
  std::vector<Vec> scanner_positions(scanners.size());
  scanner_positions[0] = {0, 0, 0};
  while (!stack.empty()) {
    size_t si{stack.top()};
    const Scanner &s{scanners[si]};
    stack.pop();
    for (int i{0}; i < scanners.size(); ++i) {
      if (!processed[i]) {
        if (const auto &d{match_scanners(s, scanners[i])}; d.has_value()) {
          processed[i] = true;
          stack.push(i);
          scanner_positions[i] = d.value();
        }
      }
    }
  }
  // Merge all
  Scanner all_beacons;
  all_beacons.reserve(scanners[0].size() * scanners.size());
  for (const auto &s : scanners) {
    std::copy(s.begin(), s.end(), std::back_inserter(all_beacons));
  }
  std::sort(all_beacons.begin(), all_beacons.end());
  auto last = std::unique(all_beacons.begin(), all_beacons.end());
  all_beacons.erase(last, all_beacons.end());

  std::printf("Part 1: %lu\n", all_beacons.size());

  size_t max_dist{};
  for (size_t i{}; i < scanner_positions.size(); ++i) {
    for (size_t j{i + 1}; j < scanner_positions.size(); ++j) {
      const auto d{manhattan_dist(scanner_positions[i], scanner_positions[j])};
      if (d >= max_dist)
        max_dist = d;
    }
  }
  std::printf("Part 2: %lu\n", max_dist);
}
