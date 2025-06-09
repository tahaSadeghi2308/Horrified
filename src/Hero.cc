#include "Hero.hpp"
#include <iostream>
#include "location.hpp"


using namespace std;

void HeroBase::addPerkCard(const Perk &perk) {
    this->heroPerks.push_back(perk);
}

HeroBase::HeroBase(
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


//---move----

void HeroBase::moveAction()
{
    std::cout << "where do you want to go (select a number)\n";
    
            std::vector<std::shared_ptr<Place>> targetPlaces = getPlace()->getNeighbors();
    
            for(int i = 0;i < targetPlaces.size();i++)
            {    
                std::cout << i+1 << '-' << targetPlaces[i]->getName() << std::endl;
            }
            int choice;
            std::cin >> choice;
            if(choice < 1 || choice >targetPlaces.size() )
            {
                throw std::invalid_argument("the number that you have selected for moving is invalid\n");
            }



            for (auto& vill : getPlace()->getVillage())
            {
                getPlace()->rmVill(vill);              
                getPlace()->addVill(vill);          
                vill->changeLoc(targetPlaces[choice-1]);        
            }

            getPlace()->deleteHero(getHeroName());
            targetPlaces[choice-1]->addHero(std::make_shared<HeroBase>(*this));
            setPlace(targetPlaces[choice-1]);

            actionCount--; 
}


//----added by shahriar-----

std::shared_ptr<Place> HeroBase::getPlace()
{
    return loc;
}

void HeroBase::setPlace(std::shared_ptr<Place> newPlace)
{
    loc = newPlace;
}