#include "monster.hpp"

using namespace std;

MonsterBase::MonsterBase(const string &_name , bool _isFrenzed , System *_sys) 
        : name(_name) , isFrenzed(_isFrenzed) , sys(_sys) {}

string MonsterBase::getMonsterName() const { return this->name; }

bool MonsterBase::getIsFrenzed() const { return this->isFrenzed; }

void MonsterBase::changeFrenzedState(bool newState){
    this->isFrenzed = newState;
}

void MonsterBase::setCurrentLocation(string_view _newLoc){
    this->currentPlace = _newLoc;
}

string MonsterBase::getCurrentLocation() const {
    return this->currentPlace;
}

Dracula::Dracula(const string &_name , bool _isFrenzed , System *_sys) 
        : MonsterBase(_name , _isFrenzed , _sys){}

InvisibleMan::InvisibleMan(const string &_name , bool _isFrenzed , System *_sys) 
        : MonsterBase(_name , _isFrenzed , _sys){}

void MonsterBase::putItems(const int count) {
    for (int i {}; i < count; i++){
        Item currentItem {sys->getRandomItem()};
        sys->putItemInPlace(currentItem.place , currentItem);
    }
}

void MonsterBase::doEvent(const string& _eventName){
    if (_eventName == "Sunrise"){
        sys->moveMonster("dracula" , "crypt");
    }
    else if (_eventName == "Thief"){
        sys->placeWithMaxItem();
    }
    else if (_eventName == "The_Delivery"){
        sys->moveVillager("wilbur&chick" , "docks");
    }
    else if (_eventName == "Fortune_Teller"){
        sys->moveVillager("maleva" , "camp");
    }
    else if (_eventName == "Former_Employer"){
        sys->moveVillager("dr.cranley" , "laboratory");
    }
    else if (_eventName == "Hurried_Assistant"){
        sys->moveVillager("fritz" , "tower");
    }
    else if (_eventName == "The_Innocent"){
        sys->moveVillager("maria" , "barn");
    }
    else if (_eventName == "Egyptian_Expert"){
        sys->moveVillager("prof.pearson" , "cave");
    }
    else if (_eventName == "The_Ichthyologist"){
        sys->moveVillager("dr.reed" , "institute");
    }
}

void MonsterBase::runMonsterPhase() {
    // get a random monster card 
    MonsterCard currentCard { sys->getRandomMonstCard() };
    currentCard.name = "Thief";
    // put items 
    this->putItems(currentCard.itemCount);

    // run an event
    this->doEvent(currentCard.name);
}
