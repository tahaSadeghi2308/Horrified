#include "monster_card_deck.hpp"
#include <fstream>
#include <sstream>
#include "exceptions.hpp"
#include <random>
#include <algorithm>

using namespace std;

MonsterCardDeck::MonsterCardDeck() {
    ifstream file("../data/before_game/monster_cards.txt");
    if(file.is_open()){
        string line , count , itemCount , name , move , dice , pri;
        while(getline(file , line)){
            stringstream stream(line);
            stream >> count >> itemCount >> name >> move 
                >> dice >> pri;
            MonsterCard temp;
            for (int i {}; i < stoi(count); i++){
                temp.name = name; 
                temp.dice = stoi(dice); temp.move = stoi(move);
                temp.itemCount = stoi(itemCount);
                stringstream ss(pri);
                string tmp;
                while(getline(ss , tmp , '_')){
                    temp.strikePriorities.push_back(tmp);
                }
                monsterCards.push_back(temp);
            }
        }
        file.close();
    }
    else {
        // NOTE : put in try block !!!
        throw FileOpenningExecption("couldn't open this file!!!\n"); 
    }
}

void MonsterCardDeck::shuffleCards(){
    random_device rd;
    mt19937 g(rd());
    shuffle(monsterCards.begin(), monsterCards.end(), g);
}

MonsterCard MonsterCardDeck::pickOneRandomly(){
    shuffleCards();
    MonsterCard temp = monsterCards[0]; 
    monsterCards.erase(monsterCards.begin());
    return temp;
}
