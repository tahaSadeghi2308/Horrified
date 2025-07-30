#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "raylib.h"

class HeroBase;
class Villager;
struct Item; 
class MonsterBase;

class Place {
    std::string name;
    std::vector<std::shared_ptr<Place>> neighbors;
    std::vector<std::shared_ptr<HeroBase>> heroes;
    std::vector<std::shared_ptr<Villager>> villagers;
    std::vector<Item> currentItems;
    std::vector<std::shared_ptr<MonsterBase>> monsters;
    Vector2 position;
    float radius = 40;

public:
    explicit Place(const std::string &_name);
    std::string getName();
    void setNeighbors(const std::vector<std::shared_ptr<Place>> neiList);
    std::vector<std::shared_ptr<Place>> getNeighbors();
    void addHero(std::shared_ptr<HeroBase> _hero);
    void deleteHero(const std::string &_heroName);
    void addVillager(std::shared_ptr<Villager> _vill);
    void deleteVillager(const std::string &_villName);
    std::vector<std::shared_ptr<Villager>> getVillagers();
    std::shared_ptr<HeroBase> getHeros(const std::string &_heroName);
    //about items
    void addItem(Item item);
    std::vector<Item>& getItems();
    void removeItem(Item );
    void addMonster(std::shared_ptr<MonsterBase> _monster);
    void deleteMonster(const std::string &_monsterName);
    std::vector<std::shared_ptr<MonsterBase>> getMonsters();
    std::vector<std::shared_ptr<HeroBase>> getAllHeroes() const;
    std::shared_ptr<MonsterBase> getOneMonster(const std::string &_monsterName);
    void setPosition(float ,float);
    bool isClicked(Vector2 mouse) const;
    void draw(Vector2 mouse) const;
    Vector2 getPosition() { return position; }
};

#endif