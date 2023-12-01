#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <utility>

constexpr const int SCORE_GOAL = 21;

constexpr const std::array<std::pair<int, int>, 7> distribution = {
    {{3, 1}, {4, 3}, {5, 6}, {6, 7}, {7, 6}, {8, 3}, {9, 1}}};

uint64_t deterministic_dice_game(int p1, int p2) {
  std::array<int, 2> scores{}, positions{p1, p2};
  int die{1}, rolls{}, turn{};
  while (scores[0] < 1000 and scores[1] < 1000) {
    positions[turn] = (positions[turn] + 3 * (die + 1)) % 10;
    die = ((die + 2) % 100) + 1;
    rolls += 3;
    scores[turn] += positions[turn] + 1;
    turn = 1 - turn;
  }

  if (scores[0] >= 1000) {
    return scores[1] * rolls;
  } else {
    return scores[0] * rolls;
  }
}

/**
 * Returns the number of ways the player at a given position will reach the goal
 * within a given number of turns and the number of ways the player does not
 * reach the goal within that number of turns.
 */
std::pair<uint64_t, uint64_t> count_possibilities(int position, int points_left,
                                                  int turns_left) {
  if (!turns_left)
    return points_left <= 0 ? std::make_pair(1, 0) : std::make_pair(0, 1);
  if (points_left <= 0)
    return {1, 0};

  std::pair<uint64_t, uint64_t> total_cnt{};
  for (auto [v, n] : distribution) {
    const int new_pos{(position + v) % 10};
    const int score{new_pos + 1};
    const auto cnt{
        count_possibilities(new_pos, points_left - score, turns_left - 1)};
    total_cnt.first += n * cnt.first;
    total_cnt.second += n * cnt.second;
  }
  return total_cnt;
}

int main() {
  std::fstream file("assets/input21.txt");
  int p1, p2;
  file.ignore(64, ':');
  file >> p1;
  file.ignore(64, ':');
  file >> p2;
  // Index spaces from 0 to 9
  p1--;
  p2--;

  std::cout << "Part 1: " << deterministic_dice_game(p1, p2) << '\n';

  std::array<int, 12> range{};
  std::iota(range.begin(), range.end(), 0);
  std::array<std::pair<uint64_t, uint64_t>, 12> branches1{};
  std::array<std::pair<uint64_t, uint64_t>, 12> branches2{};
  std::transform(range.cbegin(), range.cend(), branches1.begin(), [&p1](int n) {
    return count_possibilities(p1, SCORE_GOAL, n);
  });
  std::transform(range.cbegin(), range.cend(), branches2.begin(), [&p2](int n) {
    return count_possibilities(p2, SCORE_GOAL, n);
  });

  uint64_t p1_wins{}, p2_wins{};
  for (const auto t : range) {
    if (t > 0) {
      // Ways in which p1 reaches >= 21 in t turns and p2 doesn't in < t turns
      p1_wins += (branches1[t].first - branches1[t - 1].first) *
                 branches2[t - 1].second;
      // Ways in which p2 reaches >= 21 in t turns and p1 doesn't in <= t turns
      p2_wins +=
          (branches2[t].first - branches2[t - 1].first) * branches1[t].second;
    }
  }
  std::cout << "Part 2: " << std::max(p1_wins, p2_wins) << '\n';
}
