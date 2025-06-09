#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <vector>
#include <memory>
// #include "villager.hpp"
#include "card_manager.hpp"
class Place {
    std::string name;
    std::vector<std::shared_ptr<Place>> neighbors;
public:
    Place(const std::string &_name);
    std::string getName();
    void setNeighbors(const std::vector<std::shared_ptr<Place>> &neiList);
    std::vector<std::shared_ptr<Place>> getNeighbors();
};

#endif