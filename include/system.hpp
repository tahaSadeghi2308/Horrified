#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include "card_manager.hpp"
#include "place.hpp"
#include "villager.hpp"

class System {
    // cards part !!
    PerkDeck<Perk> perks;
    ItemBag<Item> allItems;
    MonsterCardDeck<MonsterCard> monsterCards;

    // locations 
    std::vector<std::shared_ptr<Place>> allLocations;
    std::unordered_map<std::string , std::vector<std::string>> gameMap;
        
    // villagers data structure
    std::vector<Villager> allVillagers;
    void gameInit();

public:
    System();
    ~System() = default;
    void systemInfoShow() const;
};


#endif // SYSTEM_HPP