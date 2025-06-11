#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "card_manager.hpp"
#include "Hero.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include "location.hpp"
#include "villager.hpp"
#include <map>

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

    //draculas coffins
    std::vector<std::string> coffins = {"crypt" ,"cave" ,"dungeon","graveyard"};
    //ready to defeat invisible man
    bool defeatInvisableMan; 
    //saving evidence
    std::map<std::string,bool> evidence;
     
public:
    System();
    // TODO: we should create a function for saving game with destructor !!!
    // ~System();
    void showLocs() const;
    // void runMonsterPhase();
    // void runHeroPhase();
    // void runGame();
    void advance()
    {
        
        arch->addHeroItems(Item{2, Color::RED, "Red Item A", "Cave"});
        arch->addHeroItems(Item{4, Color::RED, "Red Item B", "Cave"});
        arch->addHeroItems(Item{4, Color::RED, "Red Item c", "Cave"});
        arch->advanceAction(coffins,itemBag);

        for(auto it:coffins)
        {
            std::cout << it << " \n";
        }

    }


    };

#endif //SYSTEM_HPP