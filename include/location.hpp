#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <string>


class HeroBase;
class Villager;
struct Item; 
class Place {
    std::string name;
    std::vector<std::shared_ptr<Place>> neighbors;
    std::vector<std::shared_ptr<HeroBase>> heroes;
    std::vector<std::shared_ptr<Villager>> villagers;
    std::vector<Item> currentItems;

public:
    explicit Place(const std::string &_name);
    std::string getName();
    void setNeighbors(const std::vector<std::shared_ptr<Place>> neiList);
    std::vector<std::shared_ptr<Place>> getNeighbors();
    void addHero(std::shared_ptr<HeroBase> _hero);
    void deleteHero(const std::string &_heroName);
    void addVillager(std::shared_ptr<Villager> _place);
    void deleteVillager(const std::string &_placeName);
    std::vector<std::shared_ptr<Villager>> getVillagers();
    std::shared_ptr<HeroBase> getHeros(const std::string &_heroName);
    //about items
    void addItem(Item item);
    std::vector<Item> getItems();
    void removeItem(Item );
};

#endif