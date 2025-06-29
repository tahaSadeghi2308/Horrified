#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <iostream>
#include "system.hpp"
#include <fmt/core.h>
#include <string_view>

class MonsterBase {
    // variables
    std::string name;
    bool isFrenzed;
    std::string currentPlace;
    System *sys {nullptr};

    // functions
    void putItems(const int count);
    void doEvent(const std::string& _eventName);
    void move(int n , std::string strike);
    int attack(char , std::string_view monsterName , std::string_view cHero);
    void power(std::string_view monsterName , std::string_view cHero);
public:
    MonsterBase(const std::string &_name , bool _isFrenzed , System *_sys);
    std::string getMonsterName() const;
    bool getIsFrenzed() const;
    void changeFrenzedState(bool newState);
    void setCurrentLocation(std::string_view _newLoc);
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