#include "perk_deck.hpp"
#include <fstream>
#include <sstream>
#include "exceptions.hpp"
#include <random>
#include <algorithm>

using namespace std;

PerkDeck::PerkDeck() {
    ifstream file("../data/before_game/perks.txt");
    if(file.is_open()){
        string line , count , name , desc;
        while(getline(file , line)){
            stringstream stream(line);
            stream >> count >> name >> desc;
            Perk temp;
            for (int i {}; i < stoi(count); i++){
                temp.name = name; 
                temp.description = desc;
                perks.push_back(temp);
            }
        }
        file.close();
    }
    else {
        // NOTE : put in try block !!!
        throw FileOpenningExecption("couldn't open this file!!!\n"); 
    }
}

void PerkDeck::shufflePerks(){
    random_device rd;
    mt19937 g(rd());
    shuffle(perks.begin(), perks.end(), g);
}

Perk PerkDeck::pickOneRandomly(){
    shufflePerks();
    Perk temp = perks[0]; 
    perks.erase(perks.begin());
    return temp;
}
