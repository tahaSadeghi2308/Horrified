#ifndef CARD_MANAGER_HPP
#define CARD_MANAGER_HPP

#include <vector>
#include <string>
#include <random>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "exceptions.hpp"

enum Color { RED , BLUE , YELLOW };

constexpr int ITEM_COUNT {2};

struct Item {
    int power;
    Color color;
    std::string name;
    std::string place; // NOTE: maybe change base on place implementation
};

struct Perk {
    std::string name;
    std::string description;
};

struct MonsterCard {
    std::string name;
    int dice;
    int move;
    int itemCount;
    std::vector<std::string> strikePriorities;
};

template <class T>
class CardManagerBase {
    void shuffleCards();
    std::vector<T> cards;
public:
    CardManagerBase<T>() = default;
    T pickOneRandomly();
    void push(const T &temp);
    void pop(const int index);
};

template <class T>
class ItemBag : public CardManagerBase<T> {
public:
    ItemBag<T>();
    void addItem(const T &item);
};

template <class T>
class PerkDeck : public CardManagerBase<T> {
public:
    PerkDeck<T>();
};

template <class T>
class MonsterCardDeck : public CardManagerBase<T> {
public:
    MonsterCardDeck<T>();
};

template<class T>
void ItemBag<T>::addItem(const T &item){
    this->push(item);
}

template <class T>
void CardManagerBase<T>::shuffleCards() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

template <class T>
void CardManagerBase<T>::push(const T &item) {
    this->cards.push_back(item);
}

template <class T>
void CardManagerBase<T>::pop(const int index) {
    this->cards.erase(cards.begin() + index);
}

template <class T>
T CardManagerBase<T>::pickOneRandomly() {
    shuffleCards();
    T temp = cards[0];
    this->pop(0);
    return temp;
}

template <class T>
MonsterCardDeck<T>::MonsterCardDeck() {
    std::ifstream file("../data/before_game/monster_cards.txt");
    if(file.is_open()) {
        std::string line, count, itemCount, name, move, dice, pri;
        while(getline(file, line)) {
            std::stringstream stream(line);
            stream >> count >> itemCount >> name >> move >> dice >> pri;
            T temp;
            for (int i {}; i < std::stoi(count); i++) {
                temp.name = name; 
                temp.dice = std::stoi(dice); 
                temp.move = std::stoi(move);
                temp.itemCount = std::stoi(itemCount);
                std::stringstream ss(pri);
                std::string tmp;
                while(getline(ss, tmp, '_')) {
                    temp.strikePriorities.push_back(tmp);
                }
                this->push(temp);
            }
        }
        file.close();
    }
    else {
        throw FileOpenningExecption("couldn't open this file!!!\n"); 
    }
}

template <class T>
ItemBag<T>::ItemBag() {
    std::ifstream file("../data/before_game/items.txt");
    if(file.is_open()) {
        std::string line, power, color, name, place;
        while(getline(file, line)) {
            std::stringstream stream(line);
            stream >> power >> color >> name >> place;
            T temp;
            for (int i {}; i < ITEM_COUNT; i++) {
                temp.power = std::stoi(power);
                temp.name = name;
                temp.place = place;
                if (color == "red") temp.color = Color::RED;
                if (color == "blue") temp.color = Color::BLUE;
                if (color == "yellow") temp.color = Color::YELLOW;
                this->push(temp);
            }
        }
        file.close();
    }
    else {
        throw FileOpenningExecption("couldn't open this file!!!\n"); 
    }
}

template <class T>
PerkDeck<T>::PerkDeck() {
    std::ifstream file("../data/before_game/perks.txt");
    if(file.is_open()) {
        std::string line, count, name, desc;
        while(getline(file, line)) {
            std::stringstream stream(line);
            stream >> count >> name >> desc;
            T temp;
            for (int i {}; i < std::stoi(count); i++) {
                temp.name = name; 
                temp.description = desc;
                this->push(temp);
            }
        }
        file.close();
    }
    else {
        throw FileOpenningExecption("couldn't open this file!!!\n"); 
    }
}

#endif // CARD_MANAGER_HPP