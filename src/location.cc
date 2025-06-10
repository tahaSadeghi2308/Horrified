#include "location.hpp"
#include "Hero.hpp"
#include "villager.hpp"

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