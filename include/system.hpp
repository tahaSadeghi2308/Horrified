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

enum SearchingType {
    ETO,
    ETV
};
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

    void putVillagerInPlace(const std::string &_place , const std::string &_villName);
    int terrorLevel {0};
    
public:
    System();
    // TODO: we should create a function for saving game with destructor !!!
    // ~System();
    std::vector<std::shared_ptr<Place>> findShortestPath(
        std::shared_ptr<Place> _place,
        SearchingType type
    );
    void showLocs();
    // void runHeroPhase();
    void runGame();
    char rollDice();
    void addItem(Item i);
    int getTerrorLevel() const;
    void increaseTerrorLevel();
    Perk getRandomPerk();
    Item getRandomItem();
    MonsterCard getRandomMonstCard();
    int isEndGame() const;
    void putItemInPlace(std::string_view placeName , Item i);
    int foundCluesCount(std::string type);
    bool destroyClue(std::string type , std::string_view clueNamePlace); 
    void moveMonster(std::shared_ptr<MonsterBase> monst , std::shared_ptr<Place> newPlace);
    void moveHero(std::shared_ptr<HeroBase> her , std::shared_ptr<Place> newPlace);
    void moveVillager(std::shared_ptr<Villager> vill , std::shared_ptr<Place> newPlace);
    void killMonster(std::shared_ptr<MonsterBase> monst);
    void killVillager(std::shared_ptr<Villager> vill);
    void placeWithMaxItem();
    std::vector<std::shared_ptr<MonsterBase>> getAllMonsters() { return {dracula , invisibleMan}; }    
    std::vector<std::shared_ptr<HeroBase>> getAllHeros() { return {arch , mayor}; }
    std::vector<std::shared_ptr<Place>> getAllLocations() { return this->allLocations; } 
    std::vector<std::shared_ptr<Villager>> getAllVillagers () { return this->allVillagers; } 
    std::vector<std::string> getCoffins() {return this->coffins; }
    std::vector<std::string> getEvidence() { return this->evidence;}
};

#endif //SYSTEM_HPP