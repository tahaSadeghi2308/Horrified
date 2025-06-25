#include "monster.hpp"

using namespace std;

MonsterBase::MonsterBase(const string &_name , bool _isFrenzed) : name(_name) , isFrenzed(_isFrenzed) {}

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

Dracula::Dracula(const string &_name , bool _isFrenzed) : MonsterBase(_name , _isFrenzed){}

InvisibleMan::InvisibleMan(const string &_name , bool _isFrenzed) : MonsterBase(_name , _isFrenzed){}