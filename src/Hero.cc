#include "Hero.hpp"

    void Hero::Guid()
    {
        std::vector<std::shared_ptr<Villager>> temp = loc->getVillage(); 

        std::vector<Place*> neighbors = loc->getNeighbor();

        std::cout << "would you want to guid a villager 1-to your location 2-to other location\n";

        int choice;
        std::cin >> choice;

        if(choice == 1)
        {
            //not completed
        }
        else if(choice == 2)
        {
            if (temp.empty()) {
                std::cout << "no villagers here to guide\n";
                return;
            }
    
            std::cout << "select a villager to move\n";
            for (int i = 0; i < temp.size(); i++) {
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