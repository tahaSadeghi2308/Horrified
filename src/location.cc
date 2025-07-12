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

void Place::deleteHero(const std::string &_heroName) {
    for (auto it = heroes.begin(); it != heroes.end(); ++it) {
        if (*it && (*it)->getHeroName() == _heroName) {
            heroes.erase(it);
            return;
        }
    }
}

void Place::addVillager(shared_ptr<Villager> _vill){
    this->villagers.push_back(_vill);
}

void Place::deleteVillager(const std::string &_villagerName) {
    for (auto it = villagers.begin(); it != villagers.end(); ++it) {
        if (*it && (*it)->getName() == _villagerName) {
            villagers.erase(it);
            return;
        }
    }
}

std::vector<shared_ptr<Villager>> Place::getVillagers()
{
    return this -> villagers;
}

std::shared_ptr<HeroBase> Place::getHeros(const std::string &_heroName)
{
    for (auto hero : heroes) { 
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

   std::vector<Item>& Place::getItems()
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

void Place::deleteMonster(const std::string &_monsterName) {
    for (auto it = monsters.begin(); it != monsters.end(); ++it) {
        if (*it && (*it)->getMonsterName() == _monsterName) {
            monsters.erase(it);
            return;
        }
    }
}

vector<shared_ptr<MonsterBase>> Place::getMonsters() { return this->monsters; }

vector<shared_ptr<HeroBase>> Place::getAllHeroes() const {
    return this->heroes;
}

shared_ptr<MonsterBase> Place::getOneMonster(const string &_monsterName)
{
    for (auto monster : monsters)
    {
        if ( monster->getMonsterName() == _monsterName)
        {
            return monster;
        }
    }
    return nullptr;
}

void Place::setPosition(float x,float y)
{
    position = {x,y};
}

void Place::draw(Vector2 mouse) const
{
    bool hover = CheckCollisionPointCircle(mouse, position, radius);
    DrawCircleV(position, radius, hover ? YELLOW : WHITE);
}

bool Place::isClicked(Vector2 mouse) const
{
    return CheckCollisionPointCircle(mouse, position, radius);
}