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
#include "utility.hpp"

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
    //draculas coffins
    std::vector<std::string> coffins = {"crypt" ,"cave" ,"dungeon","graveyard"};
    //saving evidence
    std::vector<std::string> evidence = {"inn","barn","mansion","laboratory","institute"};

    //bool for hero turn if true -> arch if false -> mayor
    bool turn ;

    bool BreakOfDawn = true;

    void putVillagerInPlace(const std::string &_place , const std::string &_villName);
    char rollDice();
    int terrorLevel {0};
    std::vector<std::shared_ptr<Place>> findShortestPath(std::shared_ptr<Place> _monst);

public:
    System();
    // TODO: we should create a function for saving game with destructor !!!
    // ~System();
    void showLocs();
    void runMonsterPhase();
    void runHeroPhase();
    void runGame();
    int getTerrorLevel() const;
    void increaseTerrorLevel();
    Perk getRandomPerk();
    Item getRandomItem();
    int foundCluesCount(std::string type);
    void moveMonster(std::shared_ptr<MonsterBase> monst , std::shared_ptr<Place> newPlace);
    void moveHero(std::shared_ptr<HeroBase> her , std::shared_ptr<Place> newPlace);
    void moveVillager(std::shared_ptr<Villager> vill , std::shared_ptr<Place> newPlace);
    void killMonster(std::shared_ptr<MonsterBase> monst);
    void killVillager(std::shared_ptr<Villager> vill);
    std::vector<std::shared_ptr<MonsterBase>> getAllMonsters() { return {dracula , invisibleMan}; }    
    std::vector<std::shared_ptr<HeroBase>> getAllHeros() { return {arch , mayor}; }   
    std::vector<std::shared_ptr<Place>> getLocations() {return allLocations;} 
};

#endif //SYSTEM_HPP