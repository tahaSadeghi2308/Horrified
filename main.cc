#include <iostream>
#include "system.hpp"
using namespace std;

int main(){
    System s;
    s.run();
    // s.systemInfoShow();
    // fmt::print("\n ----------------------------- after -----------------------------\n");
    s.systemInfoShow();
    return 0;
}