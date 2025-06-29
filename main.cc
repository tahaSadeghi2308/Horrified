#include "tui.hpp"
#include <iostream>
#include "system.hpp"
#include <fmt/core.h>

using namespace std;

int main(){
    System *s = new System();
    Tui t(s);
    t.runGame();
    delete s;
    // System s;
    // s.showLocs();
    // s.runGame();
    // fmt::println("\n-----------------------------------");
    // s.showLocs();

    // System game;
    // game.runGame();
    
    // game.showLocs();
    // game.runHeroPhase();
    
    // game.runMonsterPhase();
    // cout << "--------------------------------------------------------------\n";
    // game.showLocs();
    return 0;
}
