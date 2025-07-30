#pragma once

#include "gui/page_base.hpp"
#include <raylib.h>

class System;
class HeroBase;

class WelcomePage final : public PageBase {
    Font font;
    Texture2D bg;
    Rectangle start , load , exit;
public:
    WelcomePage();
    ~WelcomePage();
    void draw(std::shared_ptr<HeroBase> &cHero ,int &actions , PageNumbers &cPage) override;
    void update(std::shared_ptr<HeroBase> &cHero ,int &actions , PageNumbers &cPage) override;
};