#pragma once

#include "gui/page_base.hpp"
#include <raylib.h>

class System;
class HeroBase;
class Villager;
class Place;

class GuidePage final : public PageBase {
    Font font;
    System *sys;
    int option;
    bool selected;
    std::shared_ptr<Villager> selectedVill {nullptr};
    std::shared_ptr<Place> choosenPlace {nullptr};
public:
    GuidePage(const Font &f , System *s);
    void draw(std::shared_ptr<HeroBase> &cHero ,int &actions , PageNumbers &cPage) override;
    void update(std::shared_ptr<HeroBase> &cHero ,int &actions , PageNumbers &cPage) override;
    void movingAsset(std::shared_ptr<Place> &destination);
};