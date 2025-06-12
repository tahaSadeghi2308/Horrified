#include "system.hpp"
#include <fstream>
#include "exceptions.hpp"
#include <sstream>
#include <iostream>
using namespace std;

System::System(){
    // Here we initialize the game
    monsterDeck = make_shared<MonsterCardDeck<MonsterCard>>();
    itemBag = make_shared<ItemBag<Item>>();
    perkDeck = make_shared<PerkDeck<Perk>>();

    // setup heroes
    arch = make_shared<Archaeologist>(4 , "archaeologist" , perkDeck->pickOneRandomly());
    mayor = make_shared<Mayor>(6 , "mayor" , perkDeck->pickOneRandomly());

    // setup monsters
    dracula = make_shared<Dracula>("dracula" , true);
    invisibleMan = make_shared<InvisibleMan>("invisibleMan" , false);

    // ----- collect the name of all locations
    ifstream file ("../data/before_game/locations.txt");
    if (file.is_open()) {
        stringstream stream;     
        string line , placeName;
        while(getline(file , line)){
            stream.str(line);
            stream >> placeName;
            allLocations.push_back(make_shared<Place>(placeName));
        }
        file.close();
    }
    else {
        throw FileOpenningExecption("couldn't open file locations.txt");
    }

    // add neighbors to locations
    ifstream file2("../data/before_game/locations.txt");
    if (file2.is_open()) {
        string line , placeName , neighbors;
        while(getline(file2 , line)){
            stringstream stream(line);
            stream >> placeName >> neighbors;

            // seprate neighbors here
            stringstream ss(neighbors);
            vector<string> temp; string x;
            while(getline(ss , x , '_')) temp.push_back(x);

            vector<shared_ptr<Place>> neigList;

            for(const string &n : temp){
                for(auto loc : allLocations){
                    if(loc->getName() == n){
                        neigList.push_back(loc);
                    }
                }
            }

            for(auto loc : allLocations){
                if(loc->getName() == placeName){
                    loc->setNeighbors(neigList);
                    break;
                }
            }
        }
        file2.close();
    }
    else {
        throw FileOpenningExecption("couldn't open file locations.txt");
    }

    // put heroes in right place
    // we have to set it in place and in hero 
    for(auto place : allLocations){
        if (place->getName() == "docks"){
            place->addHero(arch);
            arch->setCurrentPlace(place);
        }
        else if (place->getName() == "theatre"){
            place->addHero(mayor);
            mayor->setCurrentPlace(place);
        }
    }

    // TODO : here we should make a villiagers list ..
    ifstream villFile("../data/before_game/villagers.txt");
    if(villFile.is_open()){
        string line , name , safeZone;
        while(getline(villFile , line)){
            stringstream stream(line);
            stream >> name >> safeZone;
            shared_ptr<Villager> temp {make_shared<Villager>(name)};

            for(auto _place : allLocations){
                if (_place->getName() == safeZone){
                    temp->setSafeZone(_place);
                    _place->addVillager(temp);
                }
            }

            this->allVillagers.push_back(temp);
        }
        villFile.close();
    }
    else {
        throw FileOpenningExecption("couldn't open villager.txt");
    } 

    // add location to our monsters
    for (auto loc : allLocations){
        if(loc->getName() == "docks"){
            dracula->setCurrentLocation(loc);
            loc->addMonster(dracula);
        }
        else if(loc->getName() == "inn"){
            invisibleMan->setCurrentLocation(loc);
            loc->addMonster(invisibleMan);
        }
    }
}

void System::showLocs() const {
    for(auto loc : allVillagers){
        cout << loc->getName() << " : ";
        if (loc->getVillagerLoc() != nullptr){
            cout << loc->getVillagerLoc()->getName();
        }
        cout << '\n';
    }
}

