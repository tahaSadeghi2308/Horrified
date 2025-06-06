#include "monster_card_deck.hpp"
#include <iostream>

using namespace std;

int main(){
    try {
        MonsterCardDeck temp;
        MonsterCard i = temp.pickOneRandomly();
        cout << i.strikePriorities.size() << '\n';
        if (!i.strikePriorities.size()) cout << i.name << '\n';
    }
    catch ( ... ) {
    }
    return 0;
}