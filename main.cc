#include "item_bag.hpp"
#include <iostream>

using namespace std;

int main(){
    try {
        ItemBag bag;
        Item i = bag.pickOneRandom();
        cout << i.name << '\n'; 
    }
    catch ( ... ) {
        cout << "fuck you !! error in file openning\n";
    }
}