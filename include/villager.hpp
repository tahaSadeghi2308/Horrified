#ifndef VILLAGER_HPP
#define VILLEGER_HPP

#include <iostream>
#include "location.hpp"

class Villager
{
    public:
    std::string getName();
    Place* getVillagerLoc();
    void changeLoc(Place* );
    
    private:
    std::string name;
    Place* villagerLoc;
};




#endif