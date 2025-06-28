#include "system.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include "monster.hpp"
#include "place.hpp"
#include "hero.hpp"
#include <queue>

using namespace std;

System::System() { this->gameInit(); }

void System::gameInit() {
    // this function is for collecting datas for files and put them in right place in system

    // collect locations info
    ifstream file ("../data/before_game/locations.txt");
    if (file.is_open()) {
        string line , placeName , neigh;
        while(getline(file , line)){
            stringstream stream;     
            stream.str(line);
            stream >> placeName >> neigh;
            allLocations.push_back(make_shared<Place>(placeName));
            vector<string> neis;
            stringstream ss(neigh);
            string n;
            while(getline(ss , n , '_')) neis.push_back(n);
            gameMap[placeName] = neis;
        }
        file.close();
    }
    else {
        throw FileOpenningExecption("couldn't open file locations.txt");
    }

    // villagers 
    ifstream villFile ("../data/before_game/villagers.txt");
    if (villFile.is_open()) {
        string line , villName , zone;
        while(getline(villFile , line)){
            stringstream stream;     
            stream.str(line);
            stream >> villName >> zone;
            this->allVillagers.push_back(make_shared<Villager>(villName , zone));
        }
        villFile.close();
    }
    else {
        throw FileOpenningExecption("couldn't open file villagers.txt");
    }

    // monsters
    monsters.push_back(make_shared<Dracula>("dracula" , true , this));
    monsters.push_back(make_shared<InvisibleMan>("invisibleMan" , false , this));
    // TODO : we can do it with a for on monster list !!
    this->moveMonster("dracula" , "camp");
    this->moveMonster("invisibleMan" , "inn");

    // heros 
    heros.push_back(make_shared<Archaeologist>("arch" , 4 , this));
    heros.push_back(make_shared<Mayor>("mayor" , 5 , this));
    this->moveHero("arch" , "docks");
    this->moveHero("mayor" , "theatre");

    // hero -> giving a perk to each hero
    for (auto hero : this->heros) {
        hero->addPerk(this->perks.pickOneRandomly());
    }
}

unordered_map<string , vector<string>>& System::getGameMap() {
    return this->gameMap;
}

vector<shared_ptr<Place>>& System::getLocations() { return this->allLocations; }

vector<shared_ptr<Villager>>& System::getVillagers() { return this->allVillagers; }

vector<shared_ptr<HeroBase>>& System::getHeros() { return this->heros; }

vector<shared_ptr<MonsterBase>>& System::getMonsters() { return this->monsters; }

bool System::allCluesFound(string_view type) const {
    if (type == "coffin"){
        for (bool cof : this->foundCoffin){
            if (!cof) return false;
        }
        return true;
    }
    for (bool cu : this->foundClue){
            if (!cu) return false;
        }
    return true;
}

int System::foundClues(string_view type) {
    int count = 0;
    if (type == "coffin"){
        for (bool& cof : this->foundCoffin)
            if (cof) count++; 
    }
    else {
        for (bool& cu : this->foundClue)
            if (cu) count++;
    } 
    return count;
}

void System::makeFoundClues(string_view type){
    if (type == "coffin"){
        for (bool& cof : this->foundCoffin)
            if (cof == false) { cof = true; break; } 
    }
    else {
        for (bool& cu : this->foundClue)
            if (cu == false) { cu = true; break; }
    }
}

void System::systemInfoShow() {
    for(auto loc : this->allLocations) {
        fmt::print("\n---------------- {} -----------  \n" , loc->getPlaceName());
        fmt::print("      Here items : ");
        for(auto i : loc->getAllItems()){
            fmt::print("{} " , i.name);
        }
        fmt::print("\n      Here monsters : ");
        for(auto i : loc->getAllMonsters()){
            fmt::print("{} " , i->getMonsterName());
        }
        fmt::print("\n      Here villagers : ");
        for(auto i : loc->getAllVillagers()){
            fmt::print("{} " , i->getVillagerName());
        }
        fmt::print("\n      Here heros : ");
        for(auto i : loc->getAllHeros()){
            fmt::print("{} " , i->getHeroName());
        }
    }
    // fmt::print("\n----------------------------------\n");
    // cout << this->findPath(monsters[1]->getCurrentLocation() , ETO).size();

}

