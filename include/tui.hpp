#ifndef TUI_HPP
#define TUI_HPP 

#include "system.hpp"

class Tui final {
    System *sys {nullptr};
    int pageNumber {1};
    std::vector<std::string> playerPriority;
    void welcomePage();
    void quitPage() const;
    void header() const;
    void terrorLevel(int level);
    void displayActions() const;
    void heroPhasePage(std::string_view heroName);
    void heroInfo(std::shared_ptr<HeroBase>& hero);
    void monstersInfo();
    void showNeighborsInfo(std::shared_ptr<HeroBase>& hero);
public:
    explicit Tui(System *s);
    void runGame();
};

#endif // TUI_HPP