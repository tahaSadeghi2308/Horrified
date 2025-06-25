#ifndef VILLAGER_HPP
#define VILLAGER_HPP

#include <string>

struct Villager {
    // variables
    // NOTE: think about that these variables should be string or place ptr ??
private:
    std::string villagerName , safeZone;
public:
    // functions
    Villager(const std::string& name , const std::string& zone) 
        : villagerName(name) , safeZone(zone) {}
    std::string getVillagerName() const { return this->villagerName; }
    std::string getSafeZone() const { return this->safeZone; }
};

#endif // VILLAGER_HPP