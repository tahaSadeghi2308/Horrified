#ifndef GUI_HPP
#define GUI_HPP


#include <string>
#include <vector>
#include <memory>
#include "raylib.h" 
#include "system.hpp"
#include <unordered_map>
#include <fmt/core.h>
#include "pageNumber.hpp"
#include <unordered_set>
#include <utility>

class System;
class HeroBase;
class MonsterBase;
class Place;
class Villager;
class PageBase;


class Gui final {
    System *sys {nullptr};
    //about game
    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;
    Font GameFont;
    Color GameColor = { 140, 255, 100, 100 };
    Color buttonColor = { 230, 225, 200, 255 };
    //about game loop
    std::vector<std::pair<std::string, int>> playerPriority;
    std::vector<std::pair<std::string, std::string>> mainPri; // player name, hero name
    PageNumbers pageNumber ;
    int isEnd;
    int round;
    int actions;
    bool doNextPhase;
    std::shared_ptr<HeroBase> currentHero = nullptr;
    //left page
    std::unordered_map<PageNumbers , std::shared_ptr<PageBase>> pages;
    // save-game toast state
    int selectedSaveFolderNumber;
    bool showSavePopup;
    double savePopupStart;
public:
    explicit Gui(System *s, const int ,const int);
    void run();
    void saveState();
    void loadState(const int folderNumber);

    ~Gui();
};

#endif 