void System::runMonsterPhase(){
    // first pick a monster card 
    MonsterCard currentCard {monsterDeck->pickOneRandomly()};
    // TODO: Here we can show which card picked and is running !!


    // put items in right place 
    for (int i {}; i < currentCard.itemCount; i++){
        Item thisItem {itemBag->pickOneRandomly()};
        for (auto place : this->allLocations){
            if (place->getName() == thisItem.place){
                place->addItem(thisItem);
            }
        }
    }

    // run the card event !!!
    if (currentCard.name == "Sunrise") {
        for(auto place : this->allLocations){
            if (place->getName() == "crypt"){
                dracula->getCurrentLocation()->deleteMonster("dracula");
                dracula->setCurrentLocation(place);
                place->addMonster(dracula);
            }
        }
    }

    else if (currentCard.name == "Thief"){
        int maxItemCount {0};
        shared_ptr<Place> p {nullptr};

        // search for max of items place
        for (auto place : this->allLocations){
            if (place->getItems().size() > maxItemCount) {
                maxItemCount = place->getItems().size();
                p = place;
            }
        }
        for (auto _item : p->getItems()){
            itemBag->addItem(_item);
        }
        p->getItems().clear();

        invisibleMan->getCurrentLocation()->deleteMonster("invisibleMan");
        invisibleMan->setCurrentLocation(p);
        p->addMonster(invisibleMan);
    }

    else if (currentCard.name == "The_Delivery"){
        for (auto _vill : this->allVillagers){
            if (_vill->getName() == "wilbur&chick"){
                
                if (_vill->getVillagerLoc() != nullptr){
                    _vill->getVillagerLoc()->deleteVillager("wilbur&chick");
                }

                for (auto _place : this->allLocations){
                    if (_place->getName() == "docks") {
                        _vill->changeLoc(_place);
                        _place->addVillager(_vill);
                    }
                }
            }
        }
    }

    else if (currentCard.name == "Fortune_Teller"){
        for (auto _vill : this->allVillagers){
            if (_vill->getName() == "maleva"){
                
                if (_vill->getVillagerLoc() != nullptr){
                    _vill->getVillagerLoc()->deleteVillager("maleva");
                }

                for (auto _place : this->allLocations){
                    if (_place->getName() == "camp") {
                        _vill->changeLoc(_place);
                        _place->addVillager(_vill);
                    }
                }
            }
        }
    }

    else if (currentCard.name == "Former_Employer"){
        for (auto _vill : this->allVillagers){
            if (_vill->getName() == "dr.cranly"){
                
                if (_vill->getVillagerLoc() != nullptr){
                    _vill->getVillagerLoc()->deleteVillager("dr.cranly");
                }

                for (auto _place : this->allLocations){
                    if (_place->getName() == "laboratory") {
                        _vill->changeLoc(_place);
                        _place->addVillager(_vill);
                    }
                }
            }
        }
    }

    else if (currentCard.name == "Hurried_Assistant"){
        for (auto _vill : this->allVillagers){
            if (_vill->getName() == "fritz"){
                
                if (_vill->getVillagerLoc() != nullptr){
                    _vill->getVillagerLoc()->deleteVillager("fritz");
                }

                for (auto _place : this->allLocations){
                    if (_place->getName() == "tower") {
                        _vill->changeLoc(_place);
                        _place->addVillager(_vill);
                    }
                }
            }
        }
    }

    else if (currentCard.name == "The_Innocent"){
        for (auto _vill : this->allVillagers){
            if (_vill->getName() == "maria"){
                
                if (_vill->getVillagerLoc() != nullptr){
                    _vill->getVillagerLoc()->deleteVillager("maria");
                }

                for (auto _place : this->allLocations){
                    if (_place->getName() == "barn") {
                        _vill->changeLoc(_place);
                        _place->addVillager(_vill);
                    }
                }
            }
        }
    }

    else if (currentCard.name == "Egyptian_Expert"){
        for (auto _vill : this->allVillagers){
            if (_vill->getName() == "prof.pearson"){
                
                if (_vill->getVillagerLoc() != nullptr){
                    _vill->getVillagerLoc()->deleteVillager("prof.pearson");
                }

                for (auto _place : this->allLocations){
                    if (_place->getName() == "cave") {
                        _vill->changeLoc(_place);
                        _place->addVillager(_vill);
                    }
                }
            }
        }
    }

    else if (currentCard.name == "The_Ichthyologist"){
        for (auto _vill : this->allVillagers){
            if (_vill->getName() == "dr.reed"){
                
                if (_vill->getVillagerLoc() != nullptr){
                    _vill->getVillagerLoc()->deleteVillager("dr.reed");
                }

                for (auto _place : this->allLocations){
                    if (_place->getName() == "institute") {
                        _vill->changeLoc(_place);
                        _place->addVillager(_vill);
                    }
                }
            }
        }
    }

    // strike phase :D -----------------------------------

}