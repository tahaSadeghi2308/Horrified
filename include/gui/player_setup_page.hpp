#pragma once

#include "gui/page_base.hpp"
#include <raylib.h>
#include <string>
#include <vector>
#include <utility>

class System;
class HeroBase;

class PlayerSetupPage final : public PageBase {
    Font font;
    Texture2D bg;
    Rectangle nameInputBox;
    Rectangle numberInputBox;
    Rectangle submitButton;
    Rectangle backButton;
    Rectangle startGameButton;
    
    std::string playerName;
    std::string playerNumber;
    bool nameActive;
    bool numberActive;
    
    std::vector<std::pair<std::string, int>> &players;
    
    void validateAndAddPlayer();
    bool isValidName(const std::string& name);
    bool isValidNumber(const std::string& number);
    
public:
    explicit PlayerSetupPage(std::vector<std::pair<std::string, int>> &pri);
    ~PlayerSetupPage();
    void draw(std::shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) override;
    void update(std::shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) override;
}; 