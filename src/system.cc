#include "system.hpp"
#include <fstream>
#include "exceptions.hpp"
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <queue>

using namespace std;

System::System(){
    // Here we initialize the game
    monsterDeck = make_shared<MonsterCardDeck<MonsterCard>>();
    itemBag = make_shared<ItemBag<Item>>();
    perkDeck = make_shared<PerkDeck<Perk>>();

    // setup heroes
    arch = make_shared<Archaeologist>(4 , "archaeologist" , perkDeck->pickOneRandomly());
    mayor = make_shared<Mayor>(6 , "mayor" , perkDeck->pickOneRandomly());

    // setup monsters
    dracula = make_shared<Dracula>("dracula" , true);
    invisibleMan = make_shared<InvisibleMan>("invisibleMan" , false);

    // ----- collect the name of all locations
    ifstream file ("../data/before_game/locations.txt");
    if (file.is_open()) {
        stringstream stream;     
        string line , placeName;
        while(getline(file , line)){
            stream.str(line);
            stream >> placeName;
            allLocations.push_back(make_shared<Place>(placeName));
        }
        file.close();
    }
    else {
        throw FileOpenningExecption("couldn't open file locations.txt");
    }

    // add neighbors to locations
    ifstream file2("../data/before_game/locations.txt");
    if (file2.is_open()) {
        string line , placeName , neighbors;
        while(getline(file2 , line)){
            stringstream stream(line);
            stream >> placeName >> neighbors;

            // seprate neighbors here
            stringstream ss(neighbors);
            vector<string> temp; string x;
            while(getline(ss , x , '_')) temp.push_back(x);

            vector<shared_ptr<Place>> neigList;

            for(const string &n : temp){
                for(auto loc : allLocations){
                    if(loc->getName() == n){
                        neigList.push_back(loc);
                    }
                }
            }

            for(auto loc : allLocations){
                if(loc->getName() == placeName){
                    loc->setNeighbors(neigList);
                    break;
                }
            }
        }
        file2.close();
    }
    else {
        throw FileOpenningExecption("couldn't open file locations.txt");
    }

    // put heroes in right place
    // we have to set it in place and in hero 
    for(auto place : allLocations){
        if (place->getName() == "docks"){
            place->addHero(arch);
            arch->setCurrentPlace(place);
        }
        else if (place->getName() == "theatre"){
            place->addHero(mayor);
            mayor->setCurrentPlace(place);
        }
    }

    // TODO : here we should make a villiagers list ..
    ifstream villFile("../data/before_game/villagers.txt");
    if(villFile.is_open()){
        string line , name , safeZone;
        while(getline(villFile , line)){
            stringstream stream(line);
            stream >> name >> safeZone;
            shared_ptr<Villager> temp {make_shared<Villager>(name)};

            for(auto _place : allLocations){
                if (_place->getName() == safeZone){
                    temp->setSafeZone(_place);
                }
            }

            this->allVillagers.push_back(temp);
        }
        villFile.close();
    }
    else {
        throw FileOpenningExecption("couldn't open villager.txt");
    } 

    // add location to our monsters
    for (auto loc : allLocations){
        if(loc->getName() == "docks"){
            dracula->setCurrentLocation(loc);
            loc->addMonster(dracula);
        }
        else if(loc->getName() == "hospital"){
            invisibleMan->setCurrentLocation(loc);
            loc->addMonster(invisibleMan);
        }
    }
}

int System::getTerrorLevel() const { return this->terrorLevel; }

void System::killMonster(shared_ptr<MonsterBase> monst){
    for(auto loc : this->allLocations){
        loc->deleteMonster(monst->getMonsterName());
    }
    if (monst->getMonsterName() == "dracula") this->dracula = nullptr;
    else if (monst->getMonsterName() == "invisibleMan") this->invisibleMan = nullptr;
}

void System::killVillager(shared_ptr<Villager> vill){
    for (auto loc : this->allLocations){
        loc->deleteVillager(vill->getName());
    }
    int index {0};
    for(int i {}; i < this->allVillagers.size(); i++) {
        if (this->allVillagers[i]->getName() == vill->getName()) { index = i; break; }
    }
    this->allVillagers.erase(this->allVillagers.begin() + index);
}

void System::increaseTerrorLevel() { this->terrorLevel++; }

