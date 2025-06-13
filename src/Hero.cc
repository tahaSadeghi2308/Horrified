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
                cout <<"the number that you have selected for moving is invalid\n";
                return;
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

    vector<Item> emptyPlace = currentPlace->getItems();
    if(emptyPlace.empty())
    {
        std::cout << "there is no item in your place\n";
            return;
    }

    bool flag = true;

    while(true)
    {
    std::vector<Item> ITEMS = currentPlace->getItems();

        if(ITEMS.empty())
        {
            std::cout << "you picked all the items in your location (decreasing action)\n";
            break;
        }

        cout << "select your item an pick it up (if you didnt want to continue type -1)\n";

        for (int i = 0; i < ITEMS.size(); i++)
        {
            std::cout << i+1 << " - " << ITEMS[i] << '\n';
        }
        int choice;
        std::cin >> choice;

        if(choice == -1)
        {
            break;
        }

        if(choice < 1 || choice > ITEMS.size())
        {
            cout<<"invalid item \n";
            continue;
        }

        heroItems.push_back(ITEMS[choice-1]);
        cout << "you picked up -> " << ITEMS[choice-1] << std::endl;
        currentPlace->removeItem(ITEMS[choice-1]);
        flag =false;
    }
    if(!flag)
    actionCount--;    
}

//---speciall action-----

void Archaeologist::specialAction()
{
    bool flag = true;

    while(true)
    {
    std::vector<std::shared_ptr<Place>> neigh = getCurrentPlace()->getNeighbors();
        //storing all the possible items with its places with pair. first i tried map but it didnt work (it has seviral problems for us to find out which item user chosed)
        std::vector<std::pair<std::shared_ptr<Place>,Item>> possible;

        int i = 1;
        for (std::shared_ptr<Place> neighbor : neigh) {
            for (auto& item : neighbor->getItems())
            {
                std::cout << i << " - " <<  "place -> " << neighbor->getName() << "item -> " << item << '\n';
                possible.push_back(std::make_pair(neighbor,item));
                i++;
            }
        }

        if(possible.empty())
        {
            std::cout << "there is no item nearby\n";
            break;
        }

        std::cout << "select the item to pick up (type -1 if you dont want to continue)\n";
    int choice;
    std::cin >> choice;

    if(choice == -1)
    {
        break;
    }

    if(choice < 1 || choice > possible.size())
    {
        cout << "invalid selection try again\n";
        continue;
    }

    std::shared_ptr<Place> targetPlace = possible[choice - 1].first;
    Item targetItem = possible[choice - 1].second;

    targetPlace->removeItem(targetItem);   
    addHeroItems(targetItem);
    flag =false;
    }
    if(!flag)
    setActionCount(getActionCount()-1);
    else
    cout << "you picked no item so action wont be cost\n";

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
                cout <<"invalid place choice\n";
                return;
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
                cout << "invalid villager selection\n";
                return;
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
                cout << "the number that you selected for guiding a villager is invalid\n";
                return;
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
            cout <<"the neighbor that you selected to guid a villager is invalid\n";
            return;
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
            cout <<"your choice for guiding is invalid\n ";
            return;
        }

}

void HeroBase::advanceAction(std::vector<std::string>& coffins,std::vector<std::string>& evidence,std::shared_ptr<ItemBag<Item>> bag)
{
    bool flag = true;

    if(currentPlace->getName() == "precinct")
    {
        flag=false;
        std::cout << "your providing evidence for invisible man\n";
        bool check=true;
        std::vector<Item> validItems;
        for(auto& evi:evidence)
        {
            for(auto& item:heroItems)
            {
                if(item.place == evi)
                {
                    check=false;
                    validItems.push_back(item);
                }
            }
        }

        if(check)
        {
            std::cout << "you dont have a item that has come from barn, inn , laboraory, institute or mansion\n";
            return;
        }

        while(true)//if choice was invalid it didnt get out of the advance it will get another value
        {
            for(int i = 0;i < validItems.size();i++)
            {
                std::cout << "the Items that you can choose as evidence\n";
                std::cout << i+1 << " - " << validItems[i] << '\n';
            }

            int choice;
            std::cin >> choice;
            
            if (choice < 1 || choice > validItems.size())
            {
                std::cout << "invalid choice try again\n";
                continue;
            }

                Item selectedItem = validItems[choice - 1];
                for(auto removeItem = heroItems.begin();removeItem!=heroItems.end();removeItem++)
                {
                    if(removeItem->name == selectedItem.name)
                    {
                        heroItems.erase(removeItem);
                        break;
                    }
                }

                bag->addItem(selectedItem);

                for(auto removeItem = evidence.begin();removeItem!=evidence.end();removeItem++)
                {
                    if( *removeItem == selectedItem.place)
                    {
                        evidence.erase(removeItem);
                        break;
                    }
                }

                std::cout << "you successfully added " << selectedItem  << "\nto invisble mans board\n";
                actionCount--;
                break;
        }
    }
    
    for(auto coffin = coffins.begin();coffin!=coffins.end();coffin++)
    {
        if(currentPlace->getName() == *coffin)
        {
            flag = false;
            int totalRedItemPower = 0;
            std::vector<Item> redItems;
        for (const auto& item : heroItems)
        {
            if (item.color == Color::RED)
            {
                totalRedItemPower += item.power;
                redItems.push_back(item);
            }
        }

        if (totalRedItemPower < 6)
        {
            std::cout << "not enough red item to destroy a coffin\n";
            return;   
        }
        std::cout << "destroying a coffin in -> " << *coffin << std::endl;
            std::cout << "choose some red item with a valuation more than 6\n";

            int chosenItemsPower = 0 ;
            std::vector<Item> usedItem;
            while(chosenItemsPower < 6)
            {
                for (int i = 0; i < redItems.size(); i++)
                {
                    std::cout << i + 1 << " - " << redItems[i].name << " Power-> " << redItems[i].power << '\n';
                }
                int choice;
                std::cin >> choice;
                if (choice < 1 || choice > redItems.size())
                {
                    std::cout << "invalid choice try again\n";
                    continue;
                }

                Item selectedItem = redItems[choice - 1];
                usedItem.push_back(selectedItem);
                chosenItemsPower += selectedItem.power;

                redItems.erase(redItems.begin() + (choice - 1));
            }

            for (const auto& item : usedItem) {
                for (auto removeItem = heroItems.begin(); removeItem != heroItems.end(); removeItem++) {
                    if (removeItem->name == item.name)
                    {
                        heroItems.erase(removeItem);
                        break; 
                    }
                }
            }

            for(const auto item : usedItem)
            {
                bag->addItem(item);
            }

            coffins.erase(coffin);
            actionCount--;
            std::cout << "the coffin destroyed successfully in -> " << currentPlace->getName() <<std::endl;             
            break;
    }
}
    if(flag)
    {
        std::cout << "you cant use advanced action in your current location-> " << currentPlace->getName() << '\n';
        return;
    }
}

