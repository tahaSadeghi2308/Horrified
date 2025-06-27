#include "tui.hpp"
#include "utility.hpp"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

Tui::Tui(System *s) : sys(s) {}

void Tui::quitPage() const {
    clearScreen();
    fmt::println("I hope u enjoy this game :(: ");
}

void Tui::header() const {
    cout << "╔════════════════════════════════════════════════════════╗\n";
    cout << "║                   🧟 Horrified Game HUD                ║\n";
    cout << "╚════════════════════════════════════════════════════════╝\n\n";
}

void Tui::terrorLevel(int level) {
    string bar = string(level, '█') + string(5 - level, '░');
    cout << "🧟‍♂️ Terror Level: " << bar << " (" << level << " / 5)\n\n";
}

void Tui::displayActions(string_view turn) const {
    cout << "🧰 Hero Actions:\n";
    cout << "[1] Move\n";
    cout << "[2] Guide Local\n";
    cout << "[3] Pick Up Item\n";
    cout << "[4] Advanced Action\n";
    cout << "[6] Defeat Action\n";
    cout << "[7] Special Action\n";
    cout << "[8] Play perk\n";
    cout << "[9] Exit Game\n\n";
}

void Tui::welcomePage() {
    clearScreen();
    fmt::println("Be careful there are lot of monsters overthere !!!");
    fmt::println("But monsters hate Garlics");
    fmt::println("You can enter q instead of each input to quit the game !!!");
    int player1Days = getCommand("How many days has it been since you last ate garlic player1");
    if (player1Days == -1) { this->quitPage(); return; }
    int player2Days = getCommand("How many days has it been since you last ate garlic player2");
    if (player2Days == -1) { this->quitPage(); return; }
    cout << player1Days << " " << player2Days << '\n';
    if (player1Days > player2Days) {
        fmt::println("Ok , player 1 . Now choose your character: ");
        fmt::println("1. Archaeologist");
        fmt::println("2. Mayor");
        if (getCommand() == 1) this->playerPriority = {"arch" , "mayor"};
        else this->playerPriority = {"mayor" , "arch"};
    }
    fmt::println("ok game is ready to go wait for 2 seconds .....");
    this_thread::sleep_for(chrono::seconds(2));
    clearScreen();
}

void Tui::runGame() {
    this->welcomePage();

}