vector<string> System::findPath(string source , SearchType type) {
    unordered_map<string, bool> visited;
    unordered_map<string, string> parent;
    for (const auto& [node , nei] : this->gameMap) visited[node] = false;

    queue<string> q;
    visited[source] = true;
    q.push(source);
    string target;

    switch (type) {
        case SearchType::ETO:
            while (!q.empty() && target.empty()) {
                string currentPlace {q.front()};
                q.pop();
               
                for (auto nei : this->gameMap[currentPlace]) {
                    if (!visited[nei]) {
                        visited[nei] = true;
                        parent[nei] = currentPlace;

                        for (auto loc : this->allLocations) {
                            if (loc->getPlaceName() == nei) {
                                if ((loc->getAllVillagers()).size() > 0) { // TODO : and heros should be added  
                                    target = nei; 
                                    break; 
                                }
                            }
                        }
                        if (!target.empty()) break;
                        q.push(nei);
                    }
                }
                
            }
        break;
    }
    vector<string> path;
    if (!target.empty()) {
        for (string current = target; !current.empty(); current = parent[current]) {
            path.push_back(current);
        }
        reverse(path.begin(), path.end());
    }
    return path;
}

void System::addItem(const Item& i) { items.push(i); }

void System::killMonster(string_view monsterName) {
    // delete from system list
    for(int i {}; i < this->monsters.size(); i++){
        if (monsters[i]->getMonsterName() == monsterName) {
            this->monsters.erase(this->monsters.begin() + i);
            break;
        }
    }
    // delete from places
    for (auto loc : this->allLocations){
        loc->deleteMonster(string(monsterName));
    }
}
void System::killVillager(string_view villName){
    // delete from system list
    for(int i {}; i < this->allVillagers.size(); i++){
        if (allVillagers[i]->getVillagerName() == villName) {
            this->allVillagers.erase(this->allVillagers.begin() + i);
            break;
        }
    }
    // delete from places
    for (auto loc : this->allLocations){
        loc->deleteMonster(string(villName));
    }
}

void System::moveVillager(string_view villName , string_view _newPlace){
    shared_ptr<Villager> currentEntity {nullptr};
    for(auto vill : this->allVillagers){
        if (vill->getVillagerName() == villName) { currentEntity = vill; break; }
    }

    // delete from current place 
    for (auto loc : this->allLocations){
        for(auto vill : loc->getAllVillagers()){
            if (vill->getVillagerName() == currentEntity->getVillagerName()){
                loc->deleteVillager(currentEntity->getVillagerName());
            }
        }
    }

    // add to new place 
    for(auto loc : this->allLocations){
        if(loc->getPlaceName() == _newPlace) loc->addVillager(currentEntity);
    }
}


Item System::getRandomItem() { return items.pickOneRandomly(); }

MonsterCard System::getRandomMonstCard() { return monsterCards.pickOneRandomly(); }

Perk System::getRandomPerk() { 
    if (perks.size() > 0) return perks.pickOneRandomly();
}

void System::putItemInPlace(const string& _placeName , const Item &i){
    for (auto loc : this->allLocations){
        if (loc->getPlaceName() == _placeName) {
            loc->addItem(i);
            break;
        }
    }
}

void System::moveMonster(string_view _monsterName , string_view _newPLace){
    shared_ptr<MonsterBase> currentEntity {nullptr};
    for(auto monst : this->monsters){
        if (monst->getMonsterName() == _monsterName) { currentEntity = monst; break; }
    }

    // delete from current place 
    for (auto loc : this->allLocations){
        for(auto monst : loc->getAllMonsters()){
            if (monst->getMonsterName() == currentEntity->getMonsterName()){
                loc->deleteMonster(currentEntity->getMonsterName());
            }
        }
    }

    // add to new place 
    for(auto loc : this->allLocations){
        if(loc->getPlaceName() == _newPLace) loc->addMonster(currentEntity);
    }

    currentEntity->setCurrentLocation(_newPLace);
}

void System::moveHero(string_view _heroName , string_view _newPlace){
    shared_ptr<HeroBase> currentEntity {nullptr};
    for(auto hero : this->heros){
        if (hero->getHeroName() == _heroName) { currentEntity = hero; break; }
    }

    // delete from current place 
    for (auto loc : this->allLocations){
        for(auto hero : loc->getAllHeros()){
            if (hero->getHeroName() == currentEntity->getHeroName()){
                loc->deleteHero(currentEntity->getHeroName());
                break;
            }
        }
    }

    // add to new place 
    for(auto loc : this->allLocations){
        if(loc->getPlaceName() == _newPlace) loc->addHero(currentEntity);
    }

    currentEntity->setCurrentPlace(_newPlace);
}

void System::placeWithMaxItem() {
    int maxItemCount {-1};
    shared_ptr<Place> p {nullptr};
    for(auto loc : this->allLocations){
        int size = loc->getAllItems().size(); 
        if (size > maxItemCount){
            maxItemCount = size;
            p = loc;
        }
    }
    auto temp {p->getAllItems()};

    for (auto i : temp){
        this->items.addItem(i);
        p->deleteItem(i.name);
    }

    this->moveMonster("invisibleMan" , p->getPlaceName());
}

void System::run() {
    monsters[0]->runMonsterPhase();
}

int System::getTerrorLevel() const { return this->terrorLevel; }

void System::increaseTerrorLevel() { (this->terrorLevel)++; }