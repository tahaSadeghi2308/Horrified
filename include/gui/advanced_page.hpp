#pragma once

#include "gui/page_base.hpp"
#include <raylib.h>
#include <string>

class System;
class HeroBase;

class AdvancedPage final : public PageBase {
    Font font;
    System *sys;
    int option;
    bool showErr;
    float time;
    std::string errText;
    bool checkRedPower;
public:
    AdvancedPage(const Font &f , System *s);
    void draw(std::shared_ptr<HeroBase> &cHero ,int &actions , PageNumbers &cPage) override;
    void update(std::shared_ptr<HeroBase> &cHero ,int &actions , PageNumbers &cPage) override;
};