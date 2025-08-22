#pragma once

#include "gui/page_base.hpp"
#include <raylib.h>
#include <string>
#include <vector>

class System;
class HeroBase;

class HelpPage final : public PageBase {
    Font font;
    System *sys;
    std::vector<std::string> manualLines;
    float scrollOffset;
    Rectangle backButton;
public:
    HelpPage(const Font &f , System *s);
    void draw(std::shared_ptr<HeroBase> &cHero ,int &actions , PageNumbers &cPage) override;
    void update(std::shared_ptr<HeroBase> &cHero ,int &actions , PageNumbers &cPage) override;
private:
    void loadManualFromFile(const std::string &path);
};

