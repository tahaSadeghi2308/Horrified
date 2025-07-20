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
#include "pageNumber.hpp"

class System;
class Mayor;
class Archaeologist;
class HeroBase {
    int actionCount;
    std::string heroName;
    std::vector<Item> heroItems;
    std::vector<Perk> heroPerks;
    std::shared_ptr<Place> currentPlace {nullptr};
    std::string last_played_perk_;
    Texture2D address;

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
    Texture2D getAddress() { return address;}
    virtual void speciallAction(System *sys,int &pageNumber,int& actions,const int SCREEN_WIDTH,const int SCREEN_HEIGHT);
    void setAddress(std::string ad) {address = LoadTexture(ad.c_str());}
    virtual ~HeroBase()
    {
        UnloadTexture(address);
    }
};

class Archaeologist final : public HeroBase {
public:
    Archaeologist(const int &_num , std::string_view _name, const Perk &perk);
    void speciallAction(System *sys,int &pageNumber,int& actions,const int SCREEN_WIDTH,const int SCREEN_HEIGHT) override;
};

class Mayor final : public HeroBase {
public:
    Mayor(const int &_num , std::string_view _name , const Perk &perk);
};

#endif // HERO_HPP