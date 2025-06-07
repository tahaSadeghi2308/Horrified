#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <vector>
#include <memory>
#include "item_bag.hpp"
#include "villager.hpp"

class Place
{
    public:

    // essential function to work with places

    Place(std::string ,std::vector<Place*> );

    std::string getName();
    std::vector<Place*> getNeighbor();

    

    // flags to find out if a hero or monster is in the place 

    bool getDracula() const;
    bool getInvisible() const;
    bool getArchaeologist() const;
    bool getMayor() const;

    void setDracula(bool val);
    void setInvisible(bool val);
    void setArchaeologist(bool val);
    void setMayor(bool val);


    //when we move a villager we remove it from the previouse place and add it in the next one 

    void addVill(std::shared_ptr<Villager> );

    void rmVill(std::shared_ptr<Villager> );

    std::vector<std::shared_ptr<Villager>> getVillage() const;


    // same as villagers 

    void addItem(Item item);

    std::vector<Item> getItems();

    void removeItem(int );

    private:
    // all the things that could be in a place 
    std::string name;
    std::vector<Place*> neighbor;
    std::vector<std::shared_ptr<Villager>> village;
    std::vector<Item> items;
    bool dreacula;
    bool invisable;
    bool arch;
    bool mayor;
};




#endif