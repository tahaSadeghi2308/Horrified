#ifndef HERO_HPP
#define HERO_HPP

#include <iostream>
#include <vector>
#include "location.hpp"
#include <memory>
#include <stdexcept>
#include "villager.hpp"



class Hero
{    
public:
    Hero();



    virtual void Move() = 0;


    void Guid();


    void pick();
    void advance();
    void defeat();


    
    Place* getPlace() const;

    void setPlace(Place* ) ;

    int getAction() const;

    void decreaseAction();



    private:

    std::string name;
    std::vector<int> perk;
    int action;
    Place* loc;
    int ITEMS;


    ~Hero();
};



class arch:public Hero
{

    void Move();
    

    




};

#endif
