#include "location.hpp"
#include "Hero.hpp"
#include "villager.hpp"
#include "card_manager.hpp"
#include "monster.hpp"

using namespace std;

Place::Place(const string &_name) : name(_name){}

vector<shared_ptr<Place>> Place::getNeighbors() {
    return this->neighbors;
}

void Place::setNeighbors(const vector<shared_ptr<Place>> neiList){
    this->neighbors = neiList;
}

string Place::getName() { return this->name; }

void Place::addHero(shared_ptr<HeroBase> _hero){
    this->heroes.push_back(_hero);
}

void Place::deleteHero(const std::string &_heroName){
    bool endSearching {false};
    for(short i{}; i < this->heroes.size() && !endSearching; i++){
        if (heroes[i]->getHeroName() == _heroName){
            this->heroes.erase(heroes.begin() + i);
            endSearching = true;
        }
    }
}

void Place::addVillager(shared_ptr<Villager> _place){
    this->villagers.push_back(_place);
}

void Place::deleteVillager(const std::string &_placeName){
    bool endSearching {false};
    for(short i{}; i < this->villagers.size() && !endSearching; i++){
        if (villagers[i]->getName() == _placeName){
            this->villagers.erase(villagers.begin() + i);
            endSearching = true;
        }
    }
}

std::vector<shared_ptr<Villager>> Place::getVillagers()
{
    return this -> villagers;
}

std::shared_ptr<HeroBase> Place::getHeros(const std::string &_heroName)
{
    for (auto hero : heroes) { // NOTE: delete &hero -> hero
        if (hero->getHeroName() == _heroName) {
            return hero;
        }
    }
    return nullptr;
}

void Place::addItem(Item item)
   {
       currentItems.push_back(item);
   }

   std::vector<Item> Place::getItems()
   {
       return currentItems;
   }

   void Place::removeItem(Item num)
   {
        for (auto target = currentItems.begin(); target != currentItems.end(); target++)
        {
            if (target->name == num.name)
            {
                currentItems.erase(target);
                break;
            }
        }
   }

void Place::addMonster(shared_ptr<MonsterBase> _monster){
    this->monsters.push_back(_monster);
}

void Place::deleteMonster(const string &_monsterName){
    bool endSearching {false};
    for(short i{}; i < this->monsters.size() && !endSearching; i++){
        if (monsters[i]->getMonsterName() == _monsterName){
            this->monsters.erase(monsters.begin() + i);
            endSearching = true;
        }
    }
}

vector<shared_ptr<MonsterBase>> Place::getMonsters() { return this->monsters; }