#ifndef PERK_DECK_HPP
#define PERK_DECK_HPP

#include <string>
#include <vector>

struct Perk {
    std::string name;
    std::string description;
};

class PerkDeck {
    std::vector<Perk> perks;
    void shufflePerks();
public:
    PerkDeck();
    Perk pickOneRandomly();
};

#endif // PERK_DECK_HPP