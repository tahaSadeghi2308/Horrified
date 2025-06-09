#ifndef VILLAGER_HPP
#define VILLEGER_HPP

#include <iostream>
#include "location.hpp"

class Villager
{
    public:
    std::string getName();
    std::shared_ptr<Place> getVillagerLoc();
    void changeLoc(std::shared_ptr<Place> );
    
    private:
    std::string name;
    std::shared_ptr<Place> villagerLoc;
};




#endif