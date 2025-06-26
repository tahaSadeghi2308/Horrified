#include "place.hpp"
#include "villager.hpp"

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
            isFound = true;
        }
    }
}

const vector<Item>& Place::getAllItems() const {
    return this->items;
}

void Place::addMonster(shared_ptr<MonsterBase> i) {
    this->monsters.push_back(i);
}

void Place::deleteMonster(const string& _name){
    bool isFound {};
    for (int i {}; i < this->monsters.size() && !isFound; i++) {
        if (this->monsters[i]->getMonsterName() == _name){
            this->monsters.erase(monsters.begin() + i);
            isFound = true;
        }
    }
}

const vector<shared_ptr<MonsterBase>>& Place::getAllMonsters() const {
    return this->monsters;
}

void Place::addVillager(shared_ptr<Villager> m){
    this->villagers.push_back(m);
}

void Place::deleteVillager(const string& _name){
    bool isFound {};
    for (int i {}; i < this->villagers.size() && !isFound; i++) {
        if (this->villagers[i]->getVillagerName() == _name){
            this->villagers.erase(villagers.begin() + i);
            isFound = true;
        }
    }
}

const vector<shared_ptr<Villager>>& Place::getAllVillagers() const {
    return this->villagers;
}