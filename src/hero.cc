#include "hero.hpp"

using namespace std;

HeroBase::HeroBase(string_view _name , int _actions , System *s) 
    : name(_name) , actionCount(_actions) , sys(s) {}

string HeroBase::getHeroName() const { return this->name; }

string HeroBase::getCurrentPlace() const {
    return this->currentPlace;
}

void HeroBase::setCurrentPlace(string_view _newPlace) {
    this->currentPlace = _newPlace;
}

Archaeologist::Archaeologist(string_view _name , int _actions , System *s) : HeroBase(_name , _actions , s) {}

Mayor::Mayor(string_view _name , int _actions , System *s) : HeroBase(_name , _actions , s) {}

void HeroBase::addPerk(const Perk &p){
    this->perks.push_back(p);
}

const vector<Perk>& HeroBase::getAllPerks() const { return this->perks; }

void HeroBase::deletePerk(string_view perkName) {
    bool isFound {};
    for (int i {}; i < this->perks.size() && !isFound; i++) {
        if (this->perks[i].name == perkName){
            this->perks.erase(perks.begin() + i);
            isFound = true;
        }
    }
}

void HeroBase::playPerk(string_view perkName) {
    // first finding the perk
    Perk currentPerk;
    for (auto p : this->perks) {
        if (p.name == perkName) { currentPerk = p; break; }
    }
}

void HeroBase::addItem(const Item& i) {
    this->items.push_back(i);
}