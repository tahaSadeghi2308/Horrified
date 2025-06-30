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
#include <set>

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

void Tui::guidePage(std::shared_ptr<HeroBase>& hero ,int &actions)
{
        // page number 2
        clearScreen();
        fmt::println("Ok , Now you can choose one villager and move it");
        int ch;
        while(true)
        {
            fmt::println("1. Move a villager to neighbor place");
            fmt::println("2. Bring a villager from neighbor place");
            fmt::println("3. Back");
            fmt::println("4. Exit");
            ch = getCommand("Please Enter your choise");
            if (ch <= 0 || ch > 4) fmt::println("Invalid choise for guide method");
            else
            break;
        }
        clearScreen();
        if (ch == 1)
        {
            string current = hero->getCurrentPlace()->getName();
            vector<shared_ptr<Place>> neis = hero->getCurrentPlace()->getNeighbors();
            vector<shared_ptr<Villager>> hereVills;
    
            for(auto loc : sys->getLocations())
            {
                if (loc->getName() == current)
                {
                    for(auto vill : loc->getVillagers())
                    hereVills.push_back(vill);
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
                while(true)
                {
                    for(int i {}; i < hereVills.size(); i++)
                    {
                        cout << i + 1 << ". " << hereVills[i]->getName() << '\n';
                    }
                    villNum = getCommand("Enter a villager number");
                    if (villNum <= 0 || villNum > hereVills.size()){
                        fmt::println("invalid villager number");
                    }
                    else 
                    break;
                }
                // choose place 
                while (true)
                {
                    for(int i {}; i < neis.size(); i++)
                    {
                        cout << i + 1 << ". " << neis[i] << '\n';
                    }
                    locNum = getCommand("Enter a place Number");
                    if (locNum <= 0 || locNum > neis.size()){
                        fmt::println("invalid villager number");
                    }
                    else 
                    break;
                }

                sys->moveVillager(hereVills[villNum - 1] , neis[locNum - 1]);


                if (hereVills[villNum - 1]->getSafeZone() == neis[locNum - 1])
                {
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
            string current = hero->getCurrentPlace()->getName();
            vector<shared_ptr<Place>> neis = hero->getCurrentPlace()->getNeighbors();
            fmt::println("Choose the place with u want to bring a villager from");
            while (true)
            {
                for(int i {}; i < neis.size(); i++)
                {
                    cout << i + 1 << ". " << neis[i]->getName() << '\n';
                }
                locNum = getCommand("Enter a place Number");
                if (locNum <= 0 || locNum > neis.size())\
                {
                    fmt::println("invalid villager number");
                }
                else
                break;
            }
            vector<shared_ptr<Villager>> hereVills;
            for(auto loc : sys->getLocations())
            {
                if (loc->getName() == neis[locNum - 1]->getName())
                {
                    for(auto vill : loc->getVillagers()) 
                    hereVills.push_back(vill);
                    break;
                }
            }
            if (hereVills.size() == 0)
            {
                this->pageNumber = PageNumbers::GUIDE_PAGE; return;
            }
            while(true)
            {
                for(int i {}; i < hereVills.size(); i++)
                {
                    cout << i + 1 << ". " << hereVills[i]->getName() << '\n';
                }
                villNum = getCommand("Enter a villager number");
                if (villNum <= 0 || villNum > hereVills.size())
                {
                    fmt::println("invalid villager number");
                }
                else 
                break;
            }

            sys->moveVillager(hereVills[villNum - 1],neis[locNum - 1]);
            
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
        else if (ch == 3) { this->backButton(); return; }
        else if (ch == 4){ this->pageNumber = PageNumbers::EXIT_PAGE; return; }
    }

    void Tui::pickUpPage(std::shared_ptr<HeroBase>& hero ,int &actions)
    {
         //page number 3
    clearScreen();
    fmt::println("You can pick up here items");
    fmt::println("you can back from this page with chosing stop picking");
    string current = hero->getCurrentPlace()->getName();
    auto colorToString = [](Color c) -> string {
        if (c == Color::BLUE) return "blue";
        else if (c == Color::RED) return "red";
        else if (c == Color::YELLOW) return "yellow";
    };
    vector<Item> hereItems;
    for(auto loc : sys->getLocations())
    {
        if (loc->getName() == current)
        {
            for(auto item : loc->getItems()) 
            hereItems.push_back(item);
        }
    }
    int firstItemsSize = hereItems.size();
    while(true)
    {
        int itemNum;
        for(int i {}; i < hereItems.size(); i++)
        {
            fmt::println("{}. {} {} ({})", i + 1 ,hereItems[i].name ,hereItems[i].name ,hereItems[i].power); 
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
                for(auto loc : sys->getLocations())
                {
                    if (loc->getName() == current)
                    {
                        loc->removeItem(hereItems[itemNum - 1]);
                        break;
                    }
                }           
            }
        }
        else 
        {
            fmt::println("Invalid item Entered");
        }
    }
}

void Tui::advancedPage(shared_ptr<HeroBase>& hero , int &actions){
    // page number 4
    clearScreen();
    bool flag = true;

    if(hero->getCurrentPlace()->getName() == "precinct")
    {
        flag=false;
        std::cout << "your providing evidence for invisible man (since your in precinct you cant do anything else)\n";
        bool check=true;
        std::vector<Item> validItems;
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
            std::cout << "you dont have a item that has come from barn, inn , laboraory, institute or mansion\n";
            this_thread::sleep_for(chrono::seconds(3));
            this->pageNumber = PageNumbers::HERO_PHASE_PAGE;
            return;
        }

        while(true)//if choice was invalid it didnt get out of the advance it will get another value
        {
            for(int i = 0;i < validItems.size();i++)
            {
                std::cout << "the Items that you can choose as evidence\n";
                std::cout << i+1 << " - " << validItems[i] << '\n';
            }

            int choice;
            choice=getCommand();
            
            if (choice < 1 || choice > validItems.size() )
            {
                std::cout << "invalid choice try again\n";
                continue;
            }

                Item selectedItem = validItems[choice - 1];
                for(auto removeItem = hero->getHeroItems().begin();removeItem!=hero->getHeroItems().end();removeItem++)
                {
                    if(removeItem->name == selectedItem.name)
                    {
                        hero->getHeroItems().erase(removeItem);
                        sys->addItem(*removeItem);
                        break;
                    }
                }

                for(auto removeItem = sys->getEvidence().begin();removeItem!=sys->getEvidence().end();removeItem++)
                {
                    if( *removeItem == selectedItem.place)
                    {
                        sys->getEvidence().erase(removeItem);
                        break;
                    }
                }

                std::cout << "you successfully added " << selectedItem  << "\nto invisble mans board\n";
                actions--;
                break;
        }
    }
    
    vector<string> coffins = sys->getCoffins();

    for(auto coffin = coffins.begin();coffin!=coffins.end();coffin++)
    {
        if(hero->getCurrentPlace()->getName() == *coffin)
        {
            flag = false;
            int totalRedItemPower = 0;
            std::vector<Item> redItems;
        for (const auto& item : hero->getHeroItems())
        {
            if (item.color == Color::RED)
            {
                totalRedItemPower += item.power;
                redItems.push_back(item);
            }
        }

        if (totalRedItemPower < 6)
        {
            std::cout << "not enough red item to destroy a coffin\n";
            this_thread::sleep_for(chrono::seconds(3));
        this->pageNumber = PageNumbers::HERO_PHASE_PAGE;
        return;   
        }
        std::cout << "destroying a coffin in -> " << *coffin << std::endl;
            std::cout << "choose some red item with a valuation more than 6\n";

            int chosenItemsPower = 0 ;
            std::vector<Item> usedItem;
            while(chosenItemsPower < 6)
            {
                for (int i = 0; i < redItems.size(); i++)
                {
                    std::cout << i + 1 << " - " << redItems[i].name << " Power-> " << redItems[i].power << '\n';
                }
                int choice;
                choice = getCommand();
                if (choice < 1 || choice > redItems.size())
                {
                    std::cout << "invalid choice try again\n";
                    continue;
                }

                Item selectedItem = redItems[choice - 1];
                usedItem.push_back(selectedItem);
                chosenItemsPower += selectedItem.power;

                redItems.erase(redItems.begin() + (choice - 1));
            }

            for (const auto& item : usedItem) {
                for (auto removeItem = hero->getHeroItems().begin(); removeItem != hero->getHeroItems().end(); removeItem++) {
                    if (removeItem->name == item.name)
                    {
                        hero->getHeroItems().erase(removeItem);
                        sys->addItem(*removeItem);
                        break; 
                    }
                }
            }


            coffins.erase(coffin);
            actions--;
            std::cout << "the coffin destroyed successfully in -> " << hero->getCurrentPlace()->getName() <<std::endl;             
            break;
    }
}
    if(flag)
    {
        std::cout << "you cant use advanced action in your current location-> " << hero->getCurrentPlace()->getName() << '\n';
        this_thread::sleep_for(chrono::seconds(3));
        this->pageNumber = PageNumbers::HERO_PHASE_PAGE;
        return;
    }
}

void Tui::specialActionPage(shared_ptr<HeroBase>& hero , int &actions){
    //page number 7
    clearScreen();
    string current = hero->getCurrentPlace()->getName();
    vector<shared_ptr<Place>> neis = hero->getCurrentPlace()->getNeighbors();
    if (hero->getHeroName() == "mayor"){
        fmt::print("Mayor has not any special action !!!");
        while (true)
        {
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
        string curr = neis[neiNum]->getName();
        auto colorToString = [](Color c) -> string {
            if (c == Color::BLUE) return "blue";
            else if (c == Color::RED) return "red";
            else if (c == Color::YELLOW) return "yellow";
        };
        vector<Item> hereItems;
        for(auto loc : sys->getLocations()){
            if (loc->getName() == curr){
                for(auto item : loc->getItems()) hereItems.push_back(item);
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
                    hero->addHeroItems(hereItems[itemNum - 1]);
                    for(auto loc : sys->getLocations()){
                        if (loc->getName() == current){
                            loc->removeItem(hereItems[itemNum - 1]);
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




void Tui::runGame() {
    this->welcomePage();
    int round {0};
    int playerCount = playerPriority.size();
    bool doNextPhase {true};
    while (this->pageNumber != PageNumbers::EXIT_PAGE)
    {

        doNextPhase = true;

        string currentHeroName = playerPriority[round % playerCount];
        shared_ptr<HeroBase> currentHero {nullptr};
        for (auto h : sys->getAllHeros()){
            if (h->getHeroName() == currentHeroName) { currentHero = h; break;}
        }
        int actions = currentHero->getActionCount();
        while(actions != 0 && pageNumber != PageNumbers::EXIT_PAGE) {
            if (this->pageNumber == PageNumbers::HERO_PHASE_PAGE) this->heroPhasePage(currentHero , actions);
            else if (this->pageNumber == PageNumbers::MOVE_PAGE) this->movePage(currentHero , actions);
            else if (this->pageNumber == PageNumbers::GUIDE_PAGE) this->guidePage(currentHero , actions);
            else if (this->pageNumber == PageNumbers::PICKUP_PAGE) this->pickUpPage(currentHero , actions);
            else if (this->pageNumber == PageNumbers::SPECIALACTION_PAGE) this->specialActionPage(currentHero , actions);
            else if (this->pageNumber == PageNumbers::ADVANCED_PAGE) this->advancedPage(currentHero , actions);
            // else if (this->pageNumber == PageNumbers::DEFEAT_PAGE) this->defeatPage(currentHero , actions);
            else if (this->pageNumber == PageNumbers::PLAYPERK_PAGE) this->playPerkPage(currentHero , actions , doNextPhase);
        }
        if (this->pageNumber != PageNumbers::EXIT_PAGE && doNextPhase == true) 
        {
            shared_ptr<MonsterBase> m = (sys->getAllMonsters())[round % playerCount];
            // this->monsterPhasePage(m , currentHeroName);
        }
        round++;
    }
    this->quitPage();
}
