#include "utility.hpp"
#include <cstdlib>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#endif

void clearScreen(){
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #elif defined(__linux__) || defined(__unix__)
        system("clear");
    #endif
}