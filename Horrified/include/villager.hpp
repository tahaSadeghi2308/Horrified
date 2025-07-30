#ifndef VILLAGER_HPP
#define VILLAGER_HPP

#include <iostream>
#include "location.hpp"

class Villager final {
    std::string name;
    std::shared_ptr<Place> villagerLoc;
    std::shared_ptr<Place> safeZone;
    Texture2D address;
public:
    Villager(const std::string &_vill);
    std::string getName() const;
    std::shared_ptr<Place> getVillagerLoc() const;
    void changeLoc(std::shared_ptr<Place> _newLoc);
    void setSafeZone(std::shared_ptr<Place> _newLoc);
    std::shared_ptr<Place> getSafeZone(); 
    Texture2D getAddress() { return address;}
    void setAddress(std::string ad) {address = LoadTexture(ad.c_str());}
    ~Villager()
    {
        UnloadTexture(address);
    }
};

#endif