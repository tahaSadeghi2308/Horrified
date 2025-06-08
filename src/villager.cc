#include "villager.hpp"


    Villager::Villager(std::string n,Place* pl):name(n),villagerLoc(pl)
    {}

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