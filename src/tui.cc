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
#include <unordered_set>
#include <unordered_map>

using namespace std;
using namespace tabulate;

Tui::Tui(System *s) : sys(s) {}

void Tui::backButton() { this->pageNumber = PageNumbers::HERO_PHASE_PAGE; }

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
    table.add_row({"Location", "Has Hero", "Has Monster", "Has Item", "Has Villager"});
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
    cout << "[9] Exit Game\n";
    cout << "[10] Help Page\n\n";
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
    cout << "    ├── Hero Items: ";
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
        else if (num == neis.size() + 2) { this->pageNumber = PageNumbers::EXIT_PAGE; return; }
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
    if (ch == 2) { this->pageNumber = PageNumbers::HERO_PHASE_PAGE; }
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
                    // TODO: Add delete villager from system here
                    sys->killVillager(hereVills[villNum - 1]->getVillagerName());
                    hero->addPerk(sys->getRandomPerk());
                }
                hereVills.erase(hereVills.begin() + villNum - 1);
            }
            else if (villNum == hereVills.size() + 1){
                this->pageNumber = PageNumbers::HERO_PHASE_PAGE;
                return;
            }
            else fmt::println("Invalid villager choies !!!");
        }
    }
}

void Tui::advancedPage(shared_ptr<HeroBase>& hero , int &actions){
    // page number 4
    clearScreen();
    fmt::println("first clarify which of monsters u want to advance");
    int ch;
    while (true){
        fmt::println("1. Dracula");
        fmt::println("2. Invisible Man");
        fmt::println("3. Back");
        fmt::println("4. Exit");
        ch = getCommand("Enter your choise to advance");
        if(ch == 3) { this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return; }
        else if(ch == 4) { this->pageNumber = PageNumbers::EXIT_PAGE; return; }
        else if ( ch < 0 || ch > 4) {
            fmt::println("Invaid choise for advance!!!");
        }
        else break;
    }
    if (ch == 1){
        // TODO : dracula advance here
        // collect hero red items
        vector<Item> redItems;
        int itemsPowerSum = 0;
        for(auto i : hero->getAllItems()){
            if (i.color == cardColor::RED) { 
                redItems.push_back(i); 
                itemsPowerSum += i.power;
                if (itemsPowerSum >= 6) break; 
            }
        }
        if ( itemsPowerSum < 6 ){
            fmt::println("you cant advance for dracula !!");
            this_thread::sleep_for(chrono::seconds(2));
            this->pageNumber = PageNumbers::ADVANCED_PAGE; return;
        }
        else {
            if (
                hero->getCurrentPlace() != "cave" ||
                hero->getCurrentPlace() != "crypt" ||
                hero->getCurrentPlace() != "dungeon" ||
                hero->getCurrentPlace() != "graveyard"
            ){
                fmt::println("you cant advance for dracula . there is no coffin here!!");
                this_thread::sleep_for(chrono::seconds(2));
                this->pageNumber = PageNumbers::ADVANCED_PAGE; return;
            }
            else {
                // pay items
                for(auto i : redItems){
                    hero->deleteItem(i.name);
                    sys->addItem(i);
                }
                if (!sys->allCluesFound("coffin")) sys->makeFoundClues("coffin");
                actions--;
                fmt::println("Fuck !!! we destroied one coffin");
                this_thread::sleep_for(chrono::seconds(2));
                this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return;
            }
        }
    }
    else if (ch == 2){
        vector<Item> clueItems;
        for(auto i : hero->getAllItems()){
            if (
                i.place == "inn" || 
                i.place == "barn" ||
                i.place == "mansion" ||
                i.place == "laboratory" ||
                i.place == "institute"
            ){
                clueItems.push_back(i);
            }
        }

        if (clueItems.empty()) {
            fmt::println("you cant advance for Invisible man !!");
            this_thread::sleep_for(chrono::seconds(2));
            this->pageNumber = PageNumbers::ADVANCED_PAGE; return;
        }
        else {
            if (hero->getCurrentPlace() != "precinct"){
                fmt::println("you cant advance for Invisible man !!");
                this_thread::sleep_for(chrono::seconds(2));
                this->pageNumber = PageNumbers::ADVANCED_PAGE; return;
            }
            else {
                // pay items
                hero->deleteItem(clueItems[0].name);
                sys->addItem(clueItems[0]);
                if (!sys->allCluesFound("clue")) sys->makeFoundClues("clue");
                actions--;
                fmt::println("Fuck !!! we found one clue of ivisible man");
                this_thread::sleep_for(chrono::seconds(2));
                this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return;
            }
        }
    }
}

