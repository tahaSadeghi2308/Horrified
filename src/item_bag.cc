#include "item_bag.hpp"
#include <fstream>
#include <sstream>
#include "exceptions.hpp"
#include <random>
#include <algorithm>

using namespace std;

ItemBag::ItemBag() {
    ifstream file("../data/before_game/items.txt");
    if(file.is_open()){
        string line , power , color , name , place;
        while(getline(file , line)){
            stringstream stream(line);
            stream >> power >> color >> name >> place;
            Item temp;
            for (int i {}; i < ITEM_COUNT; i++){
                temp.power = stoi(power);
                temp.name = name;
                temp.place = place;
                if (color == "red") temp.color = Color::RED;
                if (color == "blue") temp.color = Color::BLUE;
                if (color == "yellow") temp.color = Color::YELLOW;
                items.push_back(temp);
            }
        }
        file.close();
    }
    else {
        // NOTE : put in try block !!!
        throw FileOpenningExecption("couldn't open this file!!!\n"); 
    }
}

void ItemBag::shuffleItems(){
    random_device rd;
    mt19937 g(rd());
    shuffle(items.begin(), items.end(), g);
}

Item ItemBag::pickOneRandom(){
    shuffleItems();
    Item temp = items[0]; 
    items.erase(items.begin());
    return temp;
};
