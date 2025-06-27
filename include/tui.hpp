#ifndef TUI_HPP
#define TUI_HPP 

#include "system.hpp"

class Tui final {
    System *sys {nullptr};
    std::vector<std::string> playerPriority;
    void welcomePage();
public:
    explicit Tui(System *s);
    void runGame();
};

#endif // TUI_HPP