void Tui::specialActionPage(shared_ptr<HeroBase>& hero , int &actions){
    //page number 7
    clearScreen();
    string current = hero->getCurrentPlace();
    vector<string> neis = (sys->getGameMap())[current];
    if (hero->getHeroName() == "mayor"){
        fmt::print("Mayor has not any special action !!!");
        while (true) {
            int num;
            fmt::println("1. Back");
            fmt::println("2. Exit");
            num = getCommand("Enter a option");
            if (num == 1) { this->backButton(); return;}
            else if (num == 2) { this->pageNumber = PageNumbers::EXIT_PAGE; return; }
            else fmt::println("Invalid option entered!!");
        }
    }
    else {
        int neiNum;
        fmt::println("First enter your neighbor which u want");
        while(true){
            int num;
            for (int i {}; i < neis.size(); i++){
                fmt::println("{}. {}", i + 1 , neis[i]);
            }
            fmt::println("{}. Back" , neis.size() + 1);
            fmt::println("{}. Exit" , neis.size() + 2);
            num = getCommand("Enter a place to pickup its items");
            if (num > 0 && num <= neis.size()) { neiNum = num - 1; break; }
            else if (num == neis.size() + 1) { this->backButton(); return;}
            else if (num == neis.size() + 2) { this->pageNumber = PageNumbers::EXIT_PAGE; return; }
            else fmt::println("Invalid place entered!!");
        }
        clearScreen();
        string curr = neis[neiNum];
        auto colorToString = [](cardColor::Color c) -> string {
            if (c == cardColor::BLUE) return "blue";
            else if (c == cardColor::RED) return "red";
            else if (c == cardColor::YELLOW) return "yellow";
        };
        vector<Item> hereItems;
        for(auto loc : sys->getLocations()){
            if (loc->getPlaceName() == curr){
                for(auto item : loc->getAllItems()) hereItems.push_back(item);
            }
        }
        int firstItemsSize = hereItems.size();
        while(true){
            int itemNum;
            for(int i {}; i < hereItems.size(); i++){
                fmt::println(
                    "{}. {} {} ({})",
                    i + 1,
                    hereItems[i].name,
                    hereItems[i].name,
                    hereItems[i].power
                ); 
            }
            cout << hereItems.size() + 1 << ". Stop picking" << '\n';
            itemNum = getCommand("Enter a item number");
            if (itemNum > 0 && itemNum <= hereItems.size() + 1){
                if (itemNum == hereItems.size() + 1){
                    if (firstItemsSize != hereItems.size()) actions--;
                    this->pageNumber = PageNumbers::HERO_PHASE_PAGE;
                    return;
                }
                else {
                    hero->addItem(hereItems[itemNum - 1]);
                    for(auto loc : sys->getLocations()){
                        if (loc->getPlaceName() == current){
                            loc->deleteItem(hereItems[itemNum - 1].name);
                            break;
                        }
                    }           
                }
            }
            else {
                fmt::println("Invalid item Entered");
            }
        }
    }
}

