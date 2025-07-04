#ifndef PLACE_HPP
#define PLACE_HPP

#include <vector>
#include <string>
#include <memory>
#include "card_manager.hpp"
#include "monster.hpp" 
#include "hero.hpp"

struct Villager;

class Place final {
    std::string placeName;
    std::vector<Item> items;
    std::vector<std::shared_ptr<MonsterBase>> monsters;
    std::vector<std::shared_ptr<Villager>> villagers;
    std::vector<std::shared_ptr<HeroBase>> heros;
public:
    explicit Place(const std::string& name);
    std::string getPlaceName() const;
    void addItem(const Item& i);
    void deleteItem(const std::string& _name);
    std::vector<Item>& getAllItems() ;
    void addMonster(std::shared_ptr<MonsterBase> m);
    void deleteMonster(const std::string& _name);
    std::vector<std::shared_ptr<MonsterBase>>& getAllMonsters() ;
    void addVillager(std::shared_ptr<Villager> m);
    void deleteVillager(const std::string& _name);
    std::vector<std::shared_ptr<Villager>>& getAllVillagers() ;
    void addHero(std::shared_ptr<HeroBase> m);
    void deleteHero(const std::string& _name);
    std::vector<std::shared_ptr<HeroBase>>& getAllHeros() ;
};

#endif // PLACE_HPP