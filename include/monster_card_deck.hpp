#ifndef MONSTER_CARD_DECK_HPP
#define MONSTER_CARD_DECK_HPP

#include <string>
#include <vector>

struct MonsterCard {
    std::string name;
    int move , dice , itemCount;
    std::vector<std::string> strikePriorities;
};

class MonsterCardDeck {
    std::vector<MonsterCard> monsterCards;
    void shuffleCards();
public:
    MonsterCardDeck();
    MonsterCard pickOneRandomly();
};


#endif // MONSTER_CARD_DECK_HPP