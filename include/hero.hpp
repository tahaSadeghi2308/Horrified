#ifndef HERO_HPP
#define HERO_HPP

#include <string>
#include <string_view>
#include <vector>
#include "card_manager.hpp"
#include "system.hpp"

class HeroBase {
    System* sys {nullptr};
    std::string name;
    std::string currentPlace;
    const int actionCount;
    std::vector<Perk> perks;
    std::vector<Item> items;
public:
    HeroBase(std::string_view _name , int _actions , System *s);
    std::string getHeroName() const;
    std::string getCurrentPlace() const;
    void setCurrentPlace(std::string_view _newPlace);
    void addPerk(const Perk &p);
    void deletePerk(std::string_view perkName);
    const std::vector<Perk>& getAllPerks() const;
    void playPerk(std::string_view perkName);
    void addItem(const Item &i);
};

class Archaeologist final : public HeroBase {
public:
    Archaeologist(std::string_view _name , int _actions , System *s);
};

class Mayor final : public HeroBase {
public:
    Mayor(std::string_view _name , int _actions , System *s);
};

#endif // HERO_HPP