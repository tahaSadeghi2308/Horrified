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

public:
    HeroBase() = default;
    HeroBase(const int &_num , std::string_view _name , const Perk &perk);
    int getActionCount();
    void setActionCount(const int &num);
    std::string getHeroName();
    void moveAction();
    void guideAction();
    void pickUpAction();
    void advanceAction(std::vector<std::string>&,std::vector<std::string>& ,std::shared_ptr<ItemBag<Item>>& );
    void defeatAction(const std::vector<std::string>& ,const std::vector<std::string>& ,std::shared_ptr<ItemBag<Item>>&,std::shared_ptr<Dracula>&,std::shared_ptr<InvisibleMan>&);
    void runPerkCard(std::shared_ptr<Archaeologist>& , std::shared_ptr<Mayor>& ,std::shared_ptr<Dracula>& ,std::shared_ptr<InvisibleMan>&  ,std::vector<std::shared_ptr<Place>>, std::shared_ptr<ItemBag<Item>>& );
    void addPerkCard(const Perk &perk);
    void setCurrentPlace(std::shared_ptr<Place> _place); // delete & here bug possibility !!
    std::shared_ptr<Place> getCurrentPlace();
    std::vector<Item> getHeroItems();
    void addHeroItems(Item);
};

class Archaeologist final : public HeroBase {
public:
    Archaeologist(const int &_num , std::string_view _name, const Perk &perk);
    void specialAction();
};

class Mayor final : public HeroBase {
public:
    Mayor(const int &_num , std::string_view _name , const Perk &perk);
};

#endif // HERO_HPP