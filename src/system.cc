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
                for(auto &loc : allLocations){
                    if(loc->getName() == n){
                        neigList.push_back(loc);
                    }
                }
            }

            for(auto &loc : allLocations){
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
    for(auto &p : allLocations){
        if 
    }


}

void System::showLocs() const {
    for(const auto &loc : allLocations){
        cout << loc->getName() << " : ";
        for(auto nei : loc->getNeighbors()){
            cout << nei->getName() << " ";
        }
        cout << '\n';
    }
}