#include "system.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

System::System(){
    this->gameInit();
}

void System::gameInit() {
    // this function is for collecting datas for files and put them in right place in system

    // -- collect locations info
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
}

void System::systemInfoShow() const {
    // cout << "all locations name : \n";
    // for (const auto locPointer : this->allLocations){
    //     cout << locPointer->getPlaceName() << ' ';
    // }
    // cout << "\n-----------------------------------------\n";

    // for (const auto& [name , neigs] : this->gameMap){
    //     cout << name << " : ";
    //     for (const auto &n : neigs) cout << n << " ";
    //     cout << '\n'; 
    // }
    for (auto &v : this->allVillagers) cout << v.getVillagerName() << " : " << v.getSafeZone() << '\n';
}