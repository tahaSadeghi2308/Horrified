#include "utility.hpp"
#include <cstdlib>
#include <fmt/core.h>
#include <sstream>
#include <iostream>
#include <cctype>

using namespace std;

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#endif

void clearScreen() {
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #elif defined(__linux__) || defined(__unix__)
        system("clear");
    #endif
}

bool isNumber(string sequence){
    if (!sequence.empty()){
        string newSeq;
        if (sequence[0] == '+' || sequence[0] == '-'){
            for(int i {1}; i < sequence.size(); i++) newSeq += sequence[i];
            for(char ch : newSeq){
                if (!isdigit(ch)){
                    return false;
                }
            }
            return true;
        }
        else {
            for(char ch : sequence){
                if (!isdigit(ch)){
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

int getCommand(string prompt){
    while (true) {
        string line , num , rest;
        fmt::print("{} > " , prompt); getline(cin , line);
        stringstream stream;
        stream.str(line);
        stream >> num; getline(stream , rest);
        if (rest == ""){
            try {
                int number;
                if (num == "q"){
                    return -1;
                }
                else if (isNumber(num)){
                    number = stoi(num);
                    if (number <= 0) throw invalid_argument("negetive number");
                    return number;
                } 
                else {
                    fmt::print("invalid input for number !!!\n");
                }
            }
            catch ( ... ){
                fmt::print("invalid input for number !!!\n");
            }
        }
        else {
            fmt::print("invalid input man !!!\n");
        }
    }
}
