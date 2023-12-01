#include <fstream>
#include <iostream>
#include <map>
#include <regex>

struct Item {
  int cost, dmg, armor;
};

Item weapons[] = {{8, 4, 0}, {10, 5, 0}, {25, 6, 0}, {40, 7, 0}, {74, 8, 0}};
Item armors[] = {{0, 0, 0},  {13, 0, 1}, {31, 0, 2},
                 {53, 0, 3}, {75, 0, 4}, {102, 0, 5}};
Item rings[] = {{0, 0, 0},  {25, 1, 0}, {50, 2, 0}, {100, 3, 0},
                {20, 0, 1}, {40, 0, 2}, {80, 0, 3}};

struct Warrior {
  int hp, dmg, arm;
  Item weapon, armor, ring1, ring2;

  void equipWeapon(const Item &w) {
    weapon = w;
    setStats();
  }

  void equipArmor(const Item &a) {
    armor = a;
    setStats();
  }

  void equipRing(const Item &r, int i) {
    if (i == 1)
      ring1 = r;
    else
      ring2 = r;
    setStats();
  }

private:
  void setStats() {
    dmg = weapon.dmg + armor.dmg + ring1.dmg + ring2.dmg;
    arm = weapon.armor + armor.armor + ring1.armor + ring2.armor;
  }
};

Warrior parse_stats(const std::string &filename) {
  auto fs = std::ifstream{filename};
  auto line = std::string{};
  auto re = std::regex{"(.*): (\\d+)"};
  auto result = Warrior{};
  while (std::getline(fs, line)) {
    auto sm = std::smatch{};
    std::regex_match(line, sm, re);
    auto prop = sm[1].str();
    if (prop == "Hit Points")
      result.hp = std::stoi(sm[2].str());
    else if (prop == "Damage")
      result.dmg = std::stoi(sm[2].str());
    else if (prop == "Armor")
      result.arm = std::stoi(sm[2].str());
  }
  return result;
}

bool fight(Warrior player, Warrior boss) {
  while (true) {
    int dmg = player.dmg - boss.arm;
    boss.hp -= dmg < 1 ? 1 : dmg;
    if (boss.hp <= 0)
      return true;
    dmg = boss.dmg - player.arm;
    player.hp -= dmg < 1 ? 1 : dmg;
    if (player.hp <= 0)
      return false;
  }
}

std::pair<int, int> tryAll(Warrior &player, Warrior &boss) {
  int min_cost = 500;
  int max_cost = 0;
  for (const auto &w : weapons) {
    player.equipWeapon(w);
    for (const auto &a : armors) {
      player.equipArmor(a);
      for (const auto &r1 : rings) {
        player.equipRing(r1, 1);
        for (const auto &r2 : rings) {
          if (r1.cost == r2.cost)
            continue;
          player.equipRing(r2, 2);
          int cost = w.cost + a.cost + r1.cost + r2.cost;
          if (fight(player, boss)) {
            if (cost < min_cost)
              min_cost = cost;
          } else if (cost > max_cost)
            max_cost = cost;
        }
      }
    }
  }
  return std::make_pair(min_cost, max_cost);
}

int main() {
  auto player = Warrior{100, 0, 0};
  auto boss = parse_stats("assets/input21.txt");
  auto [min_cost, max_cost] = tryAll(player, boss);
  std::cout << "Part 1: " << min_cost << '\n';
  std::cout << "Part 2: " << max_cost << '\n';
}
