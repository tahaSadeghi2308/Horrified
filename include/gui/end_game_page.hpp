#pragma once

#include "gui/page_base.hpp"
#include <raylib.h>
#include <string>

class System;
class HeroBase;

class EndGamePage final : public PageBase {
    Font font;
    System *sys;
    int endCode;
    Rectangle exitButton;
    std::string message;
public:
    EndGamePage(const Font &f, System *s);
    void draw(std::shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) override;
    void update(std::shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) override;
    void setEndCode(int code);
};

