#include "monster.hpp"
#include "system.hpp"

using namespace std;

MonsterBase::MonsterBase(const string &_name , bool _isFrenzed , System *s) 
    : name(_name) , isFrenzed(_isFrenzed) , sys(s) {}

string MonsterBase::getMonsterName() { return this->name; }

bool MonsterBase::getIsFrenzed() { return this->isFrenzed; }

void MonsterBase::changeFrenzedState(bool newState){
    this->isFrenzed = newState;
}

void MonsterBase::setCurrentLocation(shared_ptr<Place> _newLoc){
    this->currentPlace = _newLoc;
}
shared_ptr<Place> MonsterBase::getCurrentLocation(){
    return this->currentPlace;
}

Dracula::Dracula(const string &_name , bool _isFrenzed , System *s) : MonsterBase(_name , _isFrenzed , s){}

InvisibleMan::InvisibleMan(const string &_name , bool _isFrenzed , System *s) : MonsterBase(_name , _isFrenzed , s){}

void MonsterBase::putItem(const int count){
    for (int i {}; i < count; i++){
        Item currentItem {sys->getRandomItem()};
        sys->putItemInPlace(currentItem.place , currentItem);
    }
}

void MonsterBase::power(shared_ptr<MonsterBase> monst , shared_ptr<HeroBase> cHero){
    if (monst->getMonsterName() == "dracula"){
        sys->moveHero(cHero , monst->getCurrentLocation());
    }
    else if (monst->getMonsterName() == "invisibleMan"){
        int count = 2;
        bool foundEnemy = false;
        while (count-- && !foundEnemy) {
            for (auto place : sys->getAllLocations()){
                if (place->getName() == this->currentPlace->getName()){
                    if (!place->getVillagers().empty() || !place->getAllHeroes().empty()){
                        foundEnemy = true;
                    } 
                    else {
                        vector<shared_ptr<Place>> path = sys->findShortestPath(this->getCurrentLocation() , ETO);
                        for (auto monst : sys->getAllMonsters()){
                            if (monst->getMonsterName() == "invisibleMan"){
                                if (!path.empty()) sys->moveMonster(monst , path[1]);
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
}

void MonsterBase::doEvent(string_view eventName){
    if (eventName == "Sunrise"){
        for(auto monst : sys->getAllMonsters()){
            if (monst != nullptr && monst->getMonsterName() == "dracula"){
                for(auto loc : sys->getAllLocations()){
                    if (loc->getName() == "crypt") { sys->moveMonster(monst , loc); break; }
                }
                break;
            }
        }
    }
    else if (eventName == "Thief"){
        sys->placeWithMaxItem();
    }
    else if (eventName == "The_Delivery"){
        for(auto vill : sys->getAllVillagers()){
            if (vill->getName() == "wilbur&chick"){
                for(auto loc : sys->getAllLocations()){
                    if (loc->getName() == "docks") { sys->moveVillager(vill , loc); break; }
                }
                break;
            }
        }
    }
    else if (eventName == "Fortune_Teller"){
        for(auto vill : sys->getAllVillagers()){
            if (vill->getName() == "maleva"){
                for(auto loc : sys->getAllLocations()){
                    if (loc->getName() == "camp") { sys->moveVillager(vill , loc); break; }
                }
                break;
            }
        }
    }
    else if (eventName == "Former_Employer"){
        for(auto vill : sys->getAllVillagers()){
            if (vill->getName() == "dr.cranley"){
                for(auto loc : sys->getAllLocations()){
                    if (loc->getName() == "laboratory") { sys->moveVillager(vill , loc); break; }
                }
                break;
            }
        }
    }
    else if (eventName == "Hurried_Assistant"){
        for(auto vill : sys->getAllVillagers()){
            if (vill->getName() == "fritz"){
                for(auto loc : sys->getAllLocations()){
                    if (loc->getName() == "tower") { sys->moveVillager(vill , loc); break; }
                }
                break;
            }
        }
    }
    else if (eventName == "The_Innocent"){
        for(auto vill : sys->getAllVillagers()){
            if (vill->getName() == "maria"){
                for(auto loc : sys->getAllLocations()){
                    if (loc->getName() == "barn") { sys->moveVillager(vill , loc); break; }
                }
                break;
            }
        }
    }
    else if (eventName == "Egyptian_Expert"){
        for(auto vill : sys->getAllVillagers()){
            if (vill->getName() == "prof.pearson"){
                for(auto loc : sys->getAllLocations()){
                    if (loc->getName() == "cave") { sys->moveVillager(vill , loc); break; }
                }
                break;
            }
        }
    }
    else if (eventName == "The_Ichthyologist"){
        for(auto vill : sys->getAllVillagers()){
            if (vill->getName() == "dr.reed"){
                for(auto loc : sys->getAllLocations()){
                    if (loc->getName() == "institute") { sys->moveVillager(vill , loc); break; }
                }
                break;
            }
        }
    }
}

void MonsterBase::move(int n , string strike){
    if (strike == "du") {
        int count = n;
        bool foundEnemy = false;
        while (count-- && !foundEnemy) {
            for (auto place : sys->getAllLocations()){
                if (place->getName() == this->currentPlace->getName()){
                    if (!place->getVillagers().empty() || !place->getAllHeroes().empty()){
                        foundEnemy = true;
                    } 
                    else 
                    {
                        vector<shared_ptr<Place>> path = sys->findShortestPath(this->getCurrentLocation() , ETO);
                        for (auto monst : sys->getAllMonsters()){
                            if (monst->getMonsterName() == "dracula")
                                sys->moveMonster(monst , path[1]);
                        }
                    }
                    break;
                }
            }
        }
    }
    else if (strike == "inm"){
        int count = n;
        bool foundEnemy = false;
        while (count-- && !foundEnemy) {
            for (auto place : sys->getAllLocations()){
                if (place->getName() == this->currentPlace->getName()){
                    if (!place->getVillagers().empty() || !place->getAllHeroes().empty()){
                        foundEnemy = true;
                    } 
                    else 
                    {
                        vector<shared_ptr<Place>> path = sys->findShortestPath(this->getCurrentLocation() , ETO);
                        for (auto monst : sys->getAllMonsters()){
                            if (monst->getMonsterName() == "invisibleMan")
                                sys->moveMonster(monst , path[1]);
                        }
                    }
                    break;
                }
            }
        }
    }
    else {
        for (auto monst : sys->getAllMonsters()){
            if (monst->getIsFrenzed()){
                if (monst->getMonsterName() == "dracula"){
                    int count = n;
                    bool foundEnemy = false;
                    while (count-- && !foundEnemy) {
                        for (auto place : sys->getAllLocations()){
                            if (place->getName() == this->currentPlace->getName()){
                                if (!place->getVillagers().empty() || !place->getAllHeroes().empty()){
                                    foundEnemy = true;
                                } 
                                else 
                                {
                                    vector<shared_ptr<Place>> path = sys->findShortestPath(this->getCurrentLocation() , ETO);
                                    for (auto monst : sys->getAllMonsters()){
                                        if (monst->getMonsterName() == "dracula")
                                            sys->moveMonster(monst , path[1]);
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
                else if (monst->getMonsterName() == "invisibleMan"){
                    int count = n;
                    bool foundEnemy = false;
                    while (count-- && !foundEnemy) {
                        for (auto place : sys->getAllLocations()){
                            if (place->getName() == this->currentPlace->getName()){
                                if (!place->getVillagers().empty() || !place->getAllHeroes().empty()){
                                    foundEnemy = true;
                                } 
                                else 
                                {
                                    vector<shared_ptr<Place>> path = sys->findShortestPath(this->getCurrentLocation() , ETO);
                                    for (auto monst : sys->getAllMonsters()){
                                        if (monst->getMonsterName() == "invisibleMan")
                                            sys->moveMonster(monst , path[1]);
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

int MonsterBase::attack(
    char dice , 
    shared_ptr<MonsterBase> monst , 
    shared_ptr<HeroBase> cHero
){
    if (dice == '*'){
        shared_ptr<Place> p = this->getCurrentLocation();
        if (!p->getVillagers().empty()){
            sys->killVillager(p->getVillagers()[0]);
            return 1; // for killing a viilager
        }
        else if (!p->getAllHeroes().empty()){
            for (auto her : p->getAllHeroes()){
                if (her->getHeroName() == cHero->getHeroName()) return 2; // for attack to arch
            }
            return 3; // for attact to mayor
        }
    }
    else if (dice == '!') {
        this->power(monst , cHero);
        return 4; // for running monster power
    }
    return -1; // for no attack or power
}
