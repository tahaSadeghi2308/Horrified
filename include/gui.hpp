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

class System;
class HeroBase;
class MonsterBase;
class Place;
class Villager;

class Gui final {
    System *sys {nullptr};
    //about game
    Texture2D gameMap;
    const int SCREEN_WIDTH;
    const int SCREEN_HEIGHT;
    const int RIGHT_PANEL_WIDTH = 300;
    const int LEFT_PANEL_WIDTH = 300;
    int UP_PANEL_WIDTH; 
    const int UP_PANEL_HEIGHT = 100 ;
    Font GameFont;
    Color GameColor = { 140, 255, 100, 100 };
    Color buttonColor = { 230, 225, 200, 255 };
    Texture2D coffins;
    Texture2D smahsedCoffins;
    //about game loop
    std::vector<std::string> playerPriority;
    int pageNumber {0};
    int isEnd;
    int round;
    int actions;
    bool doNextPhase;
    std::shared_ptr<HeroBase> currentHero = nullptr;
    //about place info (could be static)
    float scroll;
    std::shared_ptr<Place> selectedPlace = nullptr;
    // left panel
    Rectangle moveRec;
    Rectangle PickRec;
    Rectangle GuidRec;
    Rectangle speciallRec;
    Rectangle AdvanceRec;
    Rectangle DefeatRec;
    Rectangle PerkRec;
    Rectangle exitANDsave;
    Rectangle Help;
    Rectangle firstHero;
    Rectangle secondHero;

public:
    explicit Gui(System *s, const int ,const int);
    void run();
    void handleInput();
    void movingAsset(std::shared_ptr<Place>& destination);
    void advancedPhase(std::shared_ptr<HeroBase>& hero,int &actions);
    void drawMap();
    void defeatPhase(std::shared_ptr<HeroBase>& hero , int &actions);
    void PlaceInfo(std::shared_ptr<Place> );
    void MovePhase(std::shared_ptr<HeroBase>& hero , int &actions);
    void pickUpPhase(std::shared_ptr<HeroBase>& hero ,int &actions);
    void guidePhase(std::shared_ptr<HeroBase>& hero ,int &actions);
    void playPerkPhase(std::shared_ptr<HeroBase>& hero , int &actions , bool &doMonsterPhase );

    void drawRightPanel(std::shared_ptr<HeroBase>& hero );
    void drawUpPanel(std::shared_ptr<HeroBase>& heroInfo , int actions);
    void drawLeftPanel();
    void drawCoffin();
    void drawEvindence();

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
    // void specialActionPage(std::shared_ptr<HeroBase>& hero,int &actions);
    
    
    // void helpPage();
};

#endif 