void Tui::guidePage(shared_ptr<HeroBase>& hero , int &actions){
    // page number 2
    clearScreen();
    fmt::println("Ok , Now you can choose one villager and move it");
    int ch;
    while(true){
        fmt::println("1. Move a villager to neighbor place");
        fmt::println("2. Bring a villager from neighbor place");
        fmt::println("3. Back");
        fmt::println("4. Exit");
        ch = getCommand("Please Enter your choise");
        if (ch <= 0 || ch > 4) fmt::println("Invalid choise for guide method");
        else break;
    }
    clearScreen();
    if (ch == 1){
        string current = hero->getCurrentPlace();
        vector<string> neis = (sys->getGameMap())[current];
        vector<shared_ptr<Villager>> hereVills;

        for(auto loc : sys->getLocations()){
            if (loc->getPlaceName() == current){
                for(auto vill : loc->getAllVillagers()) hereVills.push_back(vill);
                break;
            }
        }
        int villNum, locNum;
        if (hereVills.size() == 0) {
            this->pageNumber = PageNumbers::GUIDE_PAGE;
            return; 
        }
        else {
            // choose villager
            while(true){
                for(int i {}; i < hereVills.size(); i++){
                    cout << i + 1 << ". " << hereVills[i]->getVillagerName() << '\n';
                }
                villNum = getCommand("Enter a villager number");
                if (villNum <= 0 || villNum > hereVills.size()){
                    fmt::println("invalid villager number");
                }
                else break;
            }
            // choose place 
            while (true){
                for(int i {}; i < neis.size(); i++){
                    cout << i + 1 << ". " << neis[i] << '\n';
                }
                locNum = getCommand("Enter a place Number");
                if (locNum <= 0 || locNum > neis.size()){
                    fmt::println("invalid villager number");
                }
                else break;
            }
            sys->moveVillager(
                hereVills[villNum - 1]->getVillagerName() ,
                neis[locNum - 1]
            );
            if (hereVills[villNum - 1]->getSafeZone() == neis[locNum - 1]){
                fmt::println(
                    "You reached a villager {} to its Safe zone :)\n He will give u a perk man",
                    hereVills[villNum - 1]->getVillagerName()
                );
                sys->killVillager(hereVills[villNum - 1]->getVillagerName());
                hero->addPerk(sys->getRandomPerk());
            }
            actions--;
            this->pageNumber = PageNumbers::HERO_PHASE_PAGE;
        }
    }

    else if (ch == 2){
        // choose place
        int villNum , locNum;
        string current = hero->getCurrentPlace();
        vector<string> neis = (sys->getGameMap())[current];
        fmt::println("Choose the place with u want to bring a villager from");
        while (true){
            for(int i {}; i < neis.size(); i++){
                cout << i + 1 << ". " << neis[i] << '\n';
            }
            locNum = getCommand("Enter a place Number");
            if (locNum <= 0 || locNum > neis.size()){
                fmt::println("invalid villager number");
            }
            else break;
        }
        vector<shared_ptr<Villager>> hereVills;
        for(auto loc : sys->getLocations()){
            if (loc->getPlaceName() == neis[locNum - 1]){
                for(auto vill : loc->getAllVillagers()) hereVills.push_back(vill);
                break;
            }
        }
        if (hereVills.size() == 0) { this->pageNumber = PageNumbers::GUIDE_PAGE; return;}
        while(true){
            for(int i {}; i < hereVills.size(); i++){
                cout << i + 1 << ". " << hereVills[i]->getVillagerName() << '\n';
            }
            villNum = getCommand("Enter a villager number");
            if (villNum <= 0 || villNum > hereVills.size()){
                fmt::println("invalid villager number");
            }
            else break;
        }
        sys->moveVillager(
            hereVills[villNum - 1]->getVillagerName() ,
            neis[locNum - 1]
        );
        if (hereVills[villNum - 1]->getSafeZone() == neis[locNum - 1]){
            fmt::println(
                "You reached a villager {} to its Safe zone :)\n He will give u a perk man",
                hereVills[villNum - 1]->getVillagerName()
            );
            sys->killVillager(hereVills[villNum - 1]->getVillagerName());
            hero->addPerk(sys->getRandomPerk());
        }
        actions--;
        this->pageNumber = PageNumbers::HERO_PHASE_PAGE;
    }
    else if (ch == 3) { this->backButton(); return; }
    else if (ch == 4){ this->pageNumber = PageNumbers::EXIT_PAGE; return; }
}

