#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

class BITS {
  std::string _s;
  int _cur_pos{};
  unsigned _bits_left{0};
  uint32_t _v{};
  size_t _cache_size{32};

  void _load_next() {
    // Load the next set of bits
    if (_cur_pos >= _s.size()) {
      std::cerr << "Reached end of stream\n";
      return;
    }
    int end_pos = _cur_pos + 8 - static_cast<int>(_s.size());
    int post_shift{end_pos > 0 ? end_pos : 0};
    _v = std::stoul(_s.substr(_cur_pos, 8), nullptr, 16);
    _v <<= post_shift * 4;
    _cur_pos = std::min(_cur_pos + 8, static_cast<int>(_s.size()));
    _bits_left = 32;
  }

 public:
  size_t total_bits_read{};

  /**
   * Returns the next n bits in the stream in a 32-bit int
   */
  uint32_t read(unsigned n) {
    if (n > _bits_left) {
      // Split reading the next n bits into parts
      // First, read the bits in the state _v
      unsigned to_read{n - _bits_left};
      uint32_t left{_v & ~(~0u << _bits_left)};
      _load_next();
      _bits_left = _cache_size - to_read;
      // Add the remaining bits from the loaded _v
      uint32_t r{(left << to_read) | (_v >> _bits_left)};
      total_bits_read += n;
      return r;
    } else {
      uint32_t r{(_v >> (_bits_left - n)) & (~(~0u << n))};
      _bits_left -= n;
      total_bits_read += n;
      return r;
    }
  }

  friend std::istream& operator>>(std::istream& is, BITS& bits);
};

std::istream& operator>>(std::istream& is, BITS& bits) {
  std::getline(is, bits._s);
  return is;
}

uint64_t read_literal_value(BITS& bits) {
  uint64_t p{};
  do {
    uint32_t b{bits.read(5)};
    p <<= 4;
    p |= b & ~(~0u << 4);
    if (!(b >> 4)) break;
  } while (true);
  return p;
}

struct Package {
  uint32_t version;
  uint32_t type_id;
  uint32_t length_type_id;  // For operations
  uint64_t v;               // Literal value / length of subpackages / number of subpackages
  std::vector<Package> subpackages;

  Package(uint32_t version, uint32_t typeId, uint32_t length_type_id, uint64_t v,
          std::vector<Package>& subpackages)
      : version(version),
        type_id(typeId),
        length_type_id(length_type_id),
        v(v),
        subpackages(subpackages) {}

  friend std::ostream& operator<<(std::ostream& os, const Package& p) {
    p._print(os);
    return os;
  }

 private:
  // Prints the package in edn-format, LOL
  std::ostream& _print(std::ostream& os, size_t indent = 0) const {
    std::string tab(1, ' ');
    std::string ind(indent, ' ');
    os << ind << "{\n"
       << ind << tab << ":version " << version << ",\n"
       << ind << tab << ":type_id " << type_id << ",\n"
       << ind << tab << ":length_type_id " << length_type_id << ",\n"
       << ind << tab << ":v " << v << ",\n"
       << ind << tab << ":subpackages [";
    if (subpackages.empty())
      os << "]\n";
    else {
      os << '\n';
      for (const auto& sp : subpackages) sp._print(os, indent + 4);
      os << ind << tab << "]\n";
    }
    os << ind << "}\n";
    return os;
  }
};

Package read_package(BITS& bits) {
  uint32_t version{bits.read(3)};
  uint32_t type_id{bits.read(3)};
  uint64_t v;
  uint32_t length_type_id{};
  std::vector<Package> subpackages;

  if (type_id == 4) {
    v = read_literal_value(bits);
  } else {
    length_type_id = bits.read(1);
    if (!length_type_id) {
      v = bits.read(15);
      size_t initial{bits.total_bits_read};
      while ((bits.total_bits_read - initial) < v) {
        subpackages.push_back(read_package(bits));
      }
    } else {
      v = bits.read(11);
      for (uint32_t i{0}; i < v; ++i) {
        subpackages.push_back(read_package(bits));
      }
    }
  }
  return {version, type_id, length_type_id, v, subpackages};
}

size_t version_sum(const Package& p) {
  size_t sum{p.version};
  for (const auto& sp : p.subpackages) {
    sum += version_sum(sp);
  }
  return sum;
}

size_t evaluate_expression(const Package& p) {
  size_t value{};
  std::vector<size_t> svals;
  std::transform(p.subpackages.cbegin(), p.subpackages.cend(), std::back_inserter(svals),
                 evaluate_expression);

  switch (p.type_id) {
    case 0:
      value = std::accumulate(svals.cbegin(), svals.cend(), size_t{}, std::plus<>());
      break;
    case 1:
      value = std::accumulate(svals.cbegin(), svals.cend(), size_t{1}, std::multiplies<>());
      break;
    case 2:
      value = *std::min_element(svals.cbegin(), svals.cend());
      break;
    case 3:
      value = *std::max_element(svals.cbegin(), svals.cend());
      break;
    case 4:
      value = p.v;
      break;
    case 5:
      value = svals[0] > svals[1];
      break;
    case 6:
      value = svals[0] < svals[1];
      break;
    case 7:
      value = svals[0] == svals[1];
      break;
    default:
      std::cerr << "Invalid type_id: " << p.type_id << '\n';
  }
  return value;
}

int main() {
  std::ifstream file{"assets/input16.txt"};
  BITS bits;
  file >> bits;

  Package p{read_package(bits)};
  std::printf("Part 1: %lu\n", version_sum(p));
  std::printf("Part 2: %lu\n", evaluate_expression(p));
}
