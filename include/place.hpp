#ifndef PLACE_HPP
#define PLACE_HPP

#include <vector>
#include <string>
#include "card_manager.hpp"
#include "monster.hpp" 

// class MonsterBase;

class Place final {
    std::string placeName;
    std::vector<Item> items;
    std::vector<MonsterBase> monsters;
public:
    explicit Place(const std::string& name);
    std::string getPlaceName() const;
    void addItem(const Item& i);
    void deleteItem(const std::string& _name);
    const std::vector<Item>& getAllItems() const;
};

#endif // PLACE_HPP