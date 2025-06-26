#include <iostream>
#include "system.hpp"
using namespace std;

int main(){
    System s;
    s.systemInfoShow();
    // fmt::print("\n ----------------------------- after -----------------------------\n");
    s.run();
    s.systemInfoShow();
    return 0;
}