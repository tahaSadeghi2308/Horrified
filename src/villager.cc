#include "villager.hpp"

using namespace std;

string Villager::getName() const { return name; }

Villager::Villager(const string &_name) : name(_name){}

shared_ptr<Place> Villager::getVillagerLoc() const {
    return this->villagerLoc;   
}

void Villager::changeLoc(shared_ptr<Place> _newLoc){
    this->villagerLoc = _newLoc;
}

void Villager::setSafeZone(shared_ptr<Place> _newLoc){
    this->safeZone = _newLoc;
}