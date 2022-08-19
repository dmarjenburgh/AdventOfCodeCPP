#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <set>
#include <sstream>
#include <vector>

using Board = std::vector<std::vector<int>>;
using Pos = std::pair<int, int>;

struct BoardState {
  Board         board;
  std::set<Pos> marked_positions;
};

std::tuple<std::vector<int>, std::vector<BoardState>> parseinput() {
  std::ifstream    file("assets/input4.txt", std::ifstream::in);
  std::string      line;
  std::vector<int> numbers;
  std::getline(file, line);
  std::stringstream ss1{line};
  std::string       n;
  while (std::getline(ss1, n, ',')) numbers.push_back(std::stoi(n));

  std::vector<BoardState> boards;
  BoardState              bs;
  while (std::getline(file, line)) {
    if (line.empty()) {
      if (!bs.board.empty()) {
        boards.emplace_back(bs);
        bs = BoardState{};
      }
      continue;
    }
    std::stringstream ss{line};

    std::vector<int> row;
    int              num;
    while (ss >> num) {
      row.push_back(num);
    }
    bs.board.push_back(row);
  }
  boards.emplace_back(bs);
  return {numbers, boards};
}

void mark_number(BoardState& bstate, const int n) {
  for (int row{0}; row < 5; ++row) {
    for (int col{0}; col < 5; ++col) {
      if (bstate.board[row][col] == n) bstate.marked_positions.insert({row, col});
    }
  }
}

bool has_bingo(const BoardState& bstate) {
  std::vector<int> range(5);
  std::iota(range.begin(), range.end(), 0);

  // Check rows
  for (int row : range) {
    std::vector<Pos> positions;
    std::transform(range.cbegin(), range.cend(), std::back_inserter(positions),
                   [row](int col) { return std::make_pair(row, col); });
    const bool bingo_found =
        std::all_of(positions.cbegin(), positions.cend(),
                    [&bstate](const Pos pos) { return bstate.marked_positions.contains(pos); });
    if (bingo_found) {
      return true;
    }
  }

  // Check columns
  for (int col : range) {
    std::vector<Pos> positions;
    std::transform(range.cbegin(), range.cend(), std::back_inserter(positions),
                   [col](int row) { return std::make_pair(row, col); });
    const bool bingo_found =
        std::all_of(positions.cbegin(), positions.cend(),
                    [&bstate](const Pos pos) { return bstate.marked_positions.contains(pos); });
    if (bingo_found) {
      return true;
    }
  }

  return false;
}

unsigned int score(const BoardState& bstate, const int last_number) {
  unsigned int     sum{};
  std::vector<int> range(5);
  std::iota(range.begin(), range.end(), 0);
  for (int row : range) {
    for (int col : range) {
      if (!bstate.marked_positions.contains({row, col})) {
        sum += bstate.board[row][col];
      }
    }
  }
  return sum * last_number;
}

std::vector<BoardState> winning_boards(std::vector<BoardState>& bstates) {
  std::vector<BoardState> result;
  std::copy_if(bstates.cbegin(), bstates.cend(), result.begin(), has_bingo);
  return result;
}

int main() {
  auto [numbers, board_states] = parseinput();
  unsigned int first_score{};
  unsigned int last_score{};
  for (const int n : numbers) {
    std::for_each(board_states.begin(), board_states.end(),
                  [n](BoardState& bs) { mark_number(bs, n); });

    // Iterator it will point to the first board state with no bingo
    auto it{std::partition(board_states.begin(), board_states.end(), has_bingo)};
    if (it != board_states.begin()) {
      const size_t d = std::distance(board_states.begin(), it);
      last_score = score(board_states[0], n);
      if (!first_score) first_score = last_score;

      // Remove boards that have bingo
      board_states = std::vector<BoardState>{it, board_states.end()};
    }
  }
  std::cout << "Part 1: " << first_score << std::endl;
  std::cout << "Part 2: " << last_score << std::endl;
}
