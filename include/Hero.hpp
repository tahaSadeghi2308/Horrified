#ifndef HERO_HPP
#define HERO_HPP

#include "card_manager.hpp"
#include <vector>
#include <string>
#include <string_view>

class HeroBase {
    int actionCount;
    std::string heroName;
    std::vector<Item> heroItems;
    std::vector<Perk> heroPerks;
public:
    HeroBase() = default;
    HeroBase(const int &_num , std::string_view _name , const Perk &perk);
    int getActionCount();
    void setActionCount(const int &num);
    std::string getHeroName();
    // void moveAction();
    // void guideAction();
    // void pickUpAction();
    // void advanceAtion();
    // void defeatAction();
    // void runPerkCard();
    void addPerkCard(const Perk &perk);
};

class Archaeologist final : public HeroBase {
public:
    Archaeologist(const int &_num , std::string_view _name, const Perk &perk);
    // void specialAction();
};

class Mayor final : public HeroBase {
public:
    Mayor(const int &_num , std::string_view _name , const Perk &perk);
};

#endif // HERO_HPP