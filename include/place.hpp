#ifndef PLACE_HPP
#define PLACE_HPP

#include <vector>
#include <string>
#include "card_manager.hpp"

class Place final {
    std::string placeName;
    std::vector<Item> items;
public:
    explicit Place(const std::string& name);
    std::string getPlaceName() const;
    void addItem(const Item& i);
    void deleteItem(const std::string& _name);
    const std::vector<Item>& getAllItems() const;
};

#endif // PLACE_HPP