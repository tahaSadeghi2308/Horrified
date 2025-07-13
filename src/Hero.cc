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
    this->last_played_perk_ = "Nothing";
}

int HeroBase::getActionCount() { return this->actionCount; }

void HeroBase::setActionCount(const int &num) {
    this->actionCount = num; 
}

std::string HeroBase::getHeroName() { return this->heroName; }

Archaeologist::Archaeologist(const int &_num , std::string_view _name , const Perk &perk) :
    HeroBase(_num , _name , perk) 
    {}

Mayor::Mayor(const int &_num , std::string_view _name , const Perk &perk) :
    HeroBase(_num , _name , perk) 
    {}

void HeroBase::setCurrentPlace(shared_ptr<Place> _place) {
    this->currentPlace = _place;
}

void HeroBase::deleteItem(std::string_view itemName) {
    bool isFound {};
    for (int i {}; i < this->heroItems.size(); i++) {
        if (heroItems[i].name == itemName) {
            this->heroItems.erase(heroItems.begin() + i);
        }
    }
}

shared_ptr<Place> HeroBase::getCurrentPlace() { return this->currentPlace; }

std::vector<Item>& HeroBase::getHeroItems() { return this->heroItems; }

void HeroBase::addHeroItems(Item _item)
{
    heroItems.push_back(_item);
}

void HeroBase::deletePerk(string_view perkName) {
    bool isFound {};
    for (int i {}; i < this->heroPerks.size() && !isFound; i++) {
        if (this->heroPerks[i].name == perkName){
            this->heroPerks.erase(heroPerks.begin() + i);
            isFound = true;
        }
    }
}