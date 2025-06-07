#include <gtest/gtest.h>
#include "../include/perk_deck.hpp"
#include "../include/item_bag.hpp"
#include "../include/monster_card_deck.hpp"

TEST(PerkDeckTest, ConstructorAndPickOneRandomly) {
    PerkDeck deck;
    Perk perk = deck.pickOneRandomly();
    
    EXPECT_FALSE(perk.name.empty());
    EXPECT_FALSE(perk.description.empty());
}

TEST(ItemBagTest, ConstructorAndPickOneRandom) {
    ItemBag bag;
    Item item = bag.pickOneRandom();
    
    EXPECT_FALSE(item.name.empty());
    EXPECT_FALSE(item.place.empty());
    EXPECT_GE(item.power, 1);
    EXPECT_LE(item.power, 6);
    EXPECT_TRUE(item.color == Color::RED || 
                item.color == Color::BLUE || 
                item.color == Color::YELLOW);
}

TEST(MonsterCardDeckTest, ConstructorAndPickOneRandomly) {
    MonsterCardDeck deck;
    MonsterCard card = deck.pickOneRandomly();
    
    EXPECT_FALSE(card.name.empty());
    EXPECT_GE(card.move, 1);
    EXPECT_GE(card.dice, 2);
    EXPECT_GE(card.itemCount, 0);
    EXPECT_FALSE(card.strikePriorities.empty());
}

TEST(DeckMultiplePicksTest, MultiplePicks) {
    PerkDeck perkDeck;
    std::vector<Perk> pickedPerks;
    for (int i = 0; i < 3; i++) {
        pickedPerks.push_back(perkDeck.pickOneRandomly());
    }
    
    for (size_t i = 0; i < pickedPerks.size(); i++) {
        for (size_t j = i + 1; j < pickedPerks.size(); j++) {
            EXPECT_FALSE(pickedPerks[i].name == pickedPerks[j].name);
        }
    }
    
    ItemBag itemBag;
    std::vector<Item> pickedItems;
    for (int i = 0; i < 3; i++) {
        pickedItems.push_back(itemBag.pickOneRandom());
    }
    
    for (size_t i = 0; i < pickedItems.size(); i++) {
        for (size_t j = i + 1; j < pickedItems.size(); j++) {
            EXPECT_FALSE(pickedItems[i].name == pickedItems[j].name);
        }
    }
    
    MonsterCardDeck monsterDeck;
    std::vector<MonsterCard> pickedCards;
    for (int i = 0; i < 3; i++) {
        pickedCards.push_back(monsterDeck.pickOneRandomly());
    }
    
    for (size_t i = 0; i < pickedCards.size(); i++) {
        for (size_t j = i + 1; j < pickedCards.size(); j++) {
            EXPECT_FALSE(pickedCards[i].name == pickedCards[j].name);
        }
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 