int System::foundCluesCount(string type){
    if (type == "coffin") return 4 - this->coffins.size();
    else return 5 - this->evidence.size();
}

void System::moveMonster(shared_ptr<MonsterBase> monst , shared_ptr<Place> newPlace){
    shared_ptr<Place> currentPlace = monst->getCurrentLocation();
    monst->setCurrentLocation(newPlace);
    newPlace->addMonster(monst);
    currentPlace->deleteMonster(monst->getMonsterName());
}

void System::moveHero(shared_ptr<HeroBase> her , shared_ptr<Place> newPlace){
    shared_ptr<Place> currentPlace = her->getCurrentPlace();
    her->setCurrentPlace(newPlace);
    newPlace->addHero(her);
    currentPlace->deleteHero(her->getHeroName());
}

void System::moveVillager(shared_ptr<Villager> vill , shared_ptr<Place> newPlace){
    shared_ptr<Place> currentPlace = vill->getVillagerLoc();
    vill->changeLoc(newPlace);
    newPlace->addVillager(vill);
    currentPlace->deleteVillager(vill->getName());
}

vector<shared_ptr<Place>> System::findShortestPath(shared_ptr<Place> _p){
    unordered_map<shared_ptr<Place>, bool> visited;
    unordered_map<shared_ptr<Place>, shared_ptr<Place>> parent;
    for (auto x : allLocations) visited[x] = false;

    queue<shared_ptr<Place>> q;
    visited[_p] = true;
    q.push(_p);
    shared_ptr<Place> target {nullptr};

    while(!q.empty() && !target){
        shared_ptr<Place> currentPlace {q.front()};
        
        for (auto nei : currentPlace->getNeighbors()){
            if (!visited[nei]) {
                visited[nei] = true;
                parent[nei] = currentPlace;
                
                if (nei->getAllHeroes().size() > 0 || nei->getVillagers().size() > 0){
                    target = nei;
                    break;
                }
                q.push(nei);
            }
        }
        q.pop();
    }

    vector<shared_ptr<Place>> path;
    if (target) {
        for (shared_ptr<Place> current = target; current != nullptr; current = parent[current]) {
            path.push_back(current);
        }
        reverse(path.begin(), path.end());
    }
    return path;
}

Perk System::getRandomPerk() {
    if (perkDeck->size() > 0) 
        return perkDeck->pickOneRandomly(); 
}

Item System::getRandomItem() { return itemBag->pickOneRandomly(); }

void System::putVillagerInPlace(const string &_placeName , const string &_villName){
    for (auto _vill : this->allVillagers){
        if (_vill->getName() == _villName){
            
            if (_vill->getVillagerLoc() != nullptr){
                _vill->getVillagerLoc()->deleteVillager(_villName);
            }

            for (auto _place : this->allLocations){
                if (_place->getName() == _placeName) {
                    _vill->changeLoc(_place);
                    _place->addVillager(_vill);
                }
            }
        }
    }
}

char System::rollDice(){
    char diceChars[6] = {'-' , '*' , '-' , '-' , '!' , '-'};
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 5);
    return diceChars[distrib(gen)];
}

void System::showLocs() {
    // lets show all infoes 
    // for (auto loc : allLocations){
    //     cout << "------------  " << loc->getName() << "  -------------\n";
    //     // villgers here !!
    //     cout << "villagers : \n";
    //     for (auto vil : loc->getVillagers()){
    //         cout << "  - " << vil->getName() << '\n';
    //     } 

    //     cout << "Heroes : \n";
    //     for (auto vil : loc->getAllHeroes()){
    //         cout << "  - " << vil->getHeroName() << '\n';
    //     } 

    //     cout << "monsters : \n";
    //     for (auto vil : loc->getMonsters()){
    //         cout << "  - " << vil->getMonsterName() << '\n';
    //     } 

    //     cout << "Neighbors : ";
    //     for (auto nei : loc->getNeighbors()){
    //         cout << nei->getName() << " ";
    //     }
    //     cout << '\n';
    // }

    // for (auto x : findShortestPath(invisibleMan->getCurrentLocation())){
    //     cout << x->getName() << " ";
    // }
    // show villagers state 
    for (auto vil : allVillagers) {
        cout << "--------------  " << vil->getName() << "  ---------------" << '\n';
        cout << " - Safe zone : " << vil->getSafeZone()->getName() << '\n';
    }
}