//-----defeat-----

void HeroBase::defeatAction(const std::vector<std::string>& coffin,const std::vector<std::string>& evidence,std::shared_ptr<ItemBag<Item>> bag)
{

    //using the same method that i used for getting the red items for destroying coffins (copy,past :) )

    
    std::cout << "which monster do you want to defeat\n 1-dracula\n2-invisible man\n"; //this method needs monsters implementation to be complete
                int choice;
                std::cin >> choice;
                if(choice == 1)
                {
                    string unknown;
                    if(currentPlace->getName() != unknown)//monster place)
                    {
                        cerr << "your not in the same place with dracula\n";
                        return;
                    }

                    if(coffin.empty())
                    {
                        std::vector<Item> yellowItems;
                        int totalPower = 0;
                        for (const auto& item : heroItems)
                            {
                                if (item.color == Color::YELLOW)
                                {
                                    yellowItems.push_back(item);
                                    totalPower += item.power;
                                }
                            }
                        if(totalPower < 6)
                        {
                            cout << "you dont have enough yellow item to defeat dracula\n";
                            return;
                        }

                        std::cout << "choose some yellow item with a valuation more than 6\n";

                        std::vector<Item> usedItems;
                        int sumPower = 0;
                    while (sumPower < 6)
                    {
                        for (int i = 0; i < yellowItems.size(); ++i)
                        {
                            std::cout << i+1 << " - " << yellowItems[i].name << " Power->" << yellowItems[i].power << '\n';
                        }
                        int choose;
                        std::cin >> choose;
                        if (choose < 1 || choose > yellowItems.size())
                        {
                            std::cout << "invalid choice try again\n";
                            continue;
                        }
                        Item selected = yellowItems[choose - 1];
                        usedItems.push_back(selected);
                        sumPower += selected.power;
                        yellowItems.erase(yellowItems.begin() + (choose - 1));
                    }

            for(const auto& item : usedItems)
                {
                    for(auto removeItem = heroItems.begin(); removeItem != heroItems.end(); removeItem++)
                        {
                            if (removeItem->name == item.name)
                            {
                                heroItems.erase(removeItem);
                                break;
                            }
                        }
                }

                for(const auto item : usedItems)
                    {
                        bag->addItem(item);
                    }

                        cout << "you defeated dracula!\n";
                        //null the monster
                        actionCount--;
                    }
                    else
                    {
                        cout <<"there is still coffins out there that you have not destroyed!!\n";
                        return;
                    }
                }
                else if(choice == 2)
                {
                    string unkwon;
                    if(currentPlace->getName() != unkwon)//monster place)
                    {
                        cerr << "your not in the same place with invisible man\n";
                        return;
                    }
                    if(evidence.empty())
                    {
                        std::vector<Item> redItems;
                        int totalPower = 0;
                        for (const auto& item : heroItems)
                        {
                            if (item.color == Color::RED)
                            {
                                redItems.push_back(item);
                                totalPower += item.power;
                            }
                        }

                    if (totalPower < 9)
                    {
                        std::cout << "you dont have enough red item power to defeat invisible man\n";
                        return;
                    }

                        std::vector<Item> usedItems;
                        int chosenPower = 0;
                        while (chosenPower < 9)
                        {
                            for (int i = 0; i < redItems.size(); i++)
                            {
                                std::cout << i+1 << " - " << redItems[i].name << " Power-> " << redItems[i].power << '\n';
                            }
                            int choose;
                            std::cin >> choose;
                            if (choose < 1 || choose > redItems.size())
                            {
                                std::cout << "invalid choice try again\n";
                                continue;
                            }
                            Item selected = redItems[choose - 1];
                            usedItems.push_back(selected);
                            chosenPower += selected.power;
                            redItems.erase(redItems.begin() + (choose - 1));
                        }

                        for (const auto& item : usedItems) {
                            for (auto removeItem = heroItems.begin(); removeItem != heroItems.end(); removeItem++)
                            {
                                if (removeItem->name == item.name)
                                {
                                    heroItems.erase(removeItem);
                                    break;
                                }
                            }
                        }

                        for(const auto item : usedItems)
                        {
                            bag->addItem(item);
                        }

                    std::cout << "You defeated the invisible man!\n";
                        //same
                        actionCount--;
                    }
                    else
                    {
                        cout <<"not enough evidence for finding invisable man!!\n";
                        return;
                    }
                }
                else
                {
                    cout << "invalid choice try again\n";
                }
}