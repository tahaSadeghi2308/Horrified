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
}

void System::systemInfoShow() const {
    
    // cout << "\n-----------------------------------------\n";

    // for (const auto& [name , neigs] : this->gameMap){
    //     cout << name << " : ";
    //     for (const auto &n : neigs) cout << n << " ";
    //     cout << '\n'; 
    // }
    // for (auto &v : this->allVillagers) cout << v.getVillagerName() << " : " << v.getSafeZone() << '\n';
    monsters[0]->runMonsterPhase();
    // cout << "all locations name : \n";
    for (const auto locPointer : this->allLocations){
        fmt::print("{}: ", locPointer->getPlaceName());
        for(auto n : locPointer->getAllItems()){
            fmt::print("{} " , n.name);
        }
        fmt::print("\n");
    }
}

Item System::getRandomItem() { 
    // fmt::print("items len : {}" , items.size());  
    return items.pickOneRandomly(); 
}

MonsterCard System::getRandomMonstCard() { 
    return monsterCards.pickOneRandomly(); 
}

void System::putItemInPlace(const string& _placeName , const Item &i){
    for (auto loc : this->allLocations){
        if (loc->getPlaceName() == _placeName) {
            loc->addItem(i);
            break;
        }
    }
}