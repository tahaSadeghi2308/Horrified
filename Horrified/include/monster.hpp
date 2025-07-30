#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <iostream>
#include "location.hpp"
#include <string_view>

class System;
struct MonsterCard;
class MonsterBase {
    std::string name;
    bool isFrenzed;
    std::shared_ptr<Place> currentPlace;
    System *sys {nullptr};
    int frenzy_order_;
    void setFrenzyOrder(int order) { this->frenzy_order_ = order; }
    Texture2D address;
public:
    MonsterBase(const std::string &_name , bool _isFrenzed , System *s);
    std::string getMonsterName();
    bool getIsFrenzed();
    int getFrenzyOrder() const { return this->frenzy_order_; }
    void changeFrenzedState(bool newState);
    void setCurrentLocation(std::shared_ptr<Place> _newLoc);
    std::shared_ptr<Place> getCurrentLocation();
    void putItem(const int count);
    void doEvent(std::string_view eventName , std::shared_ptr<HeroBase> cHero);
    void move(int n , std::string strike);
    void power(std::shared_ptr<MonsterBase> monst , std::shared_ptr<HeroBase> cHero);
    int attack(char dice , std::shared_ptr<MonsterBase> monst , std::shared_ptr<HeroBase> cHero);
    Texture2D getAddress() { return address;}
    void setAddress(std::string ad) {address = LoadTexture(ad.c_str());}
    virtual ~MonsterBase()
    {
        UnloadTexture(address);
    }
};

class Dracula final : public MonsterBase {
public:
    Dracula(const std::string &_name , bool _isFrenzed , System *s);
};

class InvisibleMan final : public MonsterBase {
public:
    InvisibleMan(const std::string &_name , bool _isFrenzed ,System *s);
};


#endif // MONSTER_HPP