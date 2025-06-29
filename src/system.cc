#include "system.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include "monster.hpp"
#include "place.hpp"
#include "hero.hpp"
#include <queue>
#include <algorithm>

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
    // for(auto loc : this->allLocations) {
    //     fmt::print("\n---------------- {} -----------  \n" , loc->getPlaceName());
    //     fmt::print("      Here items : ");
    //     for(auto i : loc->getAllItems()){
    //         fmt::print("{} " , i.name);
    //     }
    //     fmt::print("\n      Here monsters : ");
    //     for(auto i : loc->getAllMonsters()){
    //         fmt::print("{} " , i->getMonsterName());
    //     }
    //     fmt::print("\n      Here villagers : ");
    //     for(auto i : loc->getAllVillagers()){
    //         fmt::print("{} " , i->getVillagerName());
    //     }
    //     fmt::print("\n      Here heros : ");
    //     for(auto i : loc->getAllHeros()){
    //         fmt::print("{} " , i->getHeroName());
    //     }
    // }
    for (auto vill : allVillagers){
        cout << vill->getVillagerName() << " ";
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
                                if ((loc->getAllVillagers()).size() > 0 || (loc->getAllHeros()).size() > 0) 
                                {
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
        case SearchType::ETV:
            while (!q.empty() && target.empty()) {
                string currentPlace {q.front()};
                q.pop();
               
                for (auto nei : this->gameMap[currentPlace]) {
                    if (!visited[nei]) {
                        visited[nei] = true;
                        parent[nei] = currentPlace;

                        for (auto loc : this->allLocations) {
                            if (loc->getPlaceName() == nei) {
                                if ((loc->getAllVillagers()).size() > 0) 
                                {
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
    // delete from system list using iterator to avoid index issues
    auto it = std::find_if(this->monsters.begin(), this->monsters.end(),
        [&monsterName](const shared_ptr<MonsterBase>& monster) {
            return monster->getMonsterName() == monsterName;
        });
    
    if (it != this->monsters.end()) {
        this->monsters.erase(it);
    }
    
    // delete from places
    for (auto loc : this->allLocations){
        loc->deleteMonster(string(monsterName));
    }
}

void System::killVillager(string_view villName){
    // delete from system list using iterator to avoid index issues
    auto it = std::find_if(this->allVillagers.begin(), this->allVillagers.end(),
        [&villName](const shared_ptr<Villager>& villager) {
            return villager->getVillagerName() == villName;
        });
    
    if (it != this->allVillagers.end()) {
        this->allVillagers.erase(it);
    }
    
    // delete from places - FIXED: was calling deleteMonster instead of deleteVillager
    for (auto loc : this->allLocations){
        loc->deleteVillager(string(villName));
    }
}

char System::rollDice() const
{
    char diceChars[6] = {'-' , '*' , '-' , '-' , '!' , '-'};
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 5);
    return diceChars[distrib(gen)];
}

void System::moveVillager(string_view villName , string_view _newPlace){
    // Find the villager
    shared_ptr<Villager> currentEntity {nullptr};
    for(auto vill : this->allVillagers){
        if (vill->getVillagerName() == villName) { 
            currentEntity = vill; 
            break; 
        }
    }
    
    // Check if villager exists
    if (!currentEntity) {
        // Villager not found - this could be a bug or the villager was already killed
        return;
    }

    // Find the target location
    shared_ptr<Place> targetLocation {nullptr};
    for(auto loc : this->allLocations){
        if(loc->getPlaceName() == _newPlace) {
            targetLocation = loc;
            break;
        }
    }
    
    // Check if target location exists
    if (!targetLocation) {
        // Target location not found
        return;
    }

    // Delete from current place (if any) - Villager doesn't track location, so search all
    for (auto loc : this->allLocations){
        const auto& villagers = loc->getAllVillagers();
        for(const auto& vill : villagers){
            if (vill->getVillagerName() == currentEntity->getVillagerName()){
                loc->deleteVillager(currentEntity->getVillagerName());
                break;
            }
        }
    }

    // Add to new place 
    targetLocation->addVillager(currentEntity);
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
    // Find the monster
    shared_ptr<MonsterBase> currentEntity {nullptr};
    for(auto monst : this->monsters){
        if (monst->getMonsterName() == _monsterName) { 
            currentEntity = monst; 
            break; 
        }
    }
    
    // Check if monster exists
    if (!currentEntity) {
        // Monster not found - this could be a bug or the monster was already killed
        return;
    }

    // Find the target location
    shared_ptr<Place> targetLocation {nullptr};
    for(auto loc : this->allLocations){
        if(loc->getPlaceName() == _newPLace) {
            targetLocation = loc;
            break;
        }
    }
    
    // Check if target location exists
    if (!targetLocation) {
        // Target location not found
        return;
    }

    // Get current location from monster and delete from it
    string currentLocationStr = currentEntity->getCurrentLocation();
    if (!currentLocationStr.empty()) {
        for (auto loc : this->allLocations) {
            if (loc->getPlaceName() == currentLocationStr) {
                loc->deleteMonster(currentEntity->getMonsterName());
                break;
            }
        }
    }

    // Add to new place 
    targetLocation->addMonster(currentEntity);
    currentEntity->setCurrentLocation(_newPLace);
}

void System::moveHero(string_view _heroName , string_view _newPlace){
    // Find the hero
    shared_ptr<HeroBase> currentEntity {nullptr};
    for(auto hero : this->heros){
        if (hero->getHeroName() == _heroName) { 
            currentEntity = hero; 
            break; 
        }
    }
    
    // Check if hero exists
    if (!currentEntity) {
        // Hero not found - this could be a bug
        return;
    }

    // Find the target location
    shared_ptr<Place> targetLocation {nullptr};
    for(auto loc : this->allLocations){
        if(loc->getPlaceName() == _newPlace) {
            targetLocation = loc;
            break;
        }
    }
    
    // Check if target location exists
    if (!targetLocation) {
        // Target location not found
        return;
    }

    // Get current location from hero and delete from it
    string currentLocationStr = currentEntity->getCurrentPlace();
    if (!currentLocationStr.empty()) {
        for (auto loc : this->allLocations) {
            if (loc->getPlaceName() == currentLocationStr) {
                loc->deleteHero(currentEntity->getHeroName());
                break;
            }
        }
    }
    
    // Add to new place 
    targetLocation->addHero(currentEntity);
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
    // cout << monsters[0]->getCurrentLocation() << "\n";
    for (auto h : heros) 
    {
        cout << h->getCurrentPlace() << "\n";
    }
}

int System::getTerrorLevel() const { return this->terrorLevel; }

void System::increaseTerrorLevel() { (this->terrorLevel)++; }