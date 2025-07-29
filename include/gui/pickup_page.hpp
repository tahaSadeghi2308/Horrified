#pragma once

#include "gui/page_base.hpp"
#include <raylib.h>

class System;
class HeroBase;

class PickupPage final : public PageBase {
    Font font;
    System *sys;
    bool isPicked;
    Rectangle panel;
    float panelW , panelH , pad , Size ;
public:
    PickupPage(const Font &f , System *s);
    void draw(std::shared_ptr<HeroBase> &cHero ,int &actions , PageNumbers &cPage) override;
    void update(std::shared_ptr<HeroBase> &cHero ,int &actions , PageNumbers &cPage) override;
};