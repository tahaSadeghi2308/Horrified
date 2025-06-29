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
    DEFEAT_PAGE = 6,
    SPECIALACTION_PAGE = 7,
    PLAYPERK_PAGE = 8,
    HELP_PAGE = 10,
    MONSTERPHASE_PAGE = 11
};

class Tui final {
    System *sys {nullptr};
    std::vector<std::string> playerPriority;
    int pageNumber {0};
public:
    explicit Tui(System *s);
    void header() const;
    void heroInfo(std::shared_ptr<HeroBase>& hero);
    void showNeighborsInfo(std::shared_ptr<HeroBase>& hero);
    void monstersInfo();
    void welcomePage();
    void quitPage() const;
    void displayActions() const;
    void runGame();
    void heroPhasePage(std::shared_ptr<HeroBase>& hero , int actions);
    void terrorLevel(int level);
};

#endif // !TUI_HPP