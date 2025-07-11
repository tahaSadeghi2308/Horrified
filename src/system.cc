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
    mayor = make_shared<Mayor>(5 , "mayor" , perkDeck->pickOneRandomly());

    // setup monsters
    dracula = make_shared<Dracula>("dracula" , true , this);
    invisibleMan = make_shared<InvisibleMan>("invisibleMan" , false , this);

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
            dracula->setCurrentLocation(place);
            place->addMonster(dracula);
        }
        else if (place->getName() == "theatre"){
            place->addHero(mayor);
            mayor->setCurrentPlace(place);
        }
        else if(place->getName() == "hospital") {
            invisibleMan->setCurrentLocation(place);
            place->addMonster(invisibleMan);
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

    for(int i{} ; i < 12 ; i++) {
        Item selectedItem = getRandomItem();
        putItemInPlace(selectedItem.place , selectedItem);
    }

    allMonsters.push_back(dracula);
    allMonsters.push_back(invisibleMan);

}

int System::getTerrorLevel() const { return this->terrorLevel; }

void System::putItemInPlace(string_view placeName , Item i){
    for (auto loc : this->allLocations){
        if (loc->getName() == placeName) { loc->addItem(i);break; }
    }
}

bool System::destroyClue(string type , string_view clueNamePlace){
    if (type == "coffin"){
        for (int i {}; i < this->coffins.size(); i++){
            if (this->coffins[i] == clueNamePlace){
                this->coffins.erase(this->coffins.begin() + i);
                return true;
            }
        }
        return false;
    }
    else if (type == "evidence"){
        for (int i {}; i < this->evidence.size(); i++){
            if (this->evidence[i] == clueNamePlace){
                this->evidence.erase(this->evidence.begin() + i);
                return true;
            }
        }
        return false;
    }
    //this should never run
    return false;
}

void System::placeWithMaxItem() {
    int itemCount {0};
    shared_ptr<Place> maxPlace {nullptr};
    for(auto loc : this->allLocations){
        if (loc->getItems().size() >= itemCount){
            itemCount = loc->getItems().size();
            maxPlace = loc;
        }
    }

    vector<Item> items = maxPlace->getItems();

    for (auto i : items) {
        maxPlace->removeItem(i);
    }

    if (invisibleMan != nullptr) this->moveMonster(invisibleMan , maxPlace);
}

void System::killMonster(shared_ptr<MonsterBase> monst){
    if (!monst) return;
    for (auto& loc : this->allLocations) {
        loc->deleteMonster(monst->getMonsterName());
    }
    

    for(auto it = allMonsters.begin() ; it !=allMonsters.end() ; it++)
    {
        if((*it)->getMonsterName() == monst->getMonsterName())
        {
            allMonsters.erase(it);
            break;
        }
    }

    if (monst->getMonsterName() == "dracula") this->dracula = nullptr;
    else if (monst->getMonsterName() == "invisibleMan") this->invisibleMan = nullptr;
    this->changeFrenzy();
}

void System::killVillager(shared_ptr<Villager> vill){
    if (!vill) return;
    for (auto& loc : this->allLocations) {
        loc->deleteVillager(vill->getName());
    }
    auto it = std::remove_if(
        this->allVillagers.begin(), this->allVillagers.end(),
        [&](const shared_ptr<Villager>& v) { return v->getName() == vill->getName(); }
    );
    this->allVillagers.erase(it, this->allVillagers.end());
}

void System::increaseTerrorLevel() { this->terrorLevel++; }

int System::foundCluesCount(string type){
    if (type == "coffin") return 4 - this->coffins.size();
    else return 5 - this->evidence.size();
}

void System::moveMonster(shared_ptr<MonsterBase> monst , shared_ptr<Place> newPlace){
    if (!monst || !newPlace) return;
    shared_ptr<Place> currentPlace = monst->getCurrentLocation();
    if (currentPlace == newPlace) return;
    if (currentPlace) currentPlace->deleteMonster(monst->getMonsterName());
    monst->setCurrentLocation(newPlace);
    newPlace->addMonster(monst);
}

