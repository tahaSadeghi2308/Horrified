#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "card_manager.hpp"
#include "Hero.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include "location.hpp"
#include "villager.hpp"
#include "monster.hpp"

class System {
    std::shared_ptr<MonsterCardDeck<MonsterCard>> monsterDeck {nullptr};
    std::shared_ptr<ItemBag<Item>> itemBag {nullptr};
    std::shared_ptr<PerkDeck<Perk>> perkDeck {nullptr};

    // heroes 
    std::shared_ptr<Archaeologist> arch {nullptr};
    std::shared_ptr<Mayor> mayor {nullptr};
    
    // monsters
    std::shared_ptr<Dracula> dracula;
    std::shared_ptr<InvisibleMan> invisibleMan;

    // all locations
    std::vector<std::shared_ptr<Place>> allLocations;

    // all villagers
    std::vector<std::shared_ptr<Villager>> allVillagers;

    void putVillagerInPlace(const std::string &_place , const std::string &_villName);
    char rollDice();

    std::vector<std::shared_ptr<Place>> findShortestPath(std::shared_ptr<Place> _monst);

public:
    System();
    // TODO: we should create a function for saving game with destructor !!!
    // ~System();
    void showLocs();
    void runMonsterPhase();
    // void runHeroPhase();
    // void runGame();
};

#endif //SYSTEM_HPP