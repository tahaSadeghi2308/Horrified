#include "location.hpp"

// essential function to work with places

   Place::Place(std::string n,std::vector<Place*> neigh ):name(n),neighbor(neigh)
   {}

   std::string Place::getName()
   {
       return name;
   }
   std::vector<Place*> Place::getNeighbor()
   {
       return neighbor;
   }

   

   // flags to find out if a hero or monster is in the place 

   bool Place::getDracula() const 
   {
       return dreacula; 
   }
   bool Place::getInvisible() const
   {
       return invisable;
   }
   bool Place::getArchaeologist() const
   {
       return arch;
   }
   bool Place::getMayor() const
   {
       return mayor;
   }


   void Place::setDracula(bool val)
   {
       dreacula = val; 
   }
   void Place::setInvisible(bool val) 
   {
       invisable = val; 
   }
   void Place::setArchaeologist(bool val) 
   {
       arch = val; 
   }
   void Place::setMayor(bool val)
   {
       mayor = val; 
   }


   //when we move a villager we remove it from the previouse place and add it in the next one 

   void Place::addVill(std::shared_ptr<Villager> vill)
   {
       village.push_back(vill);
   }

   void Place::rmVill(std::shared_ptr<Villager> vill)
   {
      // village.erase(vill);
   }

   std::vector<std::shared_ptr<Villager>> Place::getVillage() const
   {
       return village;
   }


   // same as villagers 

   void Place::addItem(Item item)
   {
       items.push_back(item);
   }

   std::vector<Item> Place::getItems()
   {
       return items;
   }

   void Place::removeItem(int num)
   {
    if(num < items.size())
    {
            items.erase(items.begin() + num);
    }
   }