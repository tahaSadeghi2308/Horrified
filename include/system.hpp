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
#include <string_view>

class Place;

// #include "monster.hpp" CIRCULAR DEPENDENCY 

enum SearchType { 
    ETO  // eneemy to others
};

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
    std::vector<std::shared_ptr<Villager>> allVillagers;

    // monsters
    std::vector<std::shared_ptr<MonsterBase>> monsters;

    void gameInit();
    
public:
    System();
    ~System() = default;
    void run();
    void moveVillager(std::string_view villName , std::string_view _newPlace);
    void placeWithMaxItem();
    void systemInfoShow();
    Item getRandomItem();
    MonsterCard getRandomMonstCard();
    void putItemInPlace(const std::string& _placeName , const Item &i);
    void moveMonster(std::string_view _monsterName , std::string_view _newPlace);
    std::vector<std::string> findPath(std::string source , SearchType type);
};


#endif // SYSTEM_HPP