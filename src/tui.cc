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
#include <unordered_set>
#include <unordered_map>

using namespace std;

Tui::Tui(System *s): sys(s) {}

void Tui::backButton() { this->pageNumber = PageNumbers::HERO_PHASE_PAGE; }

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

int Tui::monsterPhasePage(shared_ptr<HeroBase>& hero){
    clearScreen();
    fmt::println("HA HA HA its time for us to attack :)");
    MonsterCard currentCard {sys->getRandomMonstCard()};

    for (auto monst : sys->getAllMonsters()){
        if (monst != nullptr) {
            monst->putItem(currentCard.itemCount);
            monst->doEvent(currentCard.name , hero);
            break;
        }
    }

    playedMonsterCard = currentCard.name;
    bool continuePhase {true};
    int isEnd { -1 }; 

    unordered_map<string , shared_ptr<MonsterBase>> strikeMap;
    for (auto st : currentCard.strikePriorities){
        if (st == "du"){
            for (auto m : sys->getAllMonsters()){
                if (m->getMonsterName() == "dracula"){
                    strikeMap["du"] = m;
                }
            }
        }
        else if (st == "inm"){
            for (auto m : sys->getAllMonsters()){
                if (m->getMonsterName() == "invisibleMan"){
                    strikeMap["inm"] = m;
                }
            }
        }
        else {
            for (auto m : sys->getAllMonsters()){
                if (m->getIsFrenzed()){
                    strikeMap["fz"] = m;
                }
            }
        }
    }

    for (auto [stPhase , m] : strikeMap){
        if ( isEnd != -1 ) break;
        if (continuePhase){
            fmt::println("Im {} which gonna do my job !!!!" , m->getMonsterName());
            m->move(currentCard.move , stPhase);
            vector<char> dices;
            for (int i{}; i < currentCard.dice; i++) dices.push_back(sys->rollDice());
            for (char dice : dices){
                isEnd = sys->isEndGame();
                // endGame situation
                if ( isEnd != -1 ) break;
                if (!continuePhase) break;

                fmt::println("Dice sign now is : {}" , dice);
                this_thread::sleep_for(chrono::seconds(2));
                if (dice == '*') continuePhase = false;           
                int status = m->attack(dice , m , hero);

                // no attack or power ran !!
                if (status == -1 || status == 4) {
                    fmt::println("You are so lucky man :(");
                    string text = (status == -1) ? "Dice was empty so no attack" : "Power of " + m->getMonsterName() + " ran!!";
                    fmt::println(text);
                    this_thread::sleep_for(chrono::seconds(2));
                    continuePhase = true;
                    pageNumber = PageNumbers::HERO_PHASE_PAGE;
                }
                // attack to current hero!!
                else if (status == 2 || status == 3) {
                    // find which of heros has been attacked !!!!
                    shared_ptr<HeroBase> attacedHero;
                    if (status == 2) attacedHero = hero;
                    else {
                        for (auto h : sys->getAllHeros()){
                            if (h->getHeroName() != hero->getHeroName()) attacedHero = h;
                        }
                    }
                    clearScreen();
                    fmt::println("situation is so bad your hero is hurted :)");
                    fmt::println("Dont worry u can pay an item to be safe");
                    int n;
                    while (true) {
                        fmt::println("Do u want to pay an item ??");
                        fmt::println("1. Yes");
                        fmt::println("2. No");
                        n = getCommand("Choose a number");
                        if (n != 1 && n != 2) fmt::println ("invalid choose!!!");
                        else break;
                    }
                    if (n == 1) {
                        auto colorToString = [](Color c) -> string {
                            if (c == Color::BLUE) return "blue";
                            else if (c == Color::RED) return "red";
                            else if (c == Color::YELLOW) return "yellow";
                            return "";
                        };
                        
                        if (attacedHero->getHeroItems().empty()) {
                            fmt::println("You dont have any item to pay");
                            fmt::println("So we punish u becasue of ur lie !!!");
                            this_thread::sleep_for(chrono::seconds(2));
                            sys->increaseTerrorLevel();
                            for(auto loc : sys->getAllLocations()){
                                if (loc->getName() == "hospital"){
                                    sys->moveHero(attacedHero , loc);
                                    continuePhase = false;
                                    break;
                                }
                            }
                            pageNumber = PageNumbers::HERO_PHASE_PAGE;
                        }
                        else {
                            int j;
                            while(true) {   
                                for (int i {}; i < attacedHero->getHeroItems().size(); i++) {
                                    fmt::println(
                                        "{}. {} {} ({}) -> {}" ,
                                        i + 1,
                                        (attacedHero->getHeroItems())[i].name,
                                        colorToString((attacedHero->getHeroItems())[i].color),
                                        (attacedHero->getHeroItems())[i].power,
                                        (attacedHero->getHeroItems())[i].place
                                    );
                                }
                                j = getCommand("Enter item number");
                                if (j < 1 || j > attacedHero->getHeroItems().size()) fmt::println("invaid number for item");
                                else break;
                            }
                            sys->addItem(attacedHero->getHeroItems()[j - 1]);
                            attacedHero->deleteItem(attacedHero->getHeroItems()[j - 1].name);
                            continuePhase = true;
                            pageNumber = PageNumbers::HERO_PHASE_PAGE;
                            break;
                        }  
                    }
                    else {
                        fmt::println("Ok we do respect to ur choice");
                        sys->increaseTerrorLevel();
                        for(auto loc : sys->getAllLocations()){
                            if (loc->getName() == "hospital"){
                                sys->moveHero(attacedHero , loc);
                                continuePhase = false;
                                break;
                            }
                        }
                        pageNumber = PageNumbers::HERO_PHASE_PAGE;
                    }
                }
                // attaced to a villager
                else {
                    fmt::println("We lost one villager !!!!!");
                    sys->increaseTerrorLevel();
                    continuePhase = false;
                    this_thread::sleep_for(chrono::seconds(2));
                    pageNumber = PageNumbers::HERO_PHASE_PAGE;
                }
                clearScreen();
            }
        }
    }
    return isEnd;
} 

void Tui::monstersInfo() {
    cout << "╔═══════════════════════╗\n";
    cout << "║     Monster Status    ║\n";
    cout << "╚═══════════════════════╝\n";

    tabulate::Table table;
    table.add_row({"Monster", "Location", "Clue Items" , "Remaining Clue"});
    for (const auto& monster : sys->getAllMonsters()) {
        if (monster != nullptr){
            string clueText;
            string Clue;
            if (monster->getMonsterName() == "dracula"){
                clueText = to_string(sys->foundCluesCount("coffin")) + " coffins smashed";
                for(auto coffin:sys->getCoffins())
                Clue += coffin + ' ';    
            }
            else{
                clueText = to_string(sys->foundCluesCount("clue")) + " evidence found";
                for(auto evi:sys ->getEvidence())
                Clue += evi + ' ';
            }
            table.add_row({
                monster->getMonsterName(), 
                monster->getCurrentLocation()->getName(), 
                clueText,
                Clue
            });
        }
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
    cout << '\n';
    cout << "    ├── Last Played Perk: "; cout << hero->getLastPlayedName();
    for (auto item : hero->getHeroItems()) {
        cout << '\n';
        tabulate::Table t;
        string text = item.name + "(" + to_string(item.power) + ")" + " -> " + item.place;
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

void Tui::VillagersInfo() {
    cout << "╔═════════════════════════╗\n";
    cout << "║     Villagers Status    ║\n";
    cout << "╚═════════════════════════╝\n";
    tabulate::Table vill;
    vill.add_row({"Villagers", "Location" , "Safe Zone"});

        for(auto place : sys -> getAllLocations()) {
            if(!place -> getVillagers().empty()){
            for(auto villager : place ->getVillagers())
                vill.add_row({villager->getName() , villager->getVillagerLoc()->getName() , villager->getSafeZone()->getName()});
            }
        }
    vill.format().border_top("═").border_bottom("═").border_left("║").border_right("║").corner("╬");
    vill[0].format().font_style({tabulate::FontStyle::bold}).font_color(tabulate::Color::red);
    cout << vill << "\n\n";
}

void Tui::quitPage() const {
    // page number = 9
    // clearScreen();
    fmt::println("I hope u enjoy this game :(: ");
}

void Tui::displayActions() const {
    cout << "🧰 Hero Actions:\n";
    cout << "[1] Move  ";
    cout << "[2] Guide Local  ";
    cout << "[3] Pick Up Item  ";
    cout << "[4] Advanced Action  ";
    cout << "[5] Defeat Action\n";
    cout << "[6] Special Action  ";
    cout << "[7] Play perk  ";
    cout << "[8] Help Page  ";
    cout << "[9] Exit Game\n\n";
}

void Tui::welcomePage() {
    clearScreen();
    fmt::println("Be careful there are lot of monsters overthere !!!");
    fmt::println("But monsters hate Garlics");
    fmt::println("You can enter q instead of each input to quit the game !!!");
    int player1Days = getCommand("How many days has it been since you last ate garlic player1 (if equalize player1 will win)");
    if (player1Days == -1) { this->pageNumber = PageNumbers::EXIT_PAGE; return; }
    int player2Days = getCommand("How many days has it been since you last ate garlic player2 (if equalize player1 will win)");
    if (player2Days == -1) { this->pageNumber = PageNumbers::EXIT_PAGE; return; }
    int playerNumber = (player1Days <= player2Days) ? 1 : 2;
    while(true) {
        fmt::println("Ok , player {} . Now choose your character: " , playerNumber);
        fmt::println("1. Archaeologist");
        fmt::println("2. Mayor");
        int num = getCommand();
        if (num == 1) {
            this->playerPriority.push_back("archaeologist");
            this->playerPriority.push_back("mayor");
            break;
        }
        else if (num == 2) {
            this->playerPriority.push_back("mayor");
            this->playerPriority.push_back("archaeologist");
            break;
        }
        else fmt::println("Invalid argument entered !!!");
    }
    fmt::println("ok game is ready to go wait for 2 seconds .....");
    this_thread::sleep_for(chrono::seconds(2));
    clearScreen();
}

void Tui::terrorLevel(int level) {
    string bar = string(level, '#') + string(5 - level, '-');
    cout << "🧟 Terror Level: " << bar << " (" << level << " / 5)\n\n";
}

void Tui::heroPhasePage(shared_ptr<HeroBase>& hero , int actions){
    // page Number 0
    clearScreen();
    this->header();
    this->heroInfo(hero);
    fmt::println("Remaining actions : {}" , actions);
    this->showNeighborsInfo(hero);
    this->monstersInfo();
    this -> VillagersInfo();
    this->terrorLevel(sys->getTerrorLevel());
    fmt::println("The last MonsterCard that has been played is -> {}\n",playedMonsterCard);   
    this->displayActions();
    int page = getCommand("Enter a action number ");
    while (page < 1 || page > 9) {
        fmt::println("invalid page entered !!!");
        page = getCommand("Enter a action number ");
    }
    this->pageNumber = page;
}

void Tui::movePage(shared_ptr<HeroBase>& hero , int &actions) {
    // page Number 1
    clearScreen();
    string current = hero->getCurrentPlace()->getName(); 
    vector<shared_ptr<Place>> neis = hero->getCurrentPlace()->getNeighbors();
    vector<shared_ptr<Villager>> hereVills = hero->getCurrentPlace()->getVillagers();

    fmt::println("Ok, At first you can move yourself to an other place !!!");
    while (true){
        for(int i {}; i < neis.size(); i++){
            cout << i + 1 << ". " << neis[i]->getName() << '\n';
        }
        cout << neis.size() + 1 << ". Back" << '\n';
        cout << neis.size() + 2 << ". Exit" << '\n';
        int num = getCommand("Enter a number to choose where u want to go ");

        if (num > 0 && num <= neis.size()) {
            sys->moveHero(hero , neis[num - 1]);
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
        hero->getCurrentPlace()->getName(),
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
                cout << i + 1 << ". " << hereVills[i]->getName() << '\n';
            }
            cout << hereVills.size() + 1 << ". Stop moving" << '\n';
            villNum = getCommand("Enter a number ");
            if (villNum > 0 && villNum <= hereVills.size()){
                sys->moveVillager(hereVills[villNum - 1] , hero->getCurrentPlace());
                if (hereVills[villNum - 1]->getSafeZone()->getName() == hero->getCurrentPlace()->getName()){
                    fmt::println(
                        "You reached a villager {} to its Safe zone :)\n He will give u a perk man",
                        hereVills[villNum - 1]->getName()
                    );
                    // TODO: Add delete villager from system here
                    sys->killVillager(hereVills[villNum - 1]);
                    hero->addPerkCard(sys->getRandomPerk());
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

void Tui::guidePage(std::shared_ptr<HeroBase>& hero ,int &actions){
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
        if (ch < 1 || ch > 4) fmt::println("Invalid choise for guide method");
        else
        break;
    }
    clearScreen();
    if (ch == 1) {
        shared_ptr<Place> current = hero->getCurrentPlace();
        vector<shared_ptr<Place>> neis = hero->getCurrentPlace()->getNeighbors();
        vector<shared_ptr<Villager>> hereVills = current->getVillagers();
        int villNum, locNum;
        if (hereVills.size() == 0) {
            this->pageNumber = PageNumbers::GUIDE_PAGE;
            return; 
        }
        else {
            // choose villager
            while(true) {
                for(int i {}; i < hereVills.size(); i++) {
                    cout << i + 1 << ". " << hereVills[i]->getName() << '\n';
                }
                villNum = getCommand("Enter a villager number");
                if (villNum < 1 || villNum > hereVills.size()){
                    fmt::println("invalid villager number");
                }
                else break;
            }
            // choose place 
            while (true) {
                for(int i {}; i < neis.size(); i++) {
                    cout << i + 1 << ". " << neis[i]->getName() << '\n';
                }
                locNum = getCommand("Enter a place Number");
                if (locNum < 1 || locNum > neis.size()){
                    fmt::println("invalid villager number");
                }
                else break;
            }

            sys->moveVillager(hereVills[villNum - 1] , neis[locNum - 1]);

            if (hereVills[villNum - 1]->getSafeZone() == neis[locNum - 1]){
                fmt::println(
                    "You reached a villager {} to its Safe zone :)\n He will give u a perk man",
                    hereVills[villNum - 1]->getName()
                );
                sys->killVillager(hereVills[villNum - 1]);
                hero->addPerkCard(sys->getRandomPerk());
            }
            actions--;
            this->pageNumber = PageNumbers::HERO_PHASE_PAGE;
        }
    }

    else if (ch == 2){
        // choose place
        int villNum , locNum;
        shared_ptr<Place> current = hero->getCurrentPlace();
        vector<shared_ptr<Place>> neis = hero->getCurrentPlace()->getNeighbors();
        fmt::println("Choose the place with u want to bring a villager from");
        while (true){
            for(int i {}; i < neis.size(); i++){
                cout << i + 1 << ". " << neis[i]->getName() << '\n';
            }
            locNum = getCommand("Enter a place Number");
            if (locNum <= 0 || locNum > neis.size()){
                fmt::println("invalid villager number");
            }
            else break;
        }
        vector<shared_ptr<Villager>> hereVills = neis[locNum - 1]->getVillagers();
        if (hereVills.size() == 0){
            this->pageNumber = PageNumbers::GUIDE_PAGE; return;
        }
        while(true){
            for(int i {}; i < hereVills.size(); i++){
                cout << i + 1 << ". " << hereVills[i]->getName() << '\n';
            }
            villNum = getCommand("Enter a villager number");
            if (villNum <= 0 || villNum > hereVills.size()){
                fmt::println("invalid villager number");
            }
            else break;
        }

        sys->moveVillager(hereVills[villNum - 1] , current);
        
        if (hereVills[villNum - 1]->getSafeZone() == current){
            fmt::println(
                "You reached a villager {} to its Safe zone :)\n He will give u a perk man",
                hereVills[villNum - 1]->getName()
            );
            sys->killVillager(hereVills[villNum - 1]);
            hero->addPerkCard(sys->getRandomPerk());
        }
        actions--;
        this->pageNumber = PageNumbers::HERO_PHASE_PAGE;
    }
    else if (ch == 3) { this->backButton(); return; }
    else if (ch == 4){ this->pageNumber = PageNumbers::EXIT_PAGE; return; }
}

void Tui::pickUpPage(std::shared_ptr<HeroBase>& hero ,int &actions){
    //page number 3
    clearScreen();
    fmt::println("You can pick up here items");
    fmt::println("you can back from this page with chosing stop picking");
    shared_ptr<Place> current = hero->getCurrentPlace();
    auto colorToString = [](Color c) -> string {
        if (c == Color::BLUE) return "blue";
        else if (c == Color::RED) return "red";
        else if (c == Color::YELLOW) return "yellow";
        return "";
    };

    vector<Item> hereItems = current->getItems();

    int firstItemsSize = hereItems.size();
    while(true){
        int itemNum;
        for(int i {}; i < hereItems.size(); i++){
            fmt::println("{}. {} {} ({})",
                i + 1,
                hereItems[i].name,
                colorToString(hereItems[i].color),
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
                hero->addHeroItems(hereItems[itemNum - 1]);
                current->removeItem(hereItems[itemNum - 1]);
                hereItems.erase(hereItems.begin() + (itemNum - 1));
            }
        }
        else {
            fmt::println("Invalid item Entered");
        }
    }
}

void Tui::advancedPage(shared_ptr<HeroBase>& hero , int &actions){
    // page number 4
    clearScreen();
    fmt::println("first clarify which of monsters u want to advance");
    int ch;
    while (true) {
        fmt::println("1. Dracula");
        fmt::println("2. Invisible Man");
        fmt::println("3. Back");
        fmt::println("4. Exit");
        ch = getCommand("Enter your choise to advance");
        if(ch == 3) { this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return; }
        else if(ch == 4) { this->pageNumber = PageNumbers::EXIT_PAGE; return; }
        else if ( ch < 1 || ch > 4) {
            fmt::println("Invaid choise for advance!!!");
        }
        else break;
    }
    if (ch == 1){
        for (auto monst : sys->getAllMonsters()){
            if (
                monst->getMonsterName() == "dracula" &&
                monst == nullptr
            ){
                clearScreen();
                fmt::println("Man dracula is dead how many times u want to kill him ??");
                fmt::println("Im gonna redirect u to advance page to choose another option");
                this_thread::sleep_for(chrono::seconds(2));
                this->pageNumber = PageNumbers::ADVANCED_PAGE; return;
            }
        }
        bool isCoffin {true};

        for(auto coffin : sys ->getCoffins())
            if (hero->getCurrentPlace()->getName() == coffin) isCoffin = false;
            if(isCoffin){
                fmt::println("you cant advance for dracula . there is no coffin here!!");
                this_thread::sleep_for(chrono::seconds(2));
                this->pageNumber = PageNumbers::ADVANCED_PAGE; return;
            }
            else {
                vector<Item> redItems;
                int itemsPowerSum = 0;
                for(auto i : hero->getHeroItems()) {
                    if (i.color == Color::RED) { 
                        redItems.push_back(i); 
                        itemsPowerSum += i.power;  
                    }
                }
                if ( itemsPowerSum < 6 ) {
                    fmt::println("not enough red item to advance Dracula !!");
                    this_thread::sleep_for(chrono::seconds(2));
                    this->pageNumber = PageNumbers::ADVANCED_PAGE; return;
                }
                else {
                    cout << "choose some red item with a valuation more than 6\n";

                    int chosenItemsPower = 0 ;
                    vector<Item> usedItem;
                    while(chosenItemsPower < 6)
                    {
                        for (int i = 0; i < redItems.size(); i++)
                        {
                            std::cout << i + 1 << " - " << redItems[i] << '\n';
                        }
                        int choice;
                        choice = getCommand();
                        if (choice < 1 || choice > redItems.size())
                        {
                            std::cout << "invalid choice try again\n";
                            continue;
                        }

                        Item selectedItem = redItems[choice - 1];
                        hero->deleteItem(selectedItem.name);
                        sys->addItem(selectedItem);
                        chosenItemsPower += selectedItem.power;
                        redItems.erase(redItems.begin() + (choice - 1));
                    }
                    sys->destroyClue("coffin",hero->getCurrentPlace()->getName());
                    actions--;
                    std::cout << "the coffin destroyed successfully in -> " << hero->getCurrentPlace()->getName() <<std::endl;             
                    this_thread::sleep_for(chrono::seconds(2));
                    this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return;
                }
            }
        }
    else if (ch == 2) {
        for (auto monst : sys->getAllMonsters()){
            if (
                monst->getMonsterName() == "invisible" &&
                monst == nullptr
            ){
                clearScreen();
                fmt::println("Man invisible is dead how many times u want to kill him ??");
                fmt::println("Im gonna redirect u to advance page to choose another option");
                this_thread::sleep_for(chrono::seconds(2));
                this->pageNumber = PageNumbers::ADVANCED_PAGE; return;
            }
        }
        bool flag = true;

        if(hero->getCurrentPlace()->getName() == "precinct")
        {
            flag=false;
            cout << "your providing evidence for invisible man\n";
            bool check{true};
            vector<Item> validItems;
            for(auto& evi:sys->getEvidence())
            {
                for(auto& item:hero->getHeroItems())
                {
                    if(item.place == evi)
                    {
                        check=false;
                        validItems.push_back(item);
                    }
                }
            }

            if(check)
            {
                cout << "you dont have a item that has come from barn, inn , laboraory, institute or mansion\n";
                this_thread::sleep_for(chrono::seconds(2));
                this->pageNumber = PageNumbers::ADVANCED_PAGE; return;
            }

            while(true)//if choice was invalid it didnt get out of the advance it will get another value
            {
                cout << "the Items that you can choose as evidence\n";
                for(int i = 0;i < validItems.size();i++)
                {
                    cout << i+1 << " - " << validItems[i] << '\n';
                }

                int choice;
                choice = getCommand();
                
                if (choice < 1 || choice > validItems.size())
                {
                    std::cout << "invalid choice try again\n";
                    continue;
                }

                    Item selectedItem = validItems[choice - 1];

                    hero->deleteItem(selectedItem.name);

                    sys->addItem(selectedItem);

                    cout << "you successfully added " << selectedItem  << "\nto invisble mans board\n";
                    sys->destroyClue("evidence",selectedItem.place);
                    actions--;
                    this_thread::sleep_for(chrono::seconds(3));
                    this->pageNumber = PageNumbers::ADVANCED_PAGE; return;
            }
        }
        else{
            fmt::println("you must be in precinct to add some item on invisible mans board");
            this_thread::sleep_for(chrono::seconds(2));
            this->pageNumber = PageNumbers::ADVANCED_PAGE; return;
        }
    }
}

void Tui::specialActionPage(shared_ptr<HeroBase>& hero , int &actions){
    //page number 7
    clearScreen();
    shared_ptr<Place> current = hero->getCurrentPlace();
    vector<shared_ptr<Place>> neis = hero->getCurrentPlace()->getNeighbors();
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
        while(true) {
            int num;
            for (int i {}; i < neis.size(); i++){
                fmt::println("{}. {}", i + 1 , neis[i]->getName());
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
        shared_ptr<Place> curr = neis[neiNum];
        auto colorToString = [](Color c) -> string {
            if (c == Color::BLUE) return "blue";
            else if (c == Color::RED) return "red";
            else if (c == Color::YELLOW) return "yellow";
            return "";
        };
        vector<Item> hereItems = curr->getItems();
        int firstItemsSize = hereItems.size();
        while(true){
            int itemNum;
            for(int i {}; i < hereItems.size(); i++){
                fmt::println(
                    "{}. {} {} ({})",
                    i + 1,
                    hereItems[i].name,
                    colorToString(hereItems[i].color),
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
                    hero->addHeroItems(hereItems[itemNum - 1]);
                    curr->removeItem(hereItems[itemNum - 1]);
                    hereItems.erase(hereItems.begin() + (itemNum - 1)); 
                }
            }
            else {
                fmt::println("Invalid item Entered");
            }
        }
    }
}

void Tui::playPerkPage(shared_ptr<HeroBase>& hero , int &actions , bool &doMonsterPhase) {
    // page number 8
    clearScreen();
    unordered_set<string> uniqePerkNames;
    unordered_map<int , string> test;

    for (auto perk : hero->getHeroPerks()) uniqePerkNames.insert(perk.name);
    fmt::println("First choose which perk u want to play");
    string perkName;
    while(true) {
        int i {0} , perkNum;
        for(auto perk : uniqePerkNames) {
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
    hero->setLastPlayed(perkName);
    if (perkName == "Visit_from_the_Detective"){
        int n;
        clearScreen();
        fmt::println("you choosed Visit_from_the_Detective");
        fmt::println("enter your place to put invisible man");
        while(true)  {
            for(int i {}; i < sys->getAllLocations().size(); i++){
                fmt::println("{}. {}" , i+1, ((sys->getAllLocations())[i])->getName());
            }
            fmt::println("{}. Back" , sys->getAllLocations().size() + 1);
            fmt::println("{}. Exit" , sys->getAllLocations().size() + 2);
            n = getCommand("Your place is");
            if (n > 0 && n <= sys->getAllLocations().size()){
                for (auto monst : sys->getAllMonsters()){
                    if (monst->getMonsterName() == "invisibleMan"){
                        monst->setCurrentLocation(sys->getAllLocations()[n - 1]);
                        break;
                    }
                }
                this->pageNumber = PageNumbers::HERO_PHASE_PAGE; 
                return;
            }
            else if (n == sys->getAllLocations().size() + 1) {
                this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return; 
            }
            else if (n == sys->getAllLocations().size() + 2) { 
                this->pageNumber = PageNumbers::EXIT_PAGE; return;
            }
            else fmt::println("Invalid place entered!!");
        }
    }
    else if (perkName == "Break_of_Dawn") {
        doMonsterPhase = false;
        for(int i {} ; i < 2; i++){
            Item temp = sys->getRandomItem();
            hero->getCurrentPlace()->addItem(temp);
        }
        this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return;
    }
    else if (perkName == "Overstock") {
        for (auto her : sys->getAllHeros()){
            Item temp = sys->getRandomItem();
            sys->putItemInPlace(temp.place , temp);
        }
    }
    else if (perkName == "Late_into_the_Night") {
        actions += 2;
        this->pageNumber = PageNumbers::HERO_PHASE_PAGE; 
        return;
    }
    else if (perkName == "Repel") {
        clearScreen();
        fmt::println("Now we can move all monsters two times");
        for (auto m : sys->getAllMonsters()){
            for (int j {}; j < 2; j++) {
                fmt::println("Move {} to ...." , m->getMonsterName());
                vector<shared_ptr<Place>> neis = m->getCurrentLocation()->getNeighbors();
                while (true) {
                    for(int i {}; i < neis.size(); i++){
                        cout << i + 1 << ". " << neis[i]->getName() << '\n';
                    }
                    cout << neis.size() + 1 << ". Back" << '\n';
                    cout << neis.size() + 2 << ". Exit" << '\n';
                    int num = getCommand("Enter a number to choose where u want to go ");
                    if (num > 0 && num <= neis.size()) {
                        sys->moveMonster(m , neis[num - 1]);
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
        for (auto h : sys->getAllHeros()){
            for (int j {}; j < 2; j++) {
                fmt::println("Move {} to ...." , h->getHeroName());
                vector<shared_ptr<Place>> neis = h->getCurrentPlace()->getNeighbors();
                while (true) {
                    for(int i {}; i < neis.size(); i++){
                        cout << i + 1 << ". " << neis[i]->getName() << '\n';
                    }
                    cout << neis.size() + 1 << ". Back" << '\n';
                    cout << neis.size() + 2 << ". Exit" << '\n';
                    int num = getCommand("Enter a number to choose where u want to go ");
    
                    if (num > 0 && num <= neis.size()) {
                        sys->moveHero(h , neis[num - 1]);
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

void Tui::defeatPage(shared_ptr<HeroBase>& hero , int &actions){
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
        else if ( ch < 1 || ch > 4) {
            fmt::println("Invaid choise for defeat!!!");
        }
        else break;
    }
    if (ch == 1) {
        for (auto monst : sys->getAllMonsters()){
            if (
                monst->getMonsterName() == "dracula" &&
                monst == nullptr
            ){
                clearScreen();
                fmt::println("Man dracula is dead how many times u want to kill him ??");
                fmt::println("Im gonna redirect u to defeat page to choose another option");
                this_thread::sleep_for(chrono::seconds(2));
                this->pageNumber = PageNumbers::DEFEAT_PAGE; return;
            }
        }

        // they should be in same place 
        bool isInSamePlace {false};
        for(auto monst : sys->getAllMonsters()){
            if (monst->getMonsterName() == "dracula"){
                if (monst->getCurrentLocation() == hero->getCurrentPlace()) { isInSamePlace = true; break; }
            }
        }

        if (isInSamePlace == false){
            fmt::println("your not in the same place with Dracula !!");
            this_thread::sleep_for(chrono::seconds(2));
            this->pageNumber = PageNumbers::DEFEAT_PAGE; return;
        }

        else {
            vector<Item> yellowItems;
            int itemsPowerSum = 0;
            for(auto i : hero->getHeroItems()){
                if (i.color == Color::YELLOW) { 
                    yellowItems.push_back(i); 
                    itemsPowerSum += i.power; 
                }
            }
            if ( itemsPowerSum < 6 ){
                fmt::println("you cant kill dracula we haven't enogth yellow item !!");
                this_thread::sleep_for(chrono::seconds(2));
                this->pageNumber = PageNumbers::DEFEAT_PAGE; return;
            }
            else {
                if (sys->foundCluesCount("coffin") != 4){
                    fmt::println("we have not smashed all coffins yet");
                    this_thread::sleep_for(chrono::seconds(2));
                    this->pageNumber = PageNumbers::DEFEAT_PAGE; return;
                }
                else {
                    cout << "choose some yellow items with a valuation more than 6\n";

                        int sumPower = 0;
                    while (sumPower < 6)
                    {
                        for (int i = 0; i < yellowItems.size(); ++i)
                        {
                            std::cout << i+1 << " - " << yellowItems[i].name << " Power->" << yellowItems[i].power << '\n';
                        }
                        int choose;
                        choose = getCommand();
                        if (choose < 1 || choose > yellowItems.size())
                        {
                            cout << "invalid choice try again\n";
                            continue;
                        }
                        Item selected = yellowItems[choose - 1];
                        hero->deleteItem(selected.name);
                        sys->addItem(selected);
                        sumPower += selected.power;
                        yellowItems.erase(yellowItems.begin() + (choose - 1));
                    }

                
                    for (int i {}; i < sys->getAllMonsters().size(); i++){
                            if (sys->getAllMonsters()[i]->getMonsterName() == "dracula"){
                                sys->killMonster(sys->getAllMonsters()[i]);
                                break;
                            }
                        }

                    actions--;
                    fmt::println("soooo we killed dracula");
                    this_thread::sleep_for(chrono::seconds(3));
                    this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return;
                        
                    } 
                }
            }
        }
    else if (ch == 2) {
        for (auto monst : sys->getAllMonsters()){
            if (
                monst->getMonsterName() == "invisibleMan" &&
                monst == nullptr
            ){
                clearScreen();
                fmt::println("Man invisibleMan is dead how many times u want to kill him ??");
                fmt::println("Im gonna redirect u to defeat page to choose another option");
                this_thread::sleep_for(chrono::seconds(2));
                this->pageNumber = PageNumbers::DEFEAT_PAGE; return;
            }
        }

        bool isInSamePlace {false};
        for(auto monst : sys->getAllMonsters()){
            if (monst->getMonsterName() == "invisibleMan"){
                if (monst->getCurrentLocation() == hero->getCurrentPlace()) { isInSamePlace = true; break; }
            }
        }

        if (isInSamePlace == false){
            fmt::println("you'r not in the same place with invisibleMan !!");
            this_thread::sleep_for(chrono::seconds(2));
            this->pageNumber = PageNumbers::DEFEAT_PAGE; return;
        }
        else {
            vector<Item> redItems;
            int itemsPowerSum = 0;
            for(auto i : hero->getHeroItems()){
                if (i.color == Color::RED) { 
                    redItems.push_back(i); 
                    itemsPowerSum += i.power;
                }
            }
            if ( itemsPowerSum < 9 ){
                fmt::println("you cant kill invisibleMan we haven't enougth red item !!");
                this_thread::sleep_for(chrono::seconds(2));
                this->pageNumber = PageNumbers::DEFEAT_PAGE; return;
            }
            else {
                if (sys->foundCluesCount("evidence") != 5){
                    fmt::println("we have not found all evidences yet");
                    this_thread::sleep_for(chrono::seconds(2));
                    this->pageNumber = PageNumbers::DEFEAT_PAGE; return;
                }
                else {
                    cout << "choose some red items with a valuation more than 6\n";

                        int sumPower = 0;
                    while (sumPower < 6)
                    {
                        for (int i = 0; i < redItems.size(); ++i)
                        {
                            std::cout << i+1 << " - " << redItems[i].name << " Power->" << redItems[i].power << '\n';
                        }
                        int choose;
                        choose = getCommand();
                        if (choose < 1 || choose > redItems.size())
                        {
                            cout << "invalid choice try again\n";
                            continue;
                        }
                        Item selected = redItems[choose - 1];
                        hero->deleteItem(selected.name);
                        sumPower += selected.power;
                        redItems.erase(redItems.begin() + (choose - 1));
                    }

                    
                    // kill monster
                    for (int i {}; i < sys->getAllMonsters().size(); i++){
                        if (sys->getAllMonsters()[i]->getMonsterName() == "invisibleMan"){
                            sys->killMonster(sys->getAllMonsters()[i]);
                            break;
                        }
                    }

                    actions--;
                    fmt::println("soooo we killed invisibleMan");
                    this_thread::sleep_for(chrono::seconds(2));
                    this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return;
                }
            }
        }
    }
}

void Tui::helpPage() {
    clearScreen();
    int n;
    while (true) {
        fmt::println("first choose whith page u want to see its manual ??");
        fmt::println("1. Move page");
        fmt::println("2. Guide page");
        fmt::println("3. Pick up page");
        fmt::println("4. Advance page");
        fmt::println("5. Defeat Page");
        fmt::println("6. Special action page");
        fmt::println("7. Play perk page");
        fmt::println("8. Back");
        fmt::println("9. Exit");
        fmt::println("10. real Back");
        fmt::println("11. real Exit");
        n = getCommand("Enter ur choice");
        if(n == 10) {
            this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return;
        }
        if(n == 11) {
            this->pageNumber = PageNumbers::EXIT_PAGE; return;
        }

        if (n < 0 || n > 9) fmt::println("wrong input !!!");
        else break;
    }
    clearScreen();
    switch (n)
    {
    case 1:
        cout << "With Move action you can move to a neighbor location and if any villager was on your location the program will ask you if you want to move it as well with yourself or not\n";
        break;
    case 2:
        cout << "With Guide action if a villager is in your neighborhood you can bring it to your location\n";
        cout << "and if its currently in your location you can move it to a neighbor location\n";
        break;
    case 3:
        cout << "You can simply pick up a item from your current location and add it to your item bag\n";
        break;
    case 4:
        cout << "For defeating monsters as you may know you need to break dracula's coffins and collect evidence for invisible man you must do this works with Advanced action\n";
        cout << "Use Advanced to break coffins in (crypt,cave,graveyard,dungeon)\n";
        cout << "Pickup evidence(items) in(inn,barn,mansion,laboratory,institute) with pickup and collect them in precinct with advanced action to defeat invisible man\n";
        break;
    case 5:
        cout << "Defeat a monster with this action when you breaked all the coffins or collected all the evidence of invisible mans persent\n";
        break;
    case 6:
        cout << "With speciall action you can pick an item of neighbor locations\n";
        break;
    case 7:
        cout << "You can use one of perk cards that you already have in your bag\n";
        break;
    case 8:
        cout << "When you do back you will go back to the previous page\n";
        break;
    case 9:
        cout << "It doesnt matter where you are the game will immediately ends\n";
        break;
    }
    while (true)
    {
        cout << "1-Back\n2-End\n";
        int choose;
        choose = getCommand();
        if(choose == 1)
        return;
        else if(choose == 2){
        this->pageNumber = PageNumbers::EXIT_PAGE; return;
        }
        else
        cout << "Invalid choice try again\n";
    }
    
}

void Tui::runGame() {
    this->welcomePage();
    int round {0};
    int playerCount = playerPriority.size();
    int isEnd {-1};       
    while (this->pageNumber != PageNumbers::EXIT_PAGE && isEnd == -1) {
        bool doNextPhase {true};
        string currentHeroName = playerPriority[round % playerCount];
        shared_ptr<HeroBase> currentHero {nullptr};
        for (auto h : sys->getAllHeros()){
            if (h->getHeroName() == currentHeroName) { currentHero = h; break;}
        }
        int actions = currentHero->getActionCount();
        while(actions != 0 && pageNumber != PageNumbers::EXIT_PAGE && isEnd == -1) {
            isEnd = sys->isEndGame();
            if (isEnd != -1) break;
            else if (this->pageNumber == PageNumbers::HERO_PHASE_PAGE) this->heroPhasePage(currentHero , actions);
            else if (this->pageNumber == PageNumbers::MOVE_PAGE) this->movePage(currentHero , actions);
            else if (this->pageNumber == PageNumbers::GUIDE_PAGE) this->guidePage(currentHero , actions);
            else if (this->pageNumber == PageNumbers::PICKUP_PAGE) this->pickUpPage(currentHero , actions);
            else if (this->pageNumber == PageNumbers::SPECIALACTION_PAGE) this->specialActionPage(currentHero , actions);
            else if (this->pageNumber == PageNumbers::ADVANCED_PAGE) this->advancedPage(currentHero , actions);
            else if (this->pageNumber == PageNumbers::DEFEAT_PAGE) this->defeatPage(currentHero , actions);
            else if (this->pageNumber == PageNumbers::PLAYPERK_PAGE) this->playPerkPage(currentHero , actions , doNextPhase);
            else if (this->pageNumber == PageNumbers::HELP_PAGE) this->helpPage();
        }
        isEnd = sys->isEndGame();
        if (this->pageNumber != PageNumbers::EXIT_PAGE && doNextPhase == true && isEnd == -1) {
            this->monsterPhasePage(currentHero);
        }
        round++;
    }
    if (isEnd != -1) {
        if (isEnd == 1 || isEnd == 3) fmt::println("Monsters win !!!!");
        else fmt::println("villagers and heros win !!!!");
    }
    this->quitPage();
}
