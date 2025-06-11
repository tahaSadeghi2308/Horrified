#include "monster.hpp"

using namespace std;

MonsterBase::MonsterBase(const string &_name , bool _isFrenzed) : name(_name) , isFrenzed(_isFrenzed) {}

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

Dracula::Dracula(const string &_name , bool _isFrenzed) : MonsterBase(_name , _isFrenzed){}

InvisibleMan::InvisibleMan(const string &_name , bool _isFrenzed) : MonsterBase(_name , _isFrenzed){}