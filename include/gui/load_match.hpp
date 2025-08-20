#pragma once

#include "gui/page_base.hpp"
#include <raylib.h>
#include <string>
#include <vector>

class System;
class HeroBase;

class LoadMatchPage final : public PageBase {
    Font font;
    Texture2D bg;
    Rectangle backButton;
    Rectangle startGameButton;
    std::vector<std::string> folderNames;
    int selectedIndex;
    float scrollOffset;
    int &selectedFolderNumber;
public:
    explicit LoadMatchPage(int &selectedFolderRef);
    ~LoadMatchPage();
    void draw(std::shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) override;
    void update(std::shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) override;
};
