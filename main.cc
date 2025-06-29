#include <iostream>
#include "tui.hpp"
using namespace std;

int main(){
    try 
    {
        System *s = new System();
        Tui t(s); 
        t.runGame();
        delete s;
    }
    catch ( ... )
    {
        cout << "ooops\n";
    }
    // System s;
    // s.systemInfoShow();
    // fmt::println("\n-------------------------------");
    // s.run();
    // s.systemInfoShow();
    return 0;
}