void System::moveHero(shared_ptr<HeroBase> her , shared_ptr<Place> newPlace){
    if (!her || !newPlace) return;
    shared_ptr<Place> currentPlace = her->getCurrentPlace();
    if (currentPlace == newPlace) return;
    if (currentPlace) currentPlace->deleteHero(her->getHeroName());
    her->setCurrentPlace(newPlace);
    newPlace->addHero(her);
}

void System::moveVillager(shared_ptr<Villager> vill , shared_ptr<Place> newPlace){
    if (!vill || !newPlace) return;
    shared_ptr<Place> currentPlace = vill->getVillagerLoc();
    if (currentPlace == newPlace) return;
    if (currentPlace) currentPlace->deleteVillager(vill->getName());
    vill->changeLoc(newPlace);
    newPlace->addVillager(vill);
}

vector<shared_ptr<Place>> System::findShortestPath(
    shared_ptr<Place> _p ,
    SearchingType type
){
    unordered_map<shared_ptr<Place>, bool> visited;
    unordered_map<shared_ptr<Place>, shared_ptr<Place>> parent;
    for (auto x : allLocations) visited[x] = false;

    queue<shared_ptr<Place>> q;
    visited[_p] = true;
    q.push(_p);
    shared_ptr<Place> target {nullptr};
    switch (type){
        case SearchingType::ETO:
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
            break;
        case SearchingType::ETV:
            while(!q.empty() && !target){
                shared_ptr<Place> currentPlace {q.front()};
                for (auto nei : currentPlace->getNeighbors()){
                    if (!visited[nei]) {
                        visited[nei] = true;
                        parent[nei] = currentPlace;
                        if (nei->getVillagers().size() > 0){
                            target = nei;
                            break;
                        }
                        q.push(nei);
                    }
                }
                q.pop();
            }
            break;
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

void System::changeFrenzy() {
    vector<shared_ptr<MonsterBase>> aliveMonsters;
    for(auto m : this->getAllMonsters()) {
        if (m != nullptr) aliveMonsters.push_back(m);
    }
    if (aliveMonsters.size() > 1) {
        sort (
            aliveMonsters.begin(),
            aliveMonsters.end(),
            [] (shared_ptr<MonsterBase> A , shared_ptr<MonsterBase> B) -> bool {
                return A->getFrenzyOrder() < B->getFrenzyOrder();
            }
        );
        for (int i{}; i < aliveMonsters.size(); i++){
            if (i != aliveMonsters.size() - 1){
                if (aliveMonsters[i]->getIsFrenzed()){
                    aliveMonsters[i + 1]->changeFrenzedState(true);
                    aliveMonsters[i]->changeFrenzedState(false);
                    break;
                }
            }
        }
    }
}

vector<shared_ptr<Place>> System::findShortestPath(
    shared_ptr<Place> source ,
    shared_ptr<Place> destination
){
    unordered_map<shared_ptr<Place>, bool> visited;
    unordered_map<shared_ptr<Place>, shared_ptr<Place>> parent;
    for (auto x : allLocations) visited[x] = false;

    queue<shared_ptr<Place>> q;
    visited[source] = true;
    q.push(source);
    shared_ptr<Place> target {nullptr};
    while(!q.empty() && !target){
        shared_ptr<Place> currentPlace {q.front()};
        for (auto nei : currentPlace->getNeighbors()){
            if (!visited[nei]) {
                visited[nei] = true;
                parent[nei] = currentPlace;
                if (nei->getName() == destination->getName()){
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
        //this should never run
        return Perk(); 
}

MonsterCard System::getRandomMonstCard() {
    return monsterDeck->pickOneRandomly();
}

int System::isEndGame() const {
    if (monsterDeck->size() == 0) return 1;
    else if (this->dracula == nullptr && this->invisibleMan == nullptr) return 2;
    else if (this->terrorLevel == 5) return 3;
    return -1;
}

void System::addItem(Item i) { this->itemBag->push(i); }

char System::rollDice() {
    char diceChars[6] = {'-' , '*' , '-' , '-' , '!' , '-'};
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 5);
    return diceChars[distrib(gen)];
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