void Tui::pickUpPage(shared_ptr<HeroBase>& hero , int &actions){
    //page number 3
    clearScreen();
    fmt::println("You can pick up here items");
    fmt::println("you can back from this page with chosing stop picking");
    string current = hero->getCurrentPlace();
    auto colorToString = [](cardColor::Color c) -> string {
        if (c == cardColor::BLUE) return "blue";
        else if (c == cardColor::RED) return "red";
        else if (c == cardColor::YELLOW) return "yellow";
    };
    vector<Item> hereItems;
    for(auto loc : sys->getLocations()){
        if (loc->getPlaceName() == current){
            for(auto item : loc->getAllItems()) hereItems.push_back(item);
        }
    }
    int firstItemsSize = hereItems.size();
    while(true){
        int itemNum;
        for(int i {}; i < hereItems.size(); i++){
            fmt::println(
                "{}. {} {} ({})",
                i + 1,
                hereItems[i].name,
                hereItems[i].name,
                hereItems[i].power
            ); 
        }
        cout << hereItems.size() + 1 << ". Stop picking" << '\n';
        itemNum = getCommand("Enter a item number");
        if (itemNum > 0 && itemNum <= hereItems.size() + 1){
            if (itemNum == hereItems.size() + 1){
                if (firstItemsSize != hereItems.size()) actions--;
                this->pageNumber = PageNumbers::HERO_PHASE_PAGE;
                return;
            }
            else {
                hero->addItem(hereItems[itemNum - 1]);
                for(auto loc : sys->getLocations()){
                    if (loc->getPlaceName() == current){
                        loc->deleteItem(hereItems[itemNum - 1].name);
                        break;
                    }
                }           
            }
        }
        else {
            fmt::println("Invalid item Entered");
        }
    }
}

void Tui::defeatPage(shared_ptr<HeroBase>& hero , int &actions){
    // page number 6
    clearScreen();
    fmt::println("first clarify which of monsters u want to defeat");
    int ch;
    while (true){
        fmt::println("1. Dracula");
        fmt::println("2. Invisible Man");
        fmt::println("3. Back");
        fmt::println("4. Exit");
        ch = getCommand("Enter your choise to defeat");
        if(ch == 3) { this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return; }
        else if(ch == 4) { this->pageNumber = PageNumbers::EXIT_PAGE; return; }
        else if ( ch < 0 || ch > 4) {
            fmt::println("Invaid choise for defeat!!!");
        }
        else break;
    }
    if (ch == 1) {
        // they should be in same place 
        bool isInSamePlace {false};
        for(auto monst : sys->getMonsters()){
            if (monst->getMonsterName() == "dracula"){
                if (monst->getCurrentLocation() == hero->getCurrentPlace()) { isInSamePlace = true; break; }
            }
        }

        if (isInSamePlace == false){
            fmt::println("you cant kill dracula !!");
            this_thread::sleep_for(chrono::seconds(2));
            this->pageNumber = PageNumbers::DEFEAT_PAGE; return;
        }
        else {
            vector<Item> yellowItems;
            int itemsPowerSum = 0;
            for(auto i : hero->getAllItems()){
                if (i.color == cardColor::YELLOW) { 
                    yellowItems.push_back(i); 
                    itemsPowerSum += i.power;
                    if (itemsPowerSum >= 6) break; 
                }
            }
            if ( itemsPowerSum < 6 ){
                fmt::println("you cant kill dracula we haven't enogth item !!");
                this_thread::sleep_for(chrono::seconds(2));
                this->pageNumber = PageNumbers::DEFEAT_PAGE; return;
            }
            else {
                if (sys->allCluesFound("coffin") == false){
                    fmt::println("we have not found all coffins yet");
                    this_thread::sleep_for(chrono::seconds(2));
                    this->pageNumber = PageNumbers::DEFEAT_PAGE; return;
                }
                else {
                    for(auto i : yellowItems){
                        hero->deleteItem(i.name);
                        sys->addItem(i);
                    }
                    for (auto loc : sys->getLocations()){
                        if (loc->getPlaceName() == hero->getCurrentPlace()){
                            loc->deleteMonster("dracula");
                        }
                    }
                    sys->killMonster("dracula");
                    actions--;
                    fmt::println("soooo we killed dracula");
                    this_thread::sleep_for(chrono::seconds(2));
                    this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return;
                }
            }
        }
    }
    else if (ch == 2) {
        bool isInSamePlace {false};
        for(auto monst : sys->getMonsters()){
            if (monst->getMonsterName() == "invisibleMan"){
                if (monst->getCurrentLocation() == hero->getCurrentPlace()) { isInSamePlace = true; break; }
            }
        }

        if (isInSamePlace == false){
            fmt::println("you cant kill invisibleMan !!");
            this_thread::sleep_for(chrono::seconds(2));
            this->pageNumber = PageNumbers::DEFEAT_PAGE; return;
        }
        else {
            vector<Item> redItems;
            int itemsPowerSum = 0;
            for(auto i : hero->getAllItems()){
                if (i.color == cardColor::RED) { 
                    redItems.push_back(i); 
                    itemsPowerSum += i.power;
                    if (itemsPowerSum >= 9) break; 
                }
            }
            if ( itemsPowerSum < 9 ){
                fmt::println("you cant kill invisibleMan we haven't enogth item !!");
                this_thread::sleep_for(chrono::seconds(2));
                this->pageNumber = PageNumbers::DEFEAT_PAGE; return;
            }
            else {
                if (sys->allCluesFound("clue") == false){
                    fmt::println("we have not found all clues yet");
                    this_thread::sleep_for(chrono::seconds(2));
                    this->pageNumber = PageNumbers::DEFEAT_PAGE; return;
                }
                else {
                    for(auto i : redItems){
                        hero->deleteItem(i.name);
                        sys->addItem(i);
                    }
                    for (auto loc : sys->getLocations()){
                        if (loc->getPlaceName() == hero->getCurrentPlace()){
                            loc->deleteMonster("invisibleMan");
                        }
                    }
                    sys->killMonster("invisibleMan");
                    actions--;
                    fmt::println("soooo we killed invisibleMan");
                    this_thread::sleep_for(chrono::seconds(2));
                    this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return;
                }
            }
        }
    }
}

void Tui::playPerkPage(shared_ptr<HeroBase>& hero , int &actions , bool &doMonsterPhase){
    // page number 8
    clearScreen();
    unordered_set<string> uniqePerkNames;
    unordered_map<int , string> test;
    for (auto perk : hero->getAllPerks()) uniqePerkNames.insert(perk.name);
    fmt::println("First choose which perk u want to play");
    string perkName;
    while(true) {
        int i {0} , perkNum;
        for(auto perk : uniqePerkNames){
            fmt::println("{}. {}" , i + 1 , perk);
            test[i] = perk;
            i++;
        }
        fmt::println("{}. Back" , i + 1);
        fmt::println("{}. Exit" , i + 2);
        perkNum = getCommand("Enter your perk number");
        if (perkNum > 0 && perkNum <= i){
            perkName = test[i - 1];
            hero->deletePerk(perkName);
            break;
        }
        else if (perkNum == i + 1) { this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return; }
        else if (perkNum == i + 2) { this->pageNumber = PageNumbers::EXIT_PAGE; return; }
        else fmt::println("Wrong perk or option entered!!");
    }
    if (perkName == "Visit_from_the_Detective"){
        int n;
        clearScreen();
        fmt::println("you choosed Visit_from_the_Detective");
        fmt::println("enter your place to put invisible man");
        while(true) {
            for(int i {}; i < sys->getLocations().size(); i++){
                fmt::println("{}. {}" , i+1, ((sys->getLocations())[i])->getPlaceName());
            }
            fmt::println("{}. Back" , sys->getLocations().size() + 1);
            fmt::println("{}. Exit" , sys->getLocations().size() + 2);
            n = getCommand("Your place is");
            if (n > 0 && n <= sys->getLocations().size()){
                sys->moveMonster("invisibleMan" , sys->getLocations()[n - 1]->getPlaceName());
                this->pageNumber = PageNumbers::HERO_PHASE_PAGE; 
                return;
            }
            else if (n == sys->getLocations().size() + 1) { this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return; }
            else if (n == sys->getLocations().size() + 2) { this->pageNumber = PageNumbers::EXIT_PAGE; return; }
            else fmt::println("Invalid place entered!!");
        }
    }
    else if (perkName == "Break_of_Dawn") {
        doMonsterPhase = false;
        for(int i {} ; i < 2; i++){
            Item temp = sys->getRandomItem();
            sys->putItemInPlace(temp.place , temp);
        }
        this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return;
    }
    else if (perkName == "Overstock"){
        for(int i {} ; i < 2; i++){
            Item temp = sys->getRandomItem();
            sys->putItemInPlace(temp.place , temp);
        }
        this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return;
    }
    else if (perkName == "Late_into_the_Night") {
        actions += 2;
        this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return;
    }
    else if (perkName == "Repel") {
        clearScreen();
        fmt::println("Now we can move all heroes two times");
        for (auto h : sys->getHeros()){
            for (int j {}; j < 2; j++) {
                fmt::println("Move {} to ...." , h->getHeroName());
                string current = h->getCurrentPlace(); 
                vector<string> neis = (sys->getGameMap())[current];
                while (true){
                    for(int i {}; i < neis.size(); i++){
                        cout << i + 1 << ". " << neis[i] << '\n';
                    }
                    cout << neis.size() + 1 << ". Back" << '\n';
                    cout << neis.size() + 2 << ". Exit" << '\n';
                    int num = getCommand("Enter a number to choose where u want to go ");
    
                    if (num > 0 && num <= neis.size()) {
                        string x = h->getHeroName() , y = neis[num - 1];
                        sys->moveHero(x , y);
                        break;
                    }
                    else if (num == neis.size() + 1) { this->backButton(); return; }
                    else if (num == neis.size() + 2) { this->pageNumber = PageNumbers::EXIT_PAGE; return; }
                    else fmt::println("Invalid neighboer choies !!!");
                }
            }
        }
        this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return;
    }
    else if (perkName == "Hurry") {
        clearScreen();
        fmt::println("Now we can move all heroes two times");
        for (auto m : sys->getMonsters()){
            for (int j {}; j < 2; j++) {
                fmt::println("Move {} to ...." , m->getMonsterName());
                string current = m->getCurrentLocation(); 
                vector<string> neis = (sys->getGameMap())[current];
                while (true){
                    for(int i {}; i < neis.size(); i++){
                        cout << i + 1 << ". " << neis[i] << '\n';
                    }
                    cout << neis.size() + 1 << ". Back" << '\n';
                    cout << neis.size() + 2 << ". Exit" << '\n';
                    int num = getCommand("Enter a number to choose where u want to go ");
    
                    if (num > 0 && num <= neis.size()) {
                        sys->moveMonster(m->getMonsterName() , neis[num - 1]);
                        break;
                    }
                    else if (num == neis.size() + 1) { this->backButton(); return; }
                    else if (num == neis.size() + 2) { this->pageNumber = PageNumbers::EXIT_PAGE; return; }
                    else fmt::println("Invalid neighboer choies !!!");
                }
            }
        }
        this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return;
    }
}

