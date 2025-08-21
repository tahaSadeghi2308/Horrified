#ifndef MONSTER_PHASE
#define MONSTER_PHASE

#include <iostream>
#include "page_base.hpp"
#include <deque>
#include <monster.hpp>
#include "system.hpp"
#include "Hero.hpp"
#include "card_manager.hpp"


class MonsterPhasePage final : public PageBase
{
    Font GameFont;
    System* sys;

    MonsterCard currentCard;
    bool newCard {true};
    bool intialize {true}; 
    std::deque<std::pair<std::string, std::shared_ptr<MonsterBase>>> strikeMap;
    float time {};
    bool title {true};
    bool moveOnce {true};
    bool diceOnce {true};
    int status {-1};
    bool attackOnce {true};
    std::deque<char> dices;
    int option {};
    float diceTime {};
    bool powerANDattack {true};

    public:
    MonsterPhasePage(
        const Font &f,
        System *s
    );
    void Reset_For_Next_Phase();
    void draw(std::shared_ptr<HeroBase> &cHero ,int &actions , PageNumbers &cPage) override;
    void update(std::shared_ptr<HeroBase> &cHero ,int &actions ,PageNumbers &cPage) override;
};



#endif