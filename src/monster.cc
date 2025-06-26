#include "monster.hpp"

using namespace std;

MonsterBase::MonsterBase(const string &_name , bool _isFrenzed , System *_sys) 
        : name(_name) , isFrenzed(_isFrenzed) , sys(_sys) {}

string MonsterBase::getMonsterName() const { return this->name; }

bool MonsterBase::getIsFrenzed() const { return this->isFrenzed; }

void MonsterBase::changeFrenzedState(bool newState){
    this->isFrenzed = newState;
}

void MonsterBase::setCurrentLocation(const string &_newLoc){
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
        
    }
}

void MonsterBase::runMonsterPhase() {
    // get a random monster card 
    MonsterCard currentCard { sys->getRandomMonstCard() };

    // put items 
    this->putItems(currentCard.itemCount);

    // run an event
    this->doEvent(currentCard.name);
}
