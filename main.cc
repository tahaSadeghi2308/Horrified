#include "tui.hpp"
#include "gui.hpp"
#include <iostream>
#include "system.hpp"
#include <fmt/core.h>

using namespace std;

int main(){
    
    const int SCREEN_WIDTH = 1600;
    const int SCREEN_HEIGHT = 900;
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT,"Horrified");
    SetTargetFPS(60);

    System *s = new System();
    Gui t(s,SCREEN_WIDTH,SCREEN_HEIGHT);
    t.run();
    delete s;
    
    return 0;
}