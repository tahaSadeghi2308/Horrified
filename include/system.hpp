#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "card_manager.hpp"
#include "Hero.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include "location.hpp"
#include "villager.hpp"

class System {
    std::shared_ptr<MonsterCardDeck<MonsterCard>> monsterDeck {nullptr};
    std::shared_ptr<ItemBag<Item>> itemBag {nullptr};
    std::shared_ptr<PerkDeck<Perk>> perkDeck {nullptr};

    // heroes 
    std::shared_ptr<Archaeologist> arch {nullptr};
    std::shared_ptr<Mayor> mayor {nullptr};
    
    std::vector<std::shared_ptr<Place>> allLocations;

    // all villagers
    std::vector<std::shared_ptr<Villager>> allVillagers;

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