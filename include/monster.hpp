#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <iostream>
#include "system.hpp"
#include <fmt/core.h>

class MonsterBase {
    // variables
    std::string name;
    bool isFrenzed;
    std::string currentPlace;
    System *sys {nullptr};

    // functions
    void putItems(const int count);
    void doEvent(const std::string& _eventName);
    // void move();
    // void attack();

public:
    MonsterBase(const std::string &_name , bool _isFrenzed , System *_sys);
    std::string getMonsterName() const;
    bool getIsFrenzed() const;
    void changeFrenzedState(bool newState);
    void setCurrentLocation(const std::string &_newLoc);
    std::string getCurrentLocation() const;
    void runMonsterPhase();
};

class Dracula final : public MonsterBase {
public:
    Dracula(const std::string &_name , bool _isFrenzed, System *_sys);
};

class InvisibleMan final : public MonsterBase {
public:
    InvisibleMan(const std::string &_name , bool _isFrenzed, System *_sys);
};

#endif // MONSTER_HPP