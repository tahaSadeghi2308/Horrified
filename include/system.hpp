#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include "card_manager.hpp"
#include "villager.hpp"
#include <fmt/core.h>
#include <string_view>

class Place;
class MonsterBase;
class HeroBase;

enum SearchType { 
    ETO  // eneemy to others
};

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

    // heros
    std::vector<std::shared_ptr<HeroBase>> heros;

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
    void moveHero(std::string_view _monsterName , std::string_view _newPlace);
    std::vector<std::string> findPath(std::string source , SearchType type);
    void playPerkCard(const Perk& p);
};


#endif // SYSTEM_HPP