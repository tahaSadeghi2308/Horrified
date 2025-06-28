#include "tui.hpp"
#include "utility.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include "hero.hpp"
#include "tabulate/table.hpp"
#include "place.hpp"
#include "card_manager.hpp"
#include "villager.hpp"

using namespace std;
using namespace tabulate;

Tui::Tui(System *s) : sys(s) {}

void Tui::backButton() { this->pageNumber--; }

void Tui::quitPage() const {
    // page number = 9
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

void Tui::heroPhasePage(shared_ptr<HeroBase>& hero , int actions){
    // page Number 0
    clearScreen();
    this->header();
    this->heroInfo(hero);
    fmt::println("Remaining actions : {}" , actions);
    this->showNeighborsInfo(hero);
    this->monstersInfo();
    this->terrorLevel(sys->getTerrorLevel());
    this->displayActions();
    int page = getCommand("Enter a action number ");
    while (page <= 0 || page > 9) {
        fmt::println("invalid page entered !!!");
        page = getCommand("Enter a action number ");
    }
    this->pageNumber = page;
}

void Tui::movePage(shared_ptr<HeroBase>& hero , int &actions) {
    // page Number 1
    clearScreen();
    string current = hero->getCurrentPlace(); 
    vector<string> neis = (sys->getGameMap())[current];
    vector<shared_ptr<Villager>> hereVills;

    for(auto loc : sys->getLocations()){
        if (loc->getPlaceName() == current){
            for(auto vill : loc->getAllVillagers()) hereVills.push_back(vill);
            break;
        }
    }

    fmt::println("Ok, At first you can move yourself to an other place !!!");
    while (true){
        for(int i {}; i < neis.size(); i++){
            cout << i + 1 << ". " << neis[i] << '\n';
        }
        cout << neis.size() + 1 << ". Back" << '\n';
        cout << neis.size() + 2 << ". Exit" << '\n';
        int num = getCommand("Enter a number to choose where u want to go ");

        if (num > 0 && num <= neis.size()) {
            sys->moveHero(hero->getHeroName() , neis[num - 1]);
            actions--;
            break;
        }
        else if (num == neis.size() + 1) { this->backButton(); return; }
        else if (num == neis.size() + 2) { this->pageNumber = 9; return; }
        else fmt::println("Invalid neighboer choies !!!");
    }
    clearScreen();
    fmt::println(
        "Ok, Now {} is in {}. Do u want to move villagers in {}" , 
        hero->getHeroName(),
        hero->getCurrentPlace(),
        current 
    );
    int ch;
    while (true) { 
        ch = getCommand("1.Yes | 2.No");
        if (ch == 1 || ch == 2) break;
        else fmt::println("Invalid choise !!!");
    }
    if (ch == 2) { this->pageNumber = 0; }
    else {
        while (true) {
            int villNum;
            for(int i {}; i < hereVills.size(); i++){
                cout << i + 1 << ". " << hereVills[i]->getVillagerName() << '\n';
            }
            cout << hereVills.size() + 1 << ". Stop moving" << '\n';
            villNum = getCommand("Enter a number ");
            if (villNum > 0 && villNum <= hereVills.size()){
                sys->moveVillager(hereVills[villNum - 1]->getVillagerName() , hero->getCurrentPlace());
                if (hereVills[villNum - 1]->getSafeZone() == hero->getCurrentPlace()){
                    fmt::println(
                        "You reached a villager {} to its Safe zone :)\n He will give u a perk man",
                        hereVills[villNum - 1]->getVillagerName()
                    );
                    hero->addPerk(sys->getRandomPerk());
                }
                hereVills.erase(hereVills.begin() + villNum - 1);
            }
            else if (villNum == hereVills.size() + 1){
                this->pageNumber = 0;
                return;
            }
            else fmt::println("Invalid villager choies !!!");
        }
    }
}

void Tui::runGame() {
    this->welcomePage();
    int round {0};
    int playerCount = playerPriority.size();
    // cout << playerCount; 
    while (this->pageNumber != 9) {
        string currentHeroName = playerPriority[round % playerCount];
        //find hero
        shared_ptr<HeroBase> currentHero {nullptr};
        for (auto h : sys->getHeros()){
            if (h->getHeroName() == currentHeroName) { currentHero = h; break;}
        }

        int actions = currentHero->getActions();
        while(actions != 0 && pageNumber != 9) {
            if (this->pageNumber == 0) this->heroPhasePage(currentHero , actions);
            else if (this->pageNumber == 1) this->movePage(currentHero , actions);
        }
        round++;
    }
    this->quitPage();
}