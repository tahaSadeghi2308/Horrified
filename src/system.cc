#include "system.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include "monster.hpp"
#include "place.hpp"

using namespace std;

System::System(){
    this->gameInit();
}

void System::gameInit() {
    // this function is for collecting datas for files and put them in right place in system

    // collect locations info
    ifstream file ("../data/before_game/locations.txt");
    if (file.is_open()) {
        string line , placeName , neigh;
        while(getline(file , line)){
            stringstream stream;     
            stream.str(line);
            stream >> placeName >> neigh;
            allLocations.push_back(make_shared<Place>(placeName));
            vector<string> neis;
            stringstream ss(neigh);
            string n;
            while(getline(ss , n , '_')) neis.push_back(n);
            gameMap[placeName] = neis;
        }
        file.close();
    }
    else {
        throw FileOpenningExecption("couldn't open file locations.txt");
    }

    // villagers 
    ifstream villFile ("../data/before_game/villagers.txt");
    if (villFile.is_open()) {
        string line , villName , zone;
        while(getline(villFile , line)){
            stringstream stream;     
            stream.str(line);
            stream >> villName >> zone;
            this->allVillagers.push_back(Villager(villName , zone));
        }
        villFile.close();
    }
    else {
        throw FileOpenningExecption("couldn't open file villagers.txt");
    }

    // monsters
    monsters.push_back(make_shared<Dracula>("dracula" , true , this));
    monsters.push_back(make_shared<InvisibleMan>("invisibleMan" , false , this));
    this->moveMonster("dracula" , "camp");
    this->moveMonster("invisibleMan" , "inn");
}

void System::systemInfoShow() const {
    for(auto loc : this->allLocations) {
        fmt::print("\n---------------- {} -----------  \n" , loc->getPlaceName());
        fmt::print("      Here items : ");
        for(auto i : loc->getAllItems()){
            fmt::print("{} " , i.name);
        }
        fmt::print("\n      Here monsters : ");
        for(auto i : loc->getAllMonsters()){
            fmt::print("{} " , i->getMonsterName());
        }
    }
    fmt::print("\n");
    fmt::print("items cout : {}\n" , items.size());
}

Item System::getRandomItem() { return items.pickOneRandomly(); }

MonsterCard System::getRandomMonstCard() { return monsterCards.pickOneRandomly(); }

void System::putItemInPlace(const string& _placeName , const Item &i){
    for (auto loc : this->allLocations){
        if (loc->getPlaceName() == _placeName) {
            loc->addItem(i);
            break;
        }
    }
}

void System::moveMonster(string_view _monsterName , string_view _newPLace){
    shared_ptr<MonsterBase> currentEntity {nullptr};
    for(auto monst : this->monsters){
        if (monst->getMonsterName() == _monsterName) { currentEntity = monst; break; }
    }

    // delete from current place 
    for (auto loc : this->allLocations){
        for(auto monst : loc->getAllMonsters()){
            if (monst->getMonsterName() == currentEntity->getMonsterName()){
                loc->deleteMonster(currentEntity->getMonsterName());
            }
        }
    }

    // add to new place 
    for(auto loc : this->allLocations){
        if(loc->getPlaceName() == _newPLace) loc->addMonster(currentEntity);
    }

    currentEntity->setCurrentLocation(_newPLace);
}

void System::placeWithMaxItem() {
    int maxItemCount {-1};
    shared_ptr<Place> p {nullptr};
    for(auto loc : this->allLocations){
        if (loc->getAllItems().size() > maxItemCount){
            maxItemCount = loc->getAllItems().size();
            p = loc;
        }
    }
    while (maxItemCount--) {
        this->items.addItem(p->getAllItems()[0]);
        p->deleteItem(p->getAllItems()[0].name);
    }

    this->moveMonster("invisibleMan" , p->getPlaceName());
}

void System::run() {
    monsters[0]->runMonsterPhase();
}