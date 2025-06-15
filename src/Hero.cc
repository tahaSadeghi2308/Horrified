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

//-----perk-----

void HeroBase::runPerkCard(shared_ptr<Archaeologist> arch, shared_ptr<Mayor> mayor,shared_ptr<Dracula> dracula,shared_ptr<InvisibleMan> invisible,vector<shared_ptr<Place>> allLocations,shared_ptr<ItemBag<Item>> bag)
{
    //one perk is left
    if(heroPerks.empty())
    {
        cout << "you have no perk card to use\n";
        return;
    }
    cout << "select your perk card\n";
    for(int i=0; i < heroPerks.size();i++)
    {
        cout << i+1 << " - " << heroPerks[i].name << endl;
    }
    int choice;
    cin >> choice;
    string chosenPerksName = heroPerks[choice-1].name;
    if(chosenPerksName == "Late_into_the_Night")
    {
        actionCount += 2;
        heroPerks.erase(heroPerks.begin() + choice-1);
        cout << "two more anction is added to your hero you have -> " << actionCount << " left\nthe following items are added to your location\n";
        for(int i = 0;i < 2;i++)
        {
            Item addItem= bag->pickOneRandomly();
            cout << "added -> " << addItem << endl;
            currentPlace->addItem(addItem);   
        }
    }
    else if(chosenPerksName == "Visit_from_the_Detective")
    {
        cout << "select the position that you want to put invisible man\n";
        for(int i=0;i < allLocations.size() ;i++)
        {
            cout << i+1 << " - " << allLocations[i]->getName() << endl;
        }
        int locChoice;
        cin >> locChoice;
        if(locChoice < 1 || locChoice > allLocations.size())
        {
            cout << "your choice was invalid\n";
            return;
        }
        shared_ptr<Place> selectedPlace = allLocations[locChoice-1];
        invisible->getCurrentLocation()->deleteMonster(invisible->getMonsterName());
        invisible->setCurrentLocation(selectedPlace);
        selectedPlace ->addMonster(invisible);

        heroPerks.erase(heroPerks.begin() + choice-1);
        cout << "invisible man moved to -> " << selectedPlace->getName() << endl;
    }
    else if(chosenPerksName == "Overstock")
    {
        auto allItems = bag->getCards();
        vector<Item> possibleItems;
        vector<int> index;
        for(int i = 0; i < allItems.size(); i++)
            {
                if(i == 0 || allItems[i].name != allItems[i-1].name)
                {
                    possibleItems.push_back(allItems[i]);
                    index.push_back(i);
                }
            }
        cout << "select the item you want to add to your location\n";
        for(int i = 0; i < possibleItems.size(); i++)
        cout << i+1 << " - " << possibleItems[i] << endl;
        int itemChoice; 
        cin >> itemChoice;
        if(itemChoice < 1 || itemChoice > possibleItems.size())
        {
            cout << "invalid choice\n";
            return;
        }
        currentPlace->addItem(possibleItems[itemChoice-1]);

        bag->pop(index[itemChoice-1]);
        heroPerks.erase(heroPerks.begin() + choice-1);
        cout << "the item -> "<< possibleItems[itemChoice-1].name << " has been added to your location\n";
    }
    else if(chosenPerksName == "Hurry")
    {
        cout << "which hero do you want to move for two space\n1-Archaeologist\n2-Mayor\n";
        int heroChoice;
        cin >> heroChoice;
        if(heroChoice != 1 && heroChoice != 2)
        {
            cout << "invalid choice\n";
            return;
        }
        shared_ptr<HeroBase> selectedHero ;
        if(heroChoice == 1)
            selectedHero = arch;
        else
            selectedHero = mayor;

            selectedHero->moveAction();
            cout << "do you want to continue?( no -> 0  yes -> 1 )\n";
            while(true)
            {
                int choose;
                cin >> choose;
                if(choose == 0)
                {
                    break;
                }
                else if(choose == 1)
                {
                    selectedHero->moveAction();
                    break;
                }
                else
                {
                    cout << "invalid choice try again\n";
                    continue;
                }
            }
            heroPerks.erase(heroPerks.begin() + choice-1);
            return;    
    }
    else if(chosenPerksName == "Repel")
    {
        cout << "which monster do you want to move for two space\n1-Dracula\n2-invisibleMan\n";
        int monsterChoice;
        cin >> monsterChoice;
        if(monsterChoice != 1 && monsterChoice != 2)
        {
            cout << "invalid choice\n";
            return;
        }
        shared_ptr<MonsterBase> selectedMonster;
        if(monsterChoice == 1)
        {
            selectedMonster = dracula;
        }
        else
        {
            selectedMonster = invisible;
        }


        for(int j = 0;j < 2;j++)
        {
        vector<shared_ptr<Place>> targetPlaces = selectedMonster->getCurrentLocation()->getNeighbors();
    
            for(int i = 0;i < targetPlaces.size();i++)
            {    
                cout << i+1 << '-' << targetPlaces[i]->getName() << std::endl;
            }
            int choose;
            cin >> choose;
            if(choose < 1 || choose > targetPlaces.size() )
            {
                cout <<"the number that you have selected for moving is invalid\n";
                return;
            }

            string name = selectedMonster->getMonsterName();
            shared_ptr<Place> target = selectedMonster->getCurrentLocation();


            targetPlaces[choose-1] -> addMonster(target ->getOneMonster(name));
            selectedMonster->getCurrentLocation()->deleteMonster(selectedMonster->getMonsterName());
            selectedMonster->setCurrentLocation(targetPlaces[choose-1]);

            if (j == 0)
            {
                cout << "Do you want to move the monster a second time? (0 -> no, 1 -> yes)\n";
                int repeat;
                cin >> repeat;
                while (repeat != 0 && repeat != 1)
                {
                    cout << "invalid input try again (0 = no, 1 = yes)\n";
                    cin >> repeat;
                }
                if (repeat == 0)
                break;
            }
        }
        heroPerks.erase(heroPerks.begin() + choice-1);
    }
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

            shared_ptr<Place> finalPlace = targetPlaces[choice-1];
            vector<shared_ptr<Villager>> PossibleVillager = currentPlace ->getVillagers();

            if(!PossibleVillager.empty())
            {
                cout << "select which villager you want to move by yourself(when you finished selecting type -> -1 )\n";
                while(true)
                {   
                    if(PossibleVillager.empty())
                        break;
                    for (int i = 0; i < PossibleVillager.size() ; i++)
                    {
                        cout << i+1 << " - " << PossibleVillager[i]->getName() << endl;
                    }
                int villChoice;
                cin >> villChoice;
                    if(villChoice == -1)
                        break;
                    if(villChoice < 1 || villChoice > PossibleVillager.size())
                    {
                        cout << "invalid choice try again\n";
                        continue;
                    }
                    shared_ptr<Villager> chosenVill = PossibleVillager[villChoice-1];
                    
                    
                    finalPlace->addVillager(chosenVill);
                    currentPlace->deleteVillager(chosenVill->getName());
                    chosenVill->changeLoc(finalPlace);
                    PossibleVillager.erase(PossibleVillager.begin() + villChoice-1 );
                    cout << "you moved -> " << chosenVill->getName() << " to -> " << finalPlace->getName() << endl;
                }
            }

            finalPlace -> addHero(currentPlace->getHeros(getHeroName()));
            currentPlace -> deleteHero(getHeroName());
            setCurrentPlace(finalPlace);

            cout << "you moved to -> " << finalPlace->getName() << endl;
            
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
        //storing all the possible items with its places with pair. first i tried map but it didnt work (it has several problems for us to find out which item user chosed)
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

void HeroBase::defeatAction(const std::vector<std::string>& coffin,const std::vector<std::string>& evidence,std::shared_ptr<ItemBag<Item>>& bag ,shared_ptr<Dracula>& dracula,shared_ptr<InvisibleMan>& invisible)
{

    //using the same method that i used for getting the red items for destroying coffins (copy,past :) )

    
    std::cout << "which monster do you want to defeat\n 1-dracula\n2-invisible man\n"; //this method needs monsters implementation to be complete
                int choice;
                std::cin >> choice;
                if(choice == 1)
                {
                    if(currentPlace->getName() != dracula->getCurrentLocation()->getName())//monster place)
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
                        dracula->getCurrentLocation()->deleteMonster(dracula->getMonsterName());
                        dracula=nullptr;
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
                    if(currentPlace->getName() != invisible->getCurrentLocation()->getName())//monster place)
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
                        invisible->getCurrentLocation()->deleteMonster(invisible->getMonsterName());
                        invisible=nullptr;
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