void Tui::helpPage() {
    clearScreen();
    // fmt::println();
}

void Tui::monsterPhasePage(shared_ptr<MonsterBase>& monster , string_view cHero)
{
    clearScreen();
    fmt::println("HA HA HA its time for us to attack :)");
    fmt::println("im {} and im now doing my turn to do my job!!!" , monster->getMonsterName());
    char dice = sys->rollDice();
    fmt::println("Dice sign is {}" , dice);
    this_thread::sleep_for(chrono::seconds(1));
    int status = monster->runMonsterPhase(dice , cHero);
    status = 2;
    if (status == -1)
    {
        fmt::println("You are so lucky man :(");
        this_thread::sleep_for(chrono::seconds(2));
        pageNumber = PageNumbers::HERO_PHASE_PAGE;
    }
    else
    {
        clearScreen();
        if (status != 1)
        {
            fmt::println("shit situation is so bad your hero is hurted :)");
            fmt::println("Dont worry u can pay an item to be safe");
            int n;
            while (true)
            {
                fmt::println("Do u want to pay an item ??");
                fmt::println("1. Yes");
                fmt::println("2. No");
                n = getCommand("Choose a number");
                if (n != 1 && n != 2) fmt::println ("invalid choose!!!");
                else break;
            }
            if (n == 1)
            {
                auto colorToString = [](cardColor::Color c) -> string {
                    if (c == cardColor::BLUE) return "blue";
                    else if (c == cardColor::RED) return "red";
                    else if (c == cardColor::YELLOW) return "yellow";
                };
                for (auto h : sys->getHeros())
                {
                    if (h->getHeroName() == cHero)
                    {
                        if (h->getAllItems().empty())
                        {
                            fmt::println("You dont have any item to pay");
                            fmt::println("So we punish u becasue of ur lie !!!");
                            this_thread::sleep_for(chrono::seconds(2));
                            sys->increaseTerrorLevel();
                            sys->moveHero(cHero , "hospital");
                            pageNumber = PageNumbers::HERO_PHASE_PAGE;
                        }
                        else
                        {
                            int j;
                            while(true)
                            {   
                                for (int i {}; i < h->getAllItems().size(); i++)
                                {
                                    fmt::println(
                                        "{}. {} {} ({})",
                                        i + 1,
                                        (h->getAllItems())[i].name,
                                        colorToString((h->getAllItems())[i].color),
                                        (h->getAllItems())[i].power
                                    );
                                }
                                j = getCommand("Enter item number");
                                if (j < 0 || j > h->getAllItems().size()) fmt::println("invaid number for item");
                                else break;
                            }
                            sys->addItem(h->getAllItems()[j - 1]);
                            h->deleteItem(h->getAllItems()[j - 1].name);
                            pageNumber = PageNumbers::HERO_PHASE_PAGE;
                            break;
                        }
                    }
                }
            }
            else 
            {
                fmt::println("Ok we do respect to ur choice");
                sys->increaseTerrorLevel();
                sys->moveHero(cHero , "hospital");
                pageNumber = PageNumbers::HERO_PHASE_PAGE;
            }
        }
    }    
}

