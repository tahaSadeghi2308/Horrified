#include "villager.hpp"

std::string Villager::getName()
    {
        return name;
    }
    Place* Villager::getVillagerLoc()
    {
        return villagerLoc;
    }
    void Villager::changeLoc(Place* newLoc)
    {
        villagerLoc=newLoc;
    }