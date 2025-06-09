#include "villager.hpp"

std::string Villager::getName()
    {
        return name;
    }
    std::shared_ptr<Place> Villager::getVillagerLoc()
    {
        return villagerLoc;
    }
    void Villager::changeLoc(std::shared_ptr<Place> newLoc)
    {
        villagerLoc=newLoc;
    }