#ifndef TUI_HPP
#define TUI_HPP 

#include "system.hpp"

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
public:
    explicit Tui(System *s);
    void runGame();
};

#endif // TUI_HPP