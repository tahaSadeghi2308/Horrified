#ifndef TUI_HPP
#define TUI_HPP

class System;
class HeroBase;
class MonsterBase;
class Place;
class Villager;

class Tui final {
    System *sys {nullptr};
    std::vector<std::string> playerPriority;
public:
    explicit Tui(System *s);
    void header() const;
    void heroInfo(std::shared_ptr<HeroBase>& hero);
    void showNeighborsInfo(std::shared_ptr<HeroBase>& hero);
    void monstersInfo();
    void welcomePage();
    void quitPage() const;
};

#endif // !TUI_HPP