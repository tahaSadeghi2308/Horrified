#include "location.hpp"

using namespace std;

Place::Place(const string &_name) : name(_name){}

vector<shared_ptr<Place>> Place::getNeighbors() {
    return this->neighbors;
}

void Place::setNeighbors(const vector<shared_ptr<Place>> &neiList){
    this->neighbors = neiList;
}

string Place::getName(){ return this->name; }

void Place::addHero(shared_ptr<HeroBase> _hero){
    this->heroes.push_back(_hero);
}

void Place::deleteHero(const std::string &_heroName){
    bool endSearching {false};
    for(short i{}; i < this->heroes.size() && endSearching; i++){
        if (heroes[i]->getHeroName() == _heroName){
            this->heroes.erase(heroes.begin() + i);
            endSearching = true;
        }
    }
}

//----function about working with villagers---


void Place::addVill(std::shared_ptr<Villager> vill)
{
    village.push_back(vill);
}

void Place::rmVill(std::shared_ptr<Villager> vill)
{
     for(auto target = village.begin(); target != village.end(); target++)
     {
         if (*target == vill)
         {
             village.erase(target);
             break;
         }
     }
}

std::vector<std::shared_ptr<Villager>> Place::getVillage() const
{
    return village;
}

//-----working with items------

void Place::addItem(Item item)
   {
       ITEMS.push_back(item);
   }

   std::vector<Item> Place::getItems()
   {
       return ITEMS;
   }

   void Place::removeItem(Item num)
   {
        for (auto target = ITEMS.begin(); target != ITEMS.end(); target++)
        {
            if (target->name == num.name)
            {
                ITEMS.erase(target);
                break;
            }
        }
   }