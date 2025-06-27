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
    void displayActions(std::string_view turn) const;
public:
    explicit Tui(System *s);
    void runGame();
};

#endif // TUI_HPP