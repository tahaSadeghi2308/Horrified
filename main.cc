#include <iostream>
#include "tui.hpp"
using namespace std;

int main(){
    // System *s = new System();
    // Tui t(s); 
    // t.runGame();
    // delete s;
    System s;
    s.systemInfoShow();
    fmt::println("\n-------------------------------");
    s.run();
    s.systemInfoShow();
    return 0;
}