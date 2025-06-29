#include "tui.hpp"
#include "system.hpp"
#include "villager.hpp"
#include "location.hpp"
#include "Hero.hpp"
#include "tabulate/table.hpp"
#include "card_manager.hpp"
#include <fmt/core.h>
#include "utility.hpp"
#include <chrono>
#include <thread>

using namespace std;

Tui::Tui(System *s): sys(s) {}

void Tui::header() const {
    cout << "╔════════════════════════════════════════════════════════╗\n";
    cout << "║                   🧟 Horrified Game HUD                ║\n";
    cout << "╚════════════════════════════════════════════════════════╝\n\n";
}

void Tui::showNeighborsInfo(shared_ptr<HeroBase>& hero) {
    vector<shared_ptr<Place>> neis = hero->getCurrentPlace()->getNeighbors();
    auto boolToString = [](bool val) -> string { return val ? "Yes" : "No"; };
    tabulate::Table table;
    table.add_row({"Location", "Has Hero", "Has Monster", "Has Item", "Has Villager"});
    for (auto nei : neis) {
        table.add_row({
            nei->getName(),
            boolToString(!nei->getAllHeroes().empty()),
            boolToString(!nei->getMonsters().empty()),
            boolToString(!nei->getItems().empty()),
            boolToString(!nei->getVillagers().empty())
        });
    }

    table.format().border_top("═").border_bottom("═").border_left("║").border_right("║").corner("╬");
    table[0].format().font_style({tabulate::FontStyle::bold}).font_color(tabulate::Color::cyan);

    cout << "╔══════════════════════════════════════════════════════╗\n";
    cout << "║               🧭 Neighboring Locations               ║\n";
    cout << "╚══════════════════════════════════════════════════════╝\n";
    cout << table << "\n\n";
}

void Tui::monstersInfo() {
    cout << "╔═══════════════════════╗\n";
    cout << "║     Monster Status    ║\n";
    cout << "╚═══════════════════════╝\n";

    tabulate::Table table;
    table.add_row({"Monster", "Location", "Clue Items"});
    for (const auto& monster : sys->getAllMonsters()) {
        string clueText;
        if (monster->getMonsterName() == "dracula"){
            clueText = to_string(sys->foundCluesCount("coffin")) + " coffins found";   
        }
        else {
            clueText = to_string(sys->foundCluesCount("clue")) + " clues found";   
        }
        table.add_row({
            monster->getMonsterName(), 
            monster->getCurrentLocation()->getName(), 
            clueText
        });
    }
    table.format()
        .border_top("═").border_bottom("═")
        .border_left("║").border_right("║")
        .corner("╬");

    table[0].format()
        .font_style({tabulate::FontStyle::bold})
        .font_color(tabulate::Color::yellow);

    cout << table << "\n\n";
}

void Tui::heroInfo(shared_ptr<HeroBase>& hero){
    cout << "📛 Hero: " << hero->getHeroName() << '\n';
    cout << "📍 Location: " << hero->getCurrentPlace()->getName() << '\n';
    cout << "    ├── Neighbors: ";
    for (auto nei : hero->getCurrentPlace()->getNeighbors()) cout << nei->getName() << " ";

    cout << '\n';
    cout << "    ├── Here Villagers: "; 
    for (auto vill : hero->getCurrentPlace()->getVillagers()) cout << vill->getName() << " ";
    cout << '\n';
    cout << "    ├── Hero Items: ";
    for (auto item : hero->getHeroItems()) {
        tabulate::Table t;
        string text = item.name + "(" + to_string(item.power) + ")";
        t.add_row({text});
        if (item.color == Color::RED)
            t[0][0].format().font_color(tabulate::Color::red).font_style({tabulate::FontStyle::bold});
        else if (item.color == Color::YELLOW)
            t[0][0].format().font_color(tabulate::Color::yellow).font_style({tabulate::FontStyle::bold});
        else if (item.color == Color::BLUE)
            t[0][0].format().font_color(tabulate::Color::blue).font_style({tabulate::FontStyle::bold});
        cout << t << " ";
    }
    cout << "\n\n";
}

void Tui::quitPage() const {
    // page number = 9
    clearScreen();
    fmt::println("I hope u enjoy this game :(: ");
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
    int playerNumber = (player1Days > player2Days) ? 1 : 2;
    while(true) {
        fmt::println("Ok , player {} . Now choose your character: " , playerNumber);
        fmt::println("1. Archaeologist");
        fmt::println("2. Mayor");
        int num = getCommand();
        if (num == 1) {
            this->playerPriority.push_back("arch");
            this->playerPriority.push_back("mayor");
            break;
        }
        else if (num == 2) {
            this->playerPriority.push_back("mayor");
            this->playerPriority.push_back("arch");
            break;
        }
        else fmt::println("Invalid argument entered !!!");
    }
    fmt::println("ok game is ready to go wait for 2 seconds .....");
    this_thread::sleep_for(chrono::seconds(2));
    clearScreen();
}