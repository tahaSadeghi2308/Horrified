#include "Hero.hpp"

using namespace std;

void HeroBase::addPerkCard(const Perk &perk) {
    this->heroPerks.push_back(perk);
}

HeroBase::HeroBase (
    const int &_num, 
    std::string_view _name,
    const Perk &perk
) : actionCount(_num) , heroName(_name) {
    this->addPerkCard(perk);
}

int HeroBase::getActionCount() { return this->actionCount; }

void HeroBase::setActionCount(const int &num) {
    // NOTE : We dont need to validate it because its valid :) 
    this->actionCount = num; 
}

std::string HeroBase::getHeroName() { return this->heroName; }

Archaeologist::Archaeologist(const int &_num , std::string_view _name , const Perk &perk) :
    HeroBase(_num , _name , perk) {}

Mayor::Mayor(const int &_num , std::string_view _name , const Perk &perk) :
    HeroBase(_num , _name , perk) {}

void HeroBase::setCurrentPlace(shared_ptr<Place> _place) {
    this->currentPlace = _place;
}

shared_ptr<Place> HeroBase::getCurrentPlace() { return this->currentPlace; }
