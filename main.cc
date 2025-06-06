#include "perk_deck.hpp"
#include <iostream>

using namespace std;

int main(){
    try {
        PerkDeck temp;
        Perk i = temp.pickOneRandomly();
        cout << i.name << '\n';
    }
    catch ( ... ) {
        cout << "fuck you !! error in file openning\n";
    }
    return 0;
}