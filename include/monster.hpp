#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <iostream>
#include "location.hpp"

class MonsterBase {
    std::string name;
    bool isFrenzed;
    std::shared_ptr<Place> currentPlace;
public:
    MonsterBase(const std::string &_name , bool _isFrenzed);
    std::string getMonsterName();
    bool getIsFrenzed();
    void changeFrenzedState(bool newState);
    // virtual void doPower() = 0;
    void setCurrentLocation(std::shared_ptr<Place> _newLoc);
    std::shared_ptr<Place> getCurrentLocation();
};

class Dracula final : public MonsterBase {
public:
    Dracula(const std::string &_name , bool _isFrenzed);
};

class InvisibleMan final : public MonsterBase {
public:
    InvisibleMan(const std::string &_name , bool _isFrenzed);
};


#endif // MONSTER_HPP