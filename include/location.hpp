#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <string>

class HeroBase;

class Place {
    std::string name;
    std::vector<std::shared_ptr<Place>> neighbors;
    std::vector<std::shared_ptr<HeroBase>> heroes;

public:
    Place(const std::string &_name);
    std::string getName();
    void setNeighbors(const std::vector<std::shared_ptr<Place>> neiList);
    std::vector<std::shared_ptr<Place>> getNeighbors();
    void addHero(std::shared_ptr<HeroBase> _hero);
    void deleteHero(const std::string &_heroName);
};

#endif