#include "place.hpp"
#include "villager.hpp"
#include <algorithm>
// #include "hero.hpp"

using namespace std;

Place::Place(const string& name) : placeName(name) {}

string Place::getPlaceName() const { return this->placeName; }

void Place::addItem(const Item& i){
    this->items.push_back(i);
}

void Place::deleteItem(const string& _name){
    auto it = std::find_if(this->items.begin(), this->items.end(),
        [&_name](const Item& item) {
            return item.name == _name;
        });
    
    if (it != this->items.end()) {
        this->items.erase(it);
    }
}

 vector<Item>& Place::getAllItems()  {
    return this->items;
}

void Place::addMonster(shared_ptr<MonsterBase> i) {
    this->monsters.push_back(i);
}

void Place::deleteMonster(const string& _name){
    auto it = std::find_if(this->monsters.begin(), this->monsters.end(),
        [&_name](const shared_ptr<MonsterBase>& monster) {
            return monster->getMonsterName() == _name;
        });
    
    if (it != this->monsters.end()) {
        this->monsters.erase(it);
    }
}

 vector<shared_ptr<MonsterBase>>& Place::getAllMonsters()  {
    return this->monsters;
}

void Place::addVillager(shared_ptr<Villager> m){
    this->villagers.push_back(m);
}

void Place::deleteVillager(const string& _name){
    auto it = std::find_if(this->villagers.begin(), this->villagers.end(),
        [&_name](const shared_ptr<Villager>& villager) {
            return villager->getVillagerName() == _name;
        });
    
    if (it != this->villagers.end()) {
        this->villagers.erase(it);
    }
}

 vector<shared_ptr<Villager>>& Place::getAllVillagers()  {
    return this->villagers;
}

void Place::addHero(std::shared_ptr<HeroBase> m){
    this->heros.push_back(m);
}

void Place::deleteHero(const string& _name){
    auto it = std::find_if(this->heros.begin(), this->heros.end(),
        [&_name](const shared_ptr<HeroBase>& hero) {
            return hero->getHeroName() == _name;
        });
    
    if (it != this->heros.end()) {
        this->heros.erase(it);
    }
}

 vector<shared_ptr<HeroBase>>& Place::getAllHeros()  {
    return this->heros;
}