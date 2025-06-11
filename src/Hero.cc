#include "Hero.hpp"

using namespace std;

void HeroBase::addPerkCard(const Perk &perk) {
    this->heroPerks.push_back(perk);
}

HeroBase::HeroBase (
    const int &_num, 
    std::string_view _name,
    const Perk &perk
) : actionCount(_num) , heroName(_name) {
    this->addPerkCard(perk);
}

int HeroBase::getActionCount() { return this->actionCount; }

void HeroBase::setActionCount(const int &num) {
    // NOTE : We dont need to validate it because its valid :) 
    this->actionCount = num; 
}

std::string HeroBase::getHeroName() { return this->heroName; }

Archaeologist::Archaeologist(const int &_num , std::string_view _name , const Perk &perk) :
    HeroBase(_num , _name , perk) {}

Mayor::Mayor(const int &_num , std::string_view _name , const Perk &perk) :
    HeroBase(_num , _name , perk) {}

void HeroBase::setCurrentPlace(shared_ptr<Place> _place) {
    this->currentPlace = _place;
}

shared_ptr<Place> HeroBase::getCurrentPlace() { return this->currentPlace; }

std::vector<Item> HeroBase::getHeroItems() { return this->heroItems; }

void HeroBase::addHeroItems(Item _item)
{
    heroItems.push_back(_item);
}
//----move-----

void HeroBase::moveAction()
{
   std::cout << "where do you want to go (select a number)\n";
    
            std::vector<std::shared_ptr<Place>> targetPlaces = currentPlace->getNeighbors();
    
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



            for (auto vill : currentPlace -> getVillagers())
            {
                targetPlaces[choice-1]->addVillager(vill); 
                currentPlace -> deleteVillager(vill -> getName());                          
                vill -> changeLoc(targetPlaces[choice-1]);        
            }

            targetPlaces[choice-1] -> addHero(currentPlace->getHeros(getHeroName()));
            currentPlace->deleteHero(getHeroName());
            setCurrentPlace(targetPlaces[choice-1]);
            
            actionCount--;     
}

//----pickup-----


void HeroBase::pickUpAction()
{
    std::vector<Item> ITEMS = currentPlace->getItems();

        if(ITEMS.empty())
        {
            std::cout << "there is no item in your place\n";
            return;
        }

        for (int i = 0; i < ITEMS.size(); i++)
        {
            std::cout << i+1 << " - " << ITEMS[i].name << " " << ITEMS[i].color << ' ' << ITEMS[i].power << '\n';
        }
        int choice;
        std::cin >> choice;

        if(choice < 1 || choice > ITEMS.size())
        {
            throw std::out_of_range("invalid item \n");
        }

        heroItems.push_back(ITEMS[choice-1]);
        currentPlace->removeItem(ITEMS[choice-1]);
        actionCount--;
}

//---speciall action-----

void Archaeologist::specialAction()
{
    std::vector<std::shared_ptr<Place>> neigh = getCurrentPlace()->getNeighbors();
        //storing all the possible items with its places with pair. first i tried map but it didnt work (it has seviral problems for us to find out which item user chosed)
        std::vector<std::pair<std::shared_ptr<Place>,Item>> possible;

        int i = 1;
        for (std::shared_ptr<Place> neighbor : neigh) {
            for (auto& item : neighbor->getItems())
            {
                std::cout << i << " - " <<  "place -> " << neighbor->getName() << "item -> " << item.name << ' ' << item.color << ' ' << item.power << '\n';
                possible.push_back(std::make_pair(neighbor,item));
                i++;
            }
        }

        if(possible.empty())
        {
            std::cout << "there is no item nearby\n";
            return;
        }

        std::cout << "select the item to pick up \n";
    int choice;
    std::cin >> choice;

    if(choice < 1 || choice > possible.size())
    {
        throw std::out_of_range("Invalid selection.\n");
    }

    std::shared_ptr<Place> targetPlace = possible[choice - 1].first;
    Item targetItem = possible[choice - 1].second;

    targetPlace->removeItem(targetItem);   

    
    addHeroItems(targetItem);
    
    setActionCount(getActionCount()-1);
}

//-----guid----

void HeroBase::guideAction()
{
    std::vector<std::shared_ptr<Villager>> currentPlaceVillagers = currentPlace->getVillagers(); 
        std::vector<std::shared_ptr<Place>> neighbors = currentPlace->getNeighbors();

        std::cout << "would you want to guid a villager 1-to your location 2-to other location\n";

        int choice;
        std::cin >> choice;

        if(choice == 1)
        {
            //the problem is that when a player choose a wrong number fo place or villager it will throw a exeption

            std::vector<std::shared_ptr<Place>> places;

            for(std::shared_ptr<Place> item:neighbors)
            {
                std::vector<std::shared_ptr<Villager>> possibleVillager = item->getVillagers();
                if(!possibleVillager.empty())
                {
                    places.push_back(item);
                }
            }

            if(places.empty())
            {
                std::cout<<"there is no villager for you to guid it to your location\n";
                return;
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

            std::shared_ptr<Place> selectedPlace = places[placeChoice - 1];
            std::vector<std::shared_ptr<Villager>> villagers = selectedPlace->getVillagers();
        
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
        
            selectedPlace->deleteVillager(selectedVillager->getName());
            currentPlace->addVillager(selectedVillager);
            selectedVillager->changeLoc(currentPlace);
        
            std::cout << "villager ->" << selectedVillager->getName() << "came to your location\n";

            actionCount--;
        

        }
        else if(choice == 2)
        {
            if(currentPlaceVillagers.empty())
            {
                std::cout << "no villagers here to guide\n";
                return;
            }
    
            std::cout << "select a villager to move\n";

            for(int i = 0; i < currentPlaceVillagers.size(); i++)
            {
                std::cout << i + 1 << "-" << currentPlaceVillagers[i]->getName() << std::endl;
            }
            
        

        int choice2;
        std::cin >> choice2;

            if(choice2 < 1 || choice2 > currentPlaceVillagers.size() )
            {
                throw std::out_of_range("the number that you selected for guiding a villager is invalid\n");
            }

        std::shared_ptr<Villager> chosenVillager = currentPlaceVillagers[choice2-1];

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
        
        std::shared_ptr<Place> chosenPlace = neighbors[choose-1];

        currentPlace->deleteVillager(chosenVillager->getName());

        chosenPlace->addVillager(chosenVillager);
        
        chosenVillager->changeLoc(chosenPlace);

        std::cout << "villager->" << chosenVillager->getName() << " guided to -> " << chosenPlace->getName() << std::endl;

        actionCount--;

        }

        else
        {
            throw std::invalid_argument("your choice for guiding is invalid\n ");
        }

}