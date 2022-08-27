#include <iostream>
#include <fstream>
#include <map>
#include <regex>
#include <set>
#include <unordered_map>

enum class Effect {
    NONE,
    SHIELD,
    POISON,
    RECHARGE,
};

enum Spell {
    MAGIC_MISSILE = 53,
    DRAIN = 73,
    SHIELD = 113,
    POISON = 173,
    RECHARGE = 229,
};

class Character;

class Boss;

void applyEffect(Character&, Effect);

class Character {
public:
    Character(int hp, int armor, int mana) : m_hp(hp), m_armor(armor), m_mana(mana) {};

    virtual void applyEffects() {
        for (auto&[effect, t] : this->m_effects) {
            applyEffect(*this, effect);
            if (--t == 0) {
                std::cout << "Removing effect: " << int(effect) << '\n';
                this->m_effects.erase(effect);
                if (effect == Effect::SHIELD) {
                    this->m_armor = 0;
                }
            }
        }
    }

    bool dead() const {
        return m_hp <= 0;
    }

    int m_hp, m_armor, m_mana;
    std::unordered_map<Effect, int> m_effects;
};

void applyEffect(Character& c, Effect e) {
    switch (e) {
        case Effect::NONE:
            break;
        case Effect::SHIELD:
            c.m_armor = 7;
            break;
        case Effect::POISON:
            c.m_hp -= 3;
            break;
        case Effect::RECHARGE:
            c.m_mana += 101;
            break;
    }
}

class Boss : public Character {
public:
    Boss() : Character(0, 0, 0) {}

    Boss(int hp, int dmg) : Character(hp, 0, 0), m_dmg(dmg) {}

    int m_dmg{};

    void attack(Character& w) const {
        int dmg = m_dmg - w.m_armor;
        w.m_hp -= dmg < 1 ? 1 : dmg;
    }
};

class Wizard : public Character {
public:
    Wizard(int hp, int mana) : Character(hp, 0, mana) {}

    bool cast(Spell spell, Boss& b) {
        if (m_mana < spell) {
            // Insufficient mana
            return false;
        }
        m_mana -= spell;
        switch (spell) {
            case Spell::MAGIC_MISSILE:
                b.m_hp -= 4;
                break;
            case Spell::DRAIN:
                b.m_hp -= 2;
                m_hp += 2;
                break;
            case Spell::SHIELD:
                m_effects[Effect::SHIELD] = 6;
                break;
            case Spell::POISON:
                b.m_effects[Effect::POISON] = 6;
                break;
            case Spell::RECHARGE:
                m_effects[Effect::RECHARGE] = 5;
                break;
        }
        return true;
    }
};


Boss parse_stats(const std::string& filename) {
    auto fs = std::ifstream{filename};
    auto line = std::string{};
    auto re = std::regex{"(.*): (\\d+)"};
    auto result = Boss{};
    while (std::getline(fs, line)) {
        auto sm = std::smatch{};
        std::regex_match(line, sm, re);
        auto prop = sm[1].str();
        if (prop == "Hit Points")
            result.m_hp = std::stoi(sm[2].str());
        else if (prop == "Damage")
            result.m_dmg = std::stoi(sm[2].str());
    }
    return result;
}

bool fight(Wizard w, Boss b) {
    w.cast(Spell::RECHARGE, b);
    if (b.dead()) return true;

    w.applyEffects();
    b.applyEffects();
    if (w.dead()) return false;
    if (b.dead()) return true;
    b.attack(w);
    if (w.dead()) return false;

    w.applyEffects();
    b.applyEffects();
    w.cast(Spell::SHIELD, b);

    w.applyEffects();
    b.applyEffects();
    b.attack(w);

    w.applyEffects();
    b.applyEffects();
    w.cast(Spell::DRAIN, b);

    w.applyEffects();
    b.applyEffects();
    b.attack(w);

    w.applyEffects();
    b.applyEffects();
    w.cast(Spell::POISON, b);

    w.applyEffects();
    b.applyEffects();
    b.attack(w);

    w.applyEffects();
    b.applyEffects();
    w.cast(Spell::MAGIC_MISSILE, b);

    w.applyEffects();
    b.applyEffects();
    b.attack(w);

    if (w.dead()) return false;
    if (b.dead()) return true;
    return b.m_hp <= 0;

}

int main() {
    auto player = Wizard{50, 500};
    auto boss = parse_stats("assets/input22.txt");
    fight(Wizard(10, 250), Boss(14, 8));
    std::cin.get();
}