void System::runMonsterPhase(){
    // first pick a monster card 
    MonsterCard currentCard {monsterDeck->pickOneRandomly()};
    // currentCard.name = "Thief";
    // currentCard.strikePriorities = {"du"};
    // TODO: Here we can show which card picked and is running !!


    // put items in right place 
    for (int i {}; i < currentCard.itemCount; i++){
        Item thisItem {itemBag->pickOneRandomly()};
        for (auto place : this->allLocations){
            if (place->getName() == thisItem.place){
                place->addItem(thisItem);
            }
        }
    }

    // run the card event !!!
    if (currentCard.name == "Sunrise") {
        for(auto place : this->allLocations){
            if (place->getName() == "crypt"){
                dracula->getCurrentLocation()->deleteMonster("dracula");
                dracula->setCurrentLocation(place);
                place->addMonster(dracula);
            }
        }
    }

    else if (currentCard.name == "Thief"){
        int maxItemCount {0};
        shared_ptr<Place> p {nullptr};

        // search for max of items place
        for (auto place : this->allLocations){
            if (place->getItems().size() > maxItemCount) {
                maxItemCount = place->getItems().size();
                p = place;
            }
        }
        for (auto _item : p->getItems()){
            itemBag->addItem(_item);
        }
        p->getItems().clear();

        invisibleMan->getCurrentLocation()->deleteMonster("invisibleMan");
        invisibleMan->setCurrentLocation(p);
        p->addMonster(invisibleMan);
    }

    else if (currentCard.name == "The_Delivery"){
        this->putVillagerInPlace("docks" , "wilbur&chick");
    }

    else if (currentCard.name == "Fortune_Teller"){
        this->putVillagerInPlace("camp" , "maleva");
    }

    else if (currentCard.name == "Former_Employer"){
        this->putVillagerInPlace("laboratory" , "dr.cranly");
    }

    else if (currentCard.name == "Hurried_Assistant"){
        this->putVillagerInPlace("tower" , "fritz");
    }

    else if (currentCard.name == "The_Innocent"){
        this->putVillagerInPlace("barn" , "maria");
    }

    else if (currentCard.name == "Egyptian_Expert"){
        this->putVillagerInPlace("cave" , "prof.pearson");
    }

    else if (currentCard.name == "The_Ichthyologist"){
        this->putVillagerInPlace("institute" , "dr.reed");
    }

    else if (currentCard.name == "Hypnotic_Gaze"){

    }
    // ------------------------- strike phase :D -----------------------------------

    bool stopStrike {};

    for (int i {}; i < currentCard.strikePriorities.size(); i++){
        if (currentCard.strikePriorities[i] == "du"){
            if (dracula != nullptr){
                // move actions for monster
                bool continueMoving {true};
                int movesCount {currentCard.move};
                while (movesCount && continueMoving){
    
                    if (
                        dracula->getCurrentLocation()->getAllHeroes().size() > 0 ||
                        dracula->getCurrentLocation()->getVillagers().size() > 0
                    ){
                        continueMoving = false;
                    }
                    else {
                        // move the monster here
                        vector<shared_ptr<Place>> pathToEnemy {findShortestPath(dracula->getCurrentLocation())};
                        dracula->getCurrentLocation()->deleteMonster("dracula");
                        for (auto loc : allLocations){
                            if (loc->getName() == pathToEnemy[1]->getName()){
                                dracula->setCurrentLocation(loc);
                                loc->addMonster(dracula);
                            }
                        }
                    }
                    movesCount--;
                } 
                // dice and attack here ...

            }
        }
        else if (currentCard.strikePriorities[i] == "inm"){
            if (invisibleMan != nullptr){
                // move actions for monster
                bool continueMoving {true};
                int movesCount {currentCard.move};
                while (movesCount && continueMoving){
    
                    if (
                        invisibleMan->getCurrentLocation()->getAllHeroes().size() > 0 ||
                        invisibleMan->getCurrentLocation()->getVillagers().size() > 0
                    ){
                        continueMoving = false;
                    }
                    else {
                        // move the monster here
                        vector<shared_ptr<Place>> pathToEnemy {findShortestPath(invisibleMan->getCurrentLocation())};
                        invisibleMan->getCurrentLocation()->deleteMonster("invisibleMan");
                        for (auto loc : allLocations){
                            if (loc->getName() == pathToEnemy[1]->getName()){
                                invisibleMan->setCurrentLocation(loc);
                                loc->addMonster(invisibleMan);
                            }
                        }
                    }
                    movesCount--;
                } 
                // dice and attack here ...

            }
        }
        else {
            if (invisibleMan != nullptr) {
                if (invisibleMan->getIsFrenzed()){
                    // move actions for monster
                    bool continueMoving {true};
                    int movesCount {currentCard.move};
                    while (movesCount && continueMoving){
                        if (
                            invisibleMan->getCurrentLocation()->getAllHeroes().size() > 0 ||
                            invisibleMan->getCurrentLocation()->getVillagers().size() > 0
                        ){
                            continueMoving = false;
                        }
                        else {
                            // move the monster here
                            vector<shared_ptr<Place>> pathToEnemy {findShortestPath(invisibleMan->getCurrentLocation())};
                            invisibleMan->getCurrentLocation()->deleteMonster("invisibleMan");
                            for (auto loc : allLocations){
                                if (loc->getName() == pathToEnemy[1]->getName()){
                                    invisibleMan->setCurrentLocation(loc);
                                    loc->addMonster(invisibleMan);
                                }
                            }
                        }
                        movesCount--;
                    } 
                    // dice and attack here ...

                }
            }
            else if (dracula != nullptr) {
                if (dracula->getIsFrenzed()){
                    // move actions for monster
                    bool continueMoving {true};
                    int movesCount {currentCard.move};
                    while (movesCount && continueMoving){
        
                        if (
                            invisibleMan->getCurrentLocation()->getAllHeroes().size() > 0 ||
                            invisibleMan->getCurrentLocation()->getVillagers().size() > 0
                        ){
                            continueMoving = false;
                        }
                        else {
                            // move the monster here
                            vector<shared_ptr<Place>> pathToEnemy {findShortestPath(invisibleMan->getCurrentLocation())};
                            invisibleMan->getCurrentLocation()->deleteMonster("invisibleMan");
                            for (auto loc : allLocations){
                                if (loc->getName() == pathToEnemy[1]->getName()){
                                    invisibleMan->setCurrentLocation(loc);
                                    loc->addMonster(invisibleMan);
                                }
                            }
                        }
                        movesCount--;
                    } 
                    // dice and attack here ...
                }
            }
        }
    }
}


