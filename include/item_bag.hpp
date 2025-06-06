#ifndef ITEM_BAG_HPP
#define ITEM_BAG_HPP

#include <vector>
#include <string>

enum Color { RED , BLUE , YELLOW };

constexpr int ITEM_COUNT {2};

struct Item {
    int power;
    Color color;
    std::string name;
    std::string place; // NOTE: maybe change base on place implementation
};

class ItemBag {
    std::vector<Item> items;
    void shuffleItems();
public:
    ItemBag();
    Item pickOneRandom();
};

#endif // ITEM_BAG_HPP