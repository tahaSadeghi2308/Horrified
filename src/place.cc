#include "place.hpp"

using namespace std;

Place::Place(const string& name) : placeName(name) {}

string Place::getPlaceName() const { return this->placeName; }

void Place::addItem(const Item& i){
    this->items.push_back(i);
}

void Place::deleteItem(const string& _name){
    bool isFound {};
    for (int i {}; i < this->items.size() && !isFound; i++) {
        if (this->items[i].name == _name){
            this->items.erase(items.begin() + i);
        }
    }
}

const vector<Item>& Place::getAllItems() const {
    return this->items;
}