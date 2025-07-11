#include "tui.hpp"
#include "gui.hpp"
#include <iostream>
#include "system.hpp"
#include <fmt/core.h>

using namespace std;

int main(){
    System *s = new System();
    Gui t(s);
    t.run();
    delete s;
    
    return 0;
}