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


    //actions member functions

    virtual void Move() = 0;
    void Guid();
    void pick();
    void advance();
    void defeat();

    //setter getter
    
    std::string getName() const;
    void setName(std::string );
    Place* getPlace() const;
    void setPlace(Place* ) ;
    int getAction() const;
    void decreaseAction();
    void setAction(int );
    
    void addItem(Item);
    void rmitem(Item);


    private:

    std::string name;
    std::vector<int> perk;
    int action;
    Place* loc;
    std::vector<Item> ITEMS;


    
};



class arch:public Hero
{

    //getting a perk card
    arch(Place*);
    void Move();
    void specialAction(); 


};

#endif
