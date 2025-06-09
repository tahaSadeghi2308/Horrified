#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "card_manager.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include "location.hpp"

class System {
    std::shared_ptr<MonsterCardDeck<MonsterCard>> monsterDeck {nullptr};
    std::shared_ptr<ItemBag<Item>> itemBag {nullptr};
    std::shared_ptr<PerkDeck<Perk>> perkDeck {nullptr};
    // game map
    // std::unordered_map<std::string , std::shared_ptr<Place>> gameMap;

    std::vector<std::shared_ptr<Place>> allLocations;

    // TODO: create object of hero and monster by pointers
public:
    System();
    // TODO: we should create a function for saving game with destructor !!!
    // ~System();
    void showLocs() const;
    // void runMonsterPhase();
    // void runHeroPhase();
    // void runGame();
};

#endif //SYSTEM_HPP