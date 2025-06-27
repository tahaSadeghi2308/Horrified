#include "hero.hpp"

using namespace std;

HeroBase::HeroBase(string_view _name) : name(_name) {}

string HeroBase::getHeroName() const { return this->name; }

string HeroBase::getCurrentPlace() const {
    return this->currentPlace;
}

void HeroBase::setCurrentPlace(string_view _newPlace) {
    this->currentPlace = _newPlace;
}
