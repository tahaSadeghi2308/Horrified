#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <iostream>

class MonsterBase {
    std::string name;
    bool isFrenzed;
    std::string currentPlace;
public:
    MonsterBase(const std::string &_name , bool _isFrenzed);
    std::string getMonsterName() const;
    bool getIsFrenzed() const;
    void changeFrenzedState(bool newState);
    // virtual void doPower() = 0;
    void setCurrentLocation(const std::string &_newLoc);
    std::string getCurrentLocation() const;
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