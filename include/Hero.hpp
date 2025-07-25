#ifndef HERO_HPP
#define HERO_HPP

#include "card_manager.hpp"
#include <vector>
#include <iostream>
#include <string_view>
#include "location.hpp"
#include <memory>
#include "villager.hpp"
#include "monster.hpp"

class Mayor;
class Archaeologist;
class HeroBase {
    int actionCount;
    std::string heroName;
    std::vector<Item> heroItems;
    std::vector<Perk> heroPerks;
    std::shared_ptr<Place> currentPlace {nullptr};
    std::string last_played_perk_;

public:
    HeroBase() = default;
    std::string getLastPlayedName() { return this->last_played_perk_; }
    void setLastPlayed(std::string_view p) { this->last_played_perk_ = p; }
    HeroBase(const int &_num , std::string_view _name , const Perk &perk);
    int getActionCount();
    void setActionCount(const int &num);
    std::string getHeroName();
    void deleteItem(std::string_view itemName);
    void addPerkCard(const Perk &perk);
    void setCurrentPlace(std::shared_ptr<Place> _place);
    std::shared_ptr<Place> getCurrentPlace();
    std::vector<Item>& getHeroItems();
    void addHeroItems(Item);
    std::vector<Perk> getHeroPerks()const { return heroPerks; }
    void deletePerk(std::string_view perkName);
    virtual ~HeroBase() = default;
};

class Archaeologist final : public HeroBase {
public:
    Archaeologist(const int &_num , std::string_view _name, const Perk &perk);
};

class Mayor final : public HeroBase {
public:
    Mayor(const int &_num , std::string_view _name , const Perk &perk);
};

#endif // HERO_HPP