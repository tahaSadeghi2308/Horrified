#include "Hero.hpp"
#include <map>
    void Hero::Guid()
    {
        std::vector<std::shared_ptr<Villager>> temp = loc->getVillage(); 

        std::vector<Place*> neighbors = loc->getNeighbor();

        std::cout << "would you want to guid a villager 1-to your location 2-to other location\n";

        int choice;
        std::cin >> choice;

        if(choice == 1)
        {
            //the problem is that when a player choose a wrong number fo place or villager it will throw a exeption

            std::vector<Place*> places;

            for(Place* item:neighbors)
            {
                std::vector<std::shared_ptr<Villager>> possibleVillager = item->getVillage();
                if(!possibleVillager.empty())
                {
                    places.push_back(item);
                }
            }

            if(places.empty())
            {
                throw std::invalid_argument("there is no villager for you to guid it to your location\n");
            }

            std::cout << "from which place you want to guid\n";

            for(int i=0;i<places.size();i++)
            {
                std::cout << i+1 << " - " << places[i]->getName() << std::endl;
            }

            int placeChoice;
            std::cin >> placeChoice;

            if(placeChoice < 1 || placeChoice > places.size())
            {
                throw std::out_of_range("invalid place choice\n");
            }

            Place* selectedPlace = places[placeChoice - 1];
            std::vector<std::shared_ptr<Villager>> villagers = selectedPlace->getVillage();
        
            std::cout << "select a villager to guide \n";
            for(int i = 0; i < villagers.size(); i++)
            {
                std::cout << i + 1 << " - " << villagers[i]->getName() << std::endl;
            }
        
            int villagerChoice;
            std::cin >> villagerChoice;
        
            if(villagerChoice < 1 || villagerChoice > villagers.size())
            {
                throw std::out_of_range("invalid villager selection\n");
            }
        
            std::shared_ptr<Villager> selectedVillager = villagers[villagerChoice - 1];
        
            selectedPlace->rmVill(selectedVillager);
            loc->addVill(selectedVillager);
            selectedVillager->changeLoc(loc);
        
            //

            decreaseAction(); 
        

        }
        else if(choice == 2)
        {
            if(temp.empty())
            {
                std::cout << "no villagers here to guide\n";
                return;
            }
    
            std::cout << "select a villager to move\n";

            for(int i = 0; i < temp.size(); i++)
            {
                std::cout << i + 1 << "-" << temp[i]->getName() << std::endl;
            }
            
        

        int choice2;
        std::cin >> choice2;

            if(choice2 < 1 || choice2 > temp.size() )
            {
                throw std::out_of_range("the number that you selected for guiding a villager is invalid\n");
            }

        std::shared_ptr<Villager> chosenVillager = temp[choice2-1];

        std::cout << "choose the path that you want to quid the villager\n";

        for (size_t i = 0; i < neighbors.size(); i++)
        {
            std::cout << i+1 << "-" << neighbors[i]->getName() << std::endl;
        }
        
        int choose;
        std::cin >> choose;
        if(choose < 1 || choose > neighbors.size())
        {
            throw std::out_of_range("the neighbor that you selected to guid a villager is invalid\n");
        }
        
        Place* chosenPlace = neighbors[choose-1];

        loc->rmVill(chosenVillager);

        chosenPlace->addVill(chosenVillager);
        
        chosenVillager->changeLoc(chosenPlace);

        std::cout << "villager->" << chosenVillager->getName() << " guided to -> " << chosenPlace->getName() << std::endl;

        }

        else
        {
            throw std::invalid_argument("your choice for guiding is invalid\n ");
        }

    }


    
    Place* Hero::getPlace() const
    {
        return loc;
    }

    void Hero::setPlace(Place* p) 
    {
        loc = p;
    }

    int Hero::getAction() const
    {
        return action;
    }

    void Hero::decreaseAction()
    {
        if(action > 0)
        action--;
    }


//---------arch---------

    void arch::Move()
    {
        {
            std::cout << "where do you want to go (select a number)\n";
    
            std::vector<Place*> targetPlaces = getPlace()->getNeighbor();
    
            for(int i = 0;i < targetPlaces.size();i++)
            {    
                std::cout << i+1 << '-' << targetPlaces[i]->getName() << std::endl;
            }
            int choice;
            std::cin >> choice;
            if(choice < 1 || choice >targetPlaces.size() )
            {
                throw std::invalid_argument("the number that you have selected for moving is invalid\n");
            }
            getPlace()->setArchaeologist(false);
            setPlace(targetPlaces[choice-1]);
            getPlace()->setArchaeologist(true);
            decreaseAction();        
        }
    }