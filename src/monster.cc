#include "monster.hpp"
#include <queue>
#include "place.hpp"

using namespace std;

MonsterBase::MonsterBase(const string &_name , bool _isFrenzed , System *_sys) 
        : name(_name) , isFrenzed(_isFrenzed) , sys(_sys) {}

string MonsterBase::getMonsterName() const { return this->name; }

bool MonsterBase::getIsFrenzed() const { return this->isFrenzed; }

void MonsterBase::changeFrenzedState(bool newState){
    this->isFrenzed = newState;
}

void MonsterBase::setCurrentLocation(string_view _newLoc){
    this->currentPlace = _newLoc;
}

string MonsterBase::getCurrentLocation() const {
    return this->currentPlace;
}

Dracula::Dracula(const string &_name , bool _isFrenzed , System *_sys) 
        : MonsterBase(_name , _isFrenzed , _sys){}

InvisibleMan::InvisibleMan(const string &_name , bool _isFrenzed , System *_sys) 
        : MonsterBase(_name , _isFrenzed , _sys){}

void MonsterBase::putItems(const int count) {
    for (int i {}; i < count; i++){
        Item currentItem {sys->getRandomItem()};
        sys->putItemInPlace(currentItem.place , currentItem);
    }
}

void MonsterBase::doEvent(const string& _eventName){
    if (_eventName == "Sunrise"){
        sys->moveMonster("dracula" , "crypt");
    }
    else if (_eventName == "Thief"){
        sys->placeWithMaxItem();
    }
    else if (_eventName == "The_Delivery"){
        sys->moveVillager("wilbur&chick" , "docks");
    }
    else if (_eventName == "Fortune_Teller"){
        sys->moveVillager("maleva" , "camp");
    }
    else if (_eventName == "Former_Employer"){
        sys->moveVillager("dr.cranley" , "laboratory");
    }
    else if (_eventName == "Hurried_Assistant"){
        sys->moveVillager("fritz" , "tower");
    }
    else if (_eventName == "The_Innocent"){
        sys->moveVillager("maria" , "barn");
    }
    else if (_eventName == "Egyptian_Expert"){
        sys->moveVillager("prof.pearson" , "cave");
    }
    else if (_eventName == "The_Ichthyologist"){
        sys->moveVillager("dr.reed" , "institute");
    }
}

void MonsterBase::move(int n , string strike) 
{
    if (strike == "du")
    {
        int count = n;
        bool foundEnemy = false;
        while (count-- && !foundEnemy)
        {
            for (auto place : sys->getLocations())
            {
                if (place->getPlaceName() == this->currentPlace)
                {
                    if (!place->getAllVillagers().empty() || !place->getAllHeros().empty())
                    {
                        foundEnemy = true;
                    } 
                    else 
                    {
                        vector<string> path = sys->findPath(this->currentPlace , ETO);
                        sys->moveMonster("dracula" , path[1]);
                    }
                    break;
                }
            }
        }
    }

    else if (strike == "inm")
    {
        int count = n;
        bool foundEnemy = false;
        while (count-- && !foundEnemy)
        {
            for (auto place : sys->getLocations())
            {
                if (place->getPlaceName() == this->currentPlace)
                {
                    if (!place->getAllVillagers().empty() || !place->getAllHeros().empty())
                    {
                        foundEnemy = true;
                    } 
                    else 
                    {
                        vector<string> path = sys->findPath(this->currentPlace , ETO);
                        sys->moveMonster("invisibleMan" , path[1]);
                    }
                    break;
                }
            }
        }
    }
    else 
    {
        for(auto m : sys->getMonsters())
        {
            if (m->getIsFrenzed())
            {
                int count = n;
                bool foundEnemy = false;
                while (count-- && !foundEnemy)
                {
                    for (auto place : sys->getLocations())
                    {
                        if (place->getPlaceName() == this->currentPlace)
                        {
                            if (!place->getAllVillagers().empty() || !place->getAllHeros().empty())
                            {
                                foundEnemy = true;
                            } 
                            else 
                            {
                                vector<string> path = sys->findPath(this->currentPlace , ETO);
                                sys->moveMonster(m->getMonsterName() , path[1]);
                            }
                            break;
                        }
                    }
                }
            }
            break;
        }
    }
}

void MonsterBase::power(string_view monsterName , string_view cHero)
{
    if (monsterName == "dracula")
    {
        for (auto h : sys->getHeros())
        {
            if (h->getHeroName() == cHero)
            {
                if (h->getCurrentPlace() != this->getCurrentLocation())
                    sys->moveHero(cHero , this->getCurrentLocation());
                break;
            }
        }
        // sys->moveHero(cHero , this->getCurrentLocation());
    }
    else if (monsterName == "invisibleMan")
    {
        int count = 2;
        bool foundEnemy = false;
        while (count-- && !foundEnemy)
        {
            for (auto place : sys->getLocations())
            {
                if (place->getPlaceName() == this->currentPlace)
                {
                    if (!place->getAllVillagers().empty())
                    {
                        foundEnemy = true;
                    } 
                    else 
                    {
                        vector<string> path = sys->findPath(this->currentPlace , ETV);
                        if (!path.empty()) sys->moveMonster("invisibleMan" , path[1]);
                    }
                    break;
                }
            }
        }
    }
}

int MonsterBase::attack(char dice , string_view monsterName , string_view cHero)
{
    if (dice == '*')
    {
        shared_ptr<Place> p {nullptr};
        for(auto loc : sys->getLocations())
        {
            if (loc->getPlaceName() == this->getCurrentLocation())
            {
                p = loc;
                break;
            }
        }

        if (!p->getAllVillagers().empty())
        {
            sys->killVillager(p->getAllVillagers()[0]->getVillagerName());
            return 1; // for killing a viilager
        }
        else if (!p->getAllHeros().empty())
        {
            if (p->getAllHeros()[0]->getHeroName() == "arch") return 2; // for attacing to arch
            return 3; // for attact to mayor
        }
    }
    else if (dice == '!')
    {
        this->power(monsterName , cHero);
        return 4; // for running monster power
    }
    return -1; // for no attack or power
}

void MonsterBase::runMonsterPhase() {
    // get a random monster card 
    MonsterCard currentCard { sys->getRandomMonstCard() };
    currentCard.name = "Fortune_Teller";
    // put items 
    this->putItems(currentCard.itemCount);

    // run an event
    this->doEvent(currentCard.name);

    // moving 
    // for (auto st : currentCard.strikePriorities)
    // {
    //     this->move(currentCard.move , st);
    // }
    this->power( "dracula" , "arch");
}