void Tui::runGame() {
    this->welcomePage();
    int round {0};
    int playerCount = playerPriority.size();
    bool doNextPhase {true};
    while (this->pageNumber != PageNumbers::EXIT_PAGE) {
        string currentHeroName = playerPriority[round % playerCount];
        shared_ptr<HeroBase> currentHero {nullptr};
        for (auto h : sys->getHeros()){
            if (h->getHeroName() == currentHeroName) { currentHero = h; break;}
        }
        int actions = currentHero->getActions();
        while(actions != 0 && pageNumber != PageNumbers::EXIT_PAGE) {
            if (this->pageNumber == PageNumbers::HERO_PHASE_PAGE) this->heroPhasePage(currentHero , actions);
            else if (this->pageNumber == PageNumbers::MOVE_PAGE) this->movePage(currentHero , actions);
            else if (this->pageNumber == PageNumbers::GUIDE_PAGE) this->guidePage(currentHero , actions);
            else if (this->pageNumber == PageNumbers::PICKUP_PAGE) this->pickUpPage(currentHero , actions);
            else if (this->pageNumber == PageNumbers::SPECIALACTION_PAGE) this->specialActionPage(currentHero , actions);
            else if (this->pageNumber == PageNumbers::ADVANCED_PAGE) this->advancedPage(currentHero , actions);
            else if (this->pageNumber == PageNumbers::DEFEAT_PAGE) this->defeatPage(currentHero , actions);
            else if (this->pageNumber == PageNumbers::PLAYPERK_PAGE) this->playPerkPage(currentHero , actions , doNextPhase);
        }
        if (this->pageNumber != PageNumbers::EXIT_PAGE && doNextPhase == true) 
        {
            shared_ptr<MonsterBase> m = (sys->getMonsters())[round % playerCount];
            this->monsterPhasePage(m , currentHeroName);
        }
        round++;
    }
    this->quitPage();
}