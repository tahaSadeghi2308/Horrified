#include "tui.hpp"
#include "utility.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include "hero.hpp"
#include "tabulate/table.hpp"
#include "place.hpp"
#include "card_manager.hpp"

using namespace std;
using namespace tabulate;

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
    string bar = string(level, '#') + string(5 - level, '-');
    cout << "🧟 Terror Level: " << bar << " (" << level << " / 5)\n\n";
}

void Tui::showNeighborsInfo(shared_ptr<HeroBase>& hero) {
    vector<string> neis = (sys->getGameMap())[hero->getCurrentPlace()];
    auto boolToString = [](bool val) -> string { return val ? "Yes" : "No"; };
    Table table;
    table.add_row({"Location", "Has Hero", "Has Monster", "Has Item", "Has Local"});
    for (string nei : neis) {
        for (auto loc : sys->getLocations()){
            if (loc->getPlaceName() == nei){
                table.add_row({
                    nei,
                    boolToString(!loc->getAllHeros().empty()),
                    boolToString(!loc->getAllMonsters().empty()),
                    boolToString(!loc->getAllItems().empty()),
                    boolToString(!loc->getAllVillagers().empty())
                });
            }
        }
    }

    table.format().border_top("═").border_bottom("═").border_left("║").border_right("║").corner("╬");
    table[0].format().font_style({FontStyle::bold}).font_color(tabulate::Color::cyan);

    cout << "╔══════════════════════════════════════════════════════╗\n";
    cout << "║               🧭 Neighboring Locations               ║\n";
    cout << "╚══════════════════════════════════════════════════════╝\n";
    cout << table << "\n\n";
}

void Tui::monstersInfo() {
    cout << "╔═══════════════════════╗\n";
    cout << "║     Monster Status    ║\n";
    cout << "╚═══════════════════════╝\n";

    Table table;
    table.add_row({"Monster", "Location", "Clue Items"});
    for (const auto& monster : sys->getMonsters()) {
        string clueText;
        if (monster->getMonsterName() == "dracula"){
            clueText = to_string(sys->foundClues("coffin")) + " coffins found";   
        }
        else {
            clueText = to_string(sys->foundClues("clue")) + " clues found";   
        }
        table.add_row({
            monster->getMonsterName(), 
            monster->getCurrentLocation(), 
            clueText
        });
    }
    table.format()
        .border_top("═").border_bottom("═")
        .border_left("║").border_right("║")
        .corner("╬");

    table[0].format()
        .font_style({FontStyle::bold})
        .font_color(tabulate::Color::yellow);

    cout << table << "\n\n";
}

void Tui::displayActions() const {
    cout << "🧰 Hero Actions:\n";
    cout << "[1] Move  ";
    cout << "[2] Guide Local  ";
    cout << "[3] Pick Up Item  ";
    cout << "[4] Advanced Action  ";
    cout << "[6] Defeat Action\n";
    cout << "[7] Special Action  ";
    cout << "[8] Play perk  ";
    cout << "[9] Exit Game\n\n";
}

void Tui::heroInfo(shared_ptr<HeroBase>& hero){
    cout << "📛 Hero: " << hero->getHeroName() << '\n';

    cout << "📍 Location: " << hero->getCurrentPlace() << '\n';
    cout << "    ├── Neighbors: ";
    for (auto& nei : (sys->getGameMap())[hero->getCurrentPlace()]) cout << nei << " ";

    cout << '\n';
    cout << "    ├── Here Villagers: "; 
    for(auto loc : sys->getLocations()){
        if (loc->getPlaceName() == hero->getCurrentPlace()){
            for(auto vill : loc->getAllVillagers()) cout << vill->getVillagerName() << " ";
            break;
        }
    }
    cout << '\n';
    cout << "    ├── Inventory: ";
    for (auto item : hero->getAllItems()) {
        Table t;
        string text = item.name + "(" + to_string(item.power) + ")";
        t.add_row({text});
        if (item.color == cardColor::Color::RED)
            t[0][0].format().font_color(tabulate::Color::red).font_style({FontStyle::bold});
        else if (item.color == cardColor::Color::YELLOW)
            t[0][0].format().font_color(tabulate::Color::yellow).font_style({FontStyle::bold});
        else if (item.color == cardColor::Color::BLUE)
            t[0][0].format().font_color(tabulate::Color::blue).font_style({FontStyle::bold});
        cout << t << " ";
    }
    cout << "\n\n";
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

void Tui::heroPhasePage(string_view heroName){
    // find hero
    shared_ptr<HeroBase> currentHero {nullptr};
    for (auto h : sys->getHeros()){
        if (h->getHeroName() == heroName) { currentHero = h; break;}
    }
    this->header();
    this->heroInfo(currentHero);
    this->showNeighborsInfo(currentHero);
    this->monstersInfo();
    this->displayActions();
    this->terrorLevel(sys->getTerrorLevel());
}

void Tui::runGame() {
    this->heroPhasePage("arch");
}