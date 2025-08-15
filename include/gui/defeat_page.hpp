#pragma once

#include "gui/page_base.hpp"
#include <raylib.h>
#include <string>

class System;
class HeroBase;

class DefeatPage final : public PageBase {
    Font font;
    System *sys;
    int option , YellowPower , redPower;
    bool showErr;
    float time;
    std::string errText;
    bool PowerCheck;
public:
    DefeatPage(const Font &f , System *s);
    void draw(std::shared_ptr<HeroBase> &cHero ,int &actions , PageNumbers &cPage) override;
    void update(std::shared_ptr<HeroBase> &cHero ,int &actions , PageNumbers &cPage) override;
};