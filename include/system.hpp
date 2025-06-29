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
    ETO, // eneemy to others
    ETV // to villager only
};

class System {
    int terrorLevel {0};
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

    // monsters clue and coffins
    bool foundCoffin[4] = {false , false , false , false};
    bool foundClue[6] = {false , false , false , false , false , false};
    void gameInit();
    
public:
    System();
    ~System() = default;
    void run();
    char rollDice() const;
    void moveVillager(std::string_view villName , std::string_view _newPlace);
    void placeWithMaxItem();
    void systemInfoShow();
    Item getRandomItem();
    void addItem(const Item& i);
    MonsterCard getRandomMonstCard();
    Perk getRandomPerk();
    void putItemInPlace(const std::string& _placeName , const Item &i);
    void moveMonster(std::string_view _monsterName , std::string_view _newPlace);
    void moveHero(std::string_view _monsterName , std::string_view _newPlace);
    std::vector<std::string> findPath(std::string source , SearchType type);
    std::vector<std::shared_ptr<Place>>& getLocations();
    std::vector<std::shared_ptr<Villager>>& getVillagers();
    std::vector<std::shared_ptr<HeroBase>>& getHeros();
    std::vector<std::shared_ptr<MonsterBase>>& getMonsters();
    int getTerrorLevel() const;
    void increaseTerrorLevel();
    std::unordered_map<std::string , std::vector<std::string>>& getGameMap();
    bool allCluesFound(std::string_view type) const;
    void makeFoundClues(std::string_view type); 
    int foundClues(std::string_view type);
    void killMonster(std::string_view monsterName);
    void killVillager(std::string_view villName);
};

#endif // SYSTEM_HPP