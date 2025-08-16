#pragma once

#include "gui/page_base.hpp"
#include <raylib.h>
#include <string>
#include <vector>
#include <utility>

class System;
class HeroBase;

class HeroSelectionPage final : public PageBase {
    Font font;
    Texture2D bg;
    Rectangle backButton;
    Rectangle startGameButton;
    
    std::vector<std::pair<std::string, int>> &firstPri; // player name, priority
    std::vector<std::pair<std::string, std::string>> &mainPri; // player name, hero name
    System *sys;
    
    int currentPlayerIndex;
    std::string selectedHero;
    
    void addPlayerHero();
    void drawHeroOptions();
    void drawCurrentPlayerInfo();
    
public:
    explicit HeroSelectionPage(std::vector<std::pair<std::string, int>> &fp, 
                              std::vector<std::pair<std::string, std::string>> &mp, 
                              System *system);
    ~HeroSelectionPage();
    void draw(std::shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) override;
    void update(std::shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) override;
}; 