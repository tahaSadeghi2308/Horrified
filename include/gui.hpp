#ifndef GUI_HPP
#define GUI_HPP


#include <string>
#include <vector>
#include <memory>
#include "raylib.h" 
#include "system.hpp"
#include <unordered_map>

class System;
class HeroBase;
class MonsterBase;
class Place;
class Villager;


class Gui final {
    System *sys {nullptr};
    std::vector<std::string> playerPriority;
    int pageNumber {0};
    std::string playedMonsterCard = "This is the first round";
    Texture2D gameMap;
    std::shared_ptr<Place> selectedPlace = nullptr;
    const int SCREEN_WIDTH = 1600;
    const int SCREEN_HEIGHT = 900;
    const int RIGHT_PANEL_WIDTH = 400;
    Font GameFont;
    float scroll;

public:
    explicit Gui(System *s);
    void run();
    void handleInput();
    //void drawRightPanel();
    void drawMap();
    //void drawPlaceInfoPanel(std::shared_ptr<Place>);
    void PlaceInfo(std::shared_ptr<Place> );
    ~Gui();

    // void header() const;
    // void heroInfo(std::shared_ptr<HeroBase>& hero);
    // void showNeighborsInfo(std::shared_ptr<HeroBase>& hero);
    // void monstersInfo();
    // void VillagersInfo();
    // void welcomePage();
    // void quitPage() const;
    // void displayActions() const;
    // void runGame();
    // void backButton();
    // int monsterPhasePage(std::shared_ptr<HeroBase>& hero);
    // void heroPhasePage(std::shared_ptr<HeroBase>& hero , int actions);
    // void terrorLevel(int level);
    // void movePage(std::shared_ptr<HeroBase>& hero , int &actions);
    // void guidePage(std::shared_ptr<HeroBase>& hero ,int &actions);
    // void pickUpPage(std::shared_ptr<HeroBase>& hero ,int &actions);
    // void advancedPage(std::shared_ptr<HeroBase>& hero,int &actions);
    // void specialActionPage(std::shared_ptr<HeroBase>& hero,int &actions);
    // void playPerkPage(std::shared_ptr<HeroBase>& hero , int &actions , bool &doMonsterPhase );
    // void defeatPage(std::shared_ptr<HeroBase>& hero , int &actions);
    // void helpPage();
};

#endif 