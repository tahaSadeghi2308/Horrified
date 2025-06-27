#include <iostream>
#include "tui.hpp"
using namespace std;

int main(){
    System *s = new System();
    Tui t(s); 
    t.runGame();
    delete s;
    return 0;
}