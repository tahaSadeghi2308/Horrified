#ifndef PLACE_HPP
#define PLACE_HPP

#include <vector>
#include <string>
#include <memory>
#include "card_manager.hpp"
#include "monster.hpp" 

struct Villager;

class Place final {
    std::string placeName;
    std::vector<Item> items;
    std::vector<std::shared_ptr<MonsterBase>> monsters;
    std::vector<std::shared_ptr<Villager>> villagers;
public:
    explicit Place(const std::string& name);
    std::string getPlaceName() const;
    void addItem(const Item& i);
    void deleteItem(const std::string& _name);
    const std::vector<Item>& getAllItems() const;
    void addMonster(std::shared_ptr<MonsterBase> m);
    void deleteMonster(const std::string& _name);
    const std::vector<std::shared_ptr<MonsterBase>>& getAllMonsters() const;
    void addVillager(std::shared_ptr<Villager> m);
    void deleteVillager(const std::string& _name);
    const std::vector<std::shared_ptr<Villager>>& getAllVillagers() const;
};

#endif // PLACE_HPP