#ifndef TUI_HPP
#define TUI_HPP 

#include "system.hpp"

enum PageNumbers {
    EXIT_PAGE = 9,
    HERO_PHASE_PAGE = 0,
    MOVE_PAGE = 1,
    GUIDE_PAGE = 2
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
    void heroPhasePage(std::shared_ptr<HeroBase>& hero , int actions);
    void heroInfo(std::shared_ptr<HeroBase>& hero);
    void monstersInfo();
    void showNeighborsInfo(std::shared_ptr<HeroBase>& hero);
    void backButton();
    void movePage(std::shared_ptr<HeroBase>& hero , int &actions);
    void guidePage(std::shared_ptr<HeroBase>& hero , int &actions);
public:
    explicit Tui(System *s);
    void runGame();
};

#endif // TUI_HPP