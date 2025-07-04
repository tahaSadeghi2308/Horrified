#ifndef TUI_HPP
#define TUI_HPP 

#include "system.hpp"

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
    int pageNumber {0};
    std::vector<std::string> playerPriority;
    void welcomePage();
    void quitPage() const;
    void header() const;
    void terrorLevel(int level);
    void displayActions() const;
    void monsterPhasePage(std::shared_ptr<MonsterBase>& monster , std::string_view cHero);
    void heroPhasePage(std::shared_ptr<HeroBase>& hero , int actions);
    void heroInfo(std::shared_ptr<HeroBase>& hero);
    void monstersInfo();
    void showNeighborsInfo(std::shared_ptr<HeroBase>& hero);
    void backButton();
    void movePage(std::shared_ptr<HeroBase>& hero , int &actions);
    void guidePage(std::shared_ptr<HeroBase>& hero , int &actions);
    void pickUpPage(std::shared_ptr<HeroBase>& hero , int &actions);
    void advancedPage(std::shared_ptr<HeroBase>& hero , int &actions);
    void defeatPage(std::shared_ptr<HeroBase>& hero , int &actions);
    void specialActionPage(std::shared_ptr<HeroBase>& hero , int &actions);
    void playPerkPage(std::shared_ptr<HeroBase>& hero , int &actions , bool &doMonsterPhase);
    void helpPage();
public:
    explicit Tui(System *s);
    void runGame();
};

#endif // TUI_HPP