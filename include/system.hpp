#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include "card_manager.hpp"
// #include "place.hpp"
#include "villager.hpp"
#include <fmt/core.h>

class Place;

// #include "monster.hpp" CIRCULAR DEPENDENCY 

class MonsterBase;

class System {
    // cards part !!
    PerkDeck<Perk> perks;
    ItemBag<Item> items;
    MonsterCardDeck<MonsterCard> monsterCards;

    // locations 
    std::vector<std::shared_ptr<Place>> allLocations;
    std::unordered_map<std::string , std::vector<std::string>> gameMap;
        
    // villagers data structure
    std::vector<Villager> allVillagers;

    // monsters
    std::vector<std::shared_ptr<MonsterBase>> monsters;

    void gameInit();

public:
    System();
    ~System() = default;
    void systemInfoShow() const;
    Item getRandomItem();
    MonsterCard getRandomMonstCard();
    void putItemInPlace(const std::string& _placeName , const Item &i);
};


#endif // SYSTEM_HPP