//-----HeroPhase----

void System::runHeroPhase()
{
    shared_ptr<HeroBase> HeroTurn;

    if(turn)
    HeroTurn = arch;
    else
    HeroTurn = mayor;

    turn = !turn;

    while(HeroTurn -> getActionCount() != 0)
    {
         cout <<"select your option\n";
         cout << "1-Move\n2-Pick up\n3-Guid\n4-Advanced\n5-defeat\n6-Using perks\n7-help\n8-Quit\n";
         if(HeroTurn->getHeroName() == "archaeologist")
         cout << "9-Speciall action\n";
         cout << "your current location is -> " << HeroTurn->getCurrentPlace()->getName() << endl;
         int choice;
         cin >> choice;
         switch (choice)
         {
        case 1:
            HeroTurn->moveAction(perkDeck);
            break;
        case 2:
            HeroTurn->pickUpAction();
            break;
        case 3:
            HeroTurn->guideAction(perkDeck);
            break;
        case 4:
            HeroTurn->advanceAction(coffins, evidence, itemBag);
            break;
        case 5:
            HeroTurn->defeatAction(coffins, evidence, itemBag, dracula, invisibleMan);
            break;
        case 6:
            //HeroTurn->runPerkCard(arch, mayor, dracula, invisibleMan,allLocations, itemBag,perkDeck,BreakOfDawn);
            break;
        case 7:
            HeroTurn->Help();
        case 8:
            return;
        case 9:
        if (HeroTurn->getHeroName() == "archaeologist")
        {
            HeroTurn->specialAction();
        }
        else
        {
            std::cout << "invalid choice try again\n";
        }
            break;
         default:
            cout << "invalid choice try again\n";
            break;
        }
    }
    HeroTurn->setActionCount((turn)?4:5);
    
}

//-------game---------

void System::runGame()
{
    this->killMonster(this->dracula);
    this->killVillager(this->allVillagers[0]);
}