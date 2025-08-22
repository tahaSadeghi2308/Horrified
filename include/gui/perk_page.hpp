#pragma once

#include "gui/page_base.hpp"
#include <raylib.h>
#include <string>
#include <vector>

class System;
class HeroBase;
class Place;
class Villager;

class PerkPage final : public PageBase {
    Font font;
    System *sys;
    int count;
    bool found;
    std::string selectedPerk;
    bool& doMonsterPhase;
    bool isThereVillager;
    std::shared_ptr<Place> targetPlace;
    std::vector<std::shared_ptr<Villager>> selectedVillagers;
public:
    PerkPage(const Font &f , System *s , bool &doPhase);
    void draw(std::shared_ptr<HeroBase> &cHero ,int &actions , PageNumbers &cPage) override;
    void update(std::shared_ptr<HeroBase> &cHero ,int &actions , PageNumbers &cPage) override;
    void movingAsset(std::shared_ptr<Place>& destination);
};