#include "tui.hpp"
#include "utility.hpp"
#include <iostream>

using namespace std;

Tui::Tui(System *s) : sys(s) {}

void Tui::welcomePage() {
    clearScreen();
    fmt::println("Be careful there are lot of monsters overthere !!!");
    fmt::println("But monsters hate Garlics");
    int player1Days = getCommand("How many days has it been since you last ate garlic player1");
    int player2Days = getCommand("How many days has it been since you last ate garlic player2");
    cout << player1Days << " " << player2Days << '\n';
    if (player1Days > player2Days) {
        fmt::println("Ok , player 1 . Now choose your character: ");
        fmt::println("1. Archaeologist");
        fmt::println("2. Mayor");
        if (getCommand() == 1) this->playerPriority = {"arch" , "mayor"};
        else this->playerPriority = {"mayor" , "arch"};
    }
    fmt::println("ok game is ready to go .....");
}

void Tui::runGame() {
    this->welcomePage();
}