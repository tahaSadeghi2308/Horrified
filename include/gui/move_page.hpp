#pragma once

#include "gui/page_base.hpp"
#include "raylib.h"
#include <vector>

class System;
class Place;
class Villager;

class MovePage final : public PageBase {
    Font font;
    System *sys;
    bool isThereVillager;
    std::shared_ptr<Place> targetPlace;
    std::vector<std::shared_ptr<Villager>> selectedVillagers;
public:
    MovePage(
        const Font &f,
        System *s
    );
    void draw(std::shared_ptr<HeroBase> &cHero ,int &actions , PageNumbers &cPage) override;
    void update(std::shared_ptr<HeroBase> &cHero ,int &actions ,PageNumbers &cPage) override;
    void movingAsset(std::shared_ptr<Place>& destination);
};