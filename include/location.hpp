#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <vector>
#include <memory>
#include "Hero.hpp"
#include "card_manager.hpp"
#include "villager.hpp"
class Place {
    std::string name;
    std::vector<std::shared_ptr<Place>> neighbors;
    std::vector<std::shared_ptr<HeroBase>> heroes;

    std::vector<std::shared_ptr<Villager>> village;//use a village private member to store the current villagers

public:
    Place(const std::string &_name);
    std::string getName();
    void setNeighbors(const std::vector<std::shared_ptr<Place>> &neiList);
    std::vector<std::shared_ptr<Place>> getNeighbors();
    void addHero(std::shared_ptr<HeroBase> _hero);
    void deleteHero(const std::string &_heroName);

    //----functions about working with villagers----
    void addVill(std::shared_ptr<Villager> );
    void rmVill(std::shared_ptr<Villager> );
    std::vector<std::shared_ptr<Villager>> getVillage() const;

};

#endif