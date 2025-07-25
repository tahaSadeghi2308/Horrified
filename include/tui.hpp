#ifndef TUI_HPP
#define TUI_HPP

#include <string>
#include <vector>
#include <memory> 

class System;
class HeroBase;
class MonsterBase;
class Place;
class Villager;

enum PageNumbers {
    EXIT_PAGE = 9,
    HERO_PHASE_PAGE = 0,
    MOVE_PAGE = 1,
    GUIDE_PAGE = 2,
    PICKUP_PAGE = 3,
    ADVANCED_PAGE = 4,
    DEFEAT_PAGE = 5,
    SPECIALACTION_PAGE = 6,
    PLAYPERK_PAGE = 7,
    HELP_PAGE = 8,
    MONSTERPHASE_PAGE = 10
};

class Tui final {
    System *sys {nullptr};
    std::vector<std::string> playerPriority;
    int pageNumber {0};
    std::string playedMonsterCard = "This is the first round";
public:
    explicit Tui(System *s);
    void header() const;
    void heroInfo(std::shared_ptr<HeroBase>& hero);
    void showNeighborsInfo(std::shared_ptr<HeroBase>& hero);
    void monstersInfo();
    void VillagersInfo();
    void welcomePage();
    void quitPage() const;
    void displayActions() const;
    void runGame();
    void backButton();
    int monsterPhasePage(std::shared_ptr<HeroBase>& hero);
    void heroPhasePage(std::shared_ptr<HeroBase>& hero , int actions);
    void terrorLevel(int level);
    void movePage(std::shared_ptr<HeroBase>& hero , int &actions);
    void guidePage(std::shared_ptr<HeroBase>& hero ,int &actions);
    void pickUpPage(std::shared_ptr<HeroBase>& hero ,int &actions);
    void advancedPage(std::shared_ptr<HeroBase>& hero,int &actions);
    void specialActionPage(std::shared_ptr<HeroBase>& hero,int &actions);
    void playPerkPage(std::shared_ptr<HeroBase>& hero , int &actions , bool &doMonsterPhase );
    void defeatPage(std::shared_ptr<HeroBase>& hero , int &actions);
    void helpPage();
};

#endif // !TUI_HPP