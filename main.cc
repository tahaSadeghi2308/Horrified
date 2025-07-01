#include "tui.hpp"
#include <iostream>
#include "system.hpp"
#include <fmt/core.h>

using namespace std;

int main(){
    System *s = new System();
    Tui t(s);
    t.runGame();
    delete s;
    // System s;
    // s.showLocs();
    // s.runGame();
    // fmt::println("\n-----------------------------------");
    // s.showLocs();

    // System game;
    // game.runGame();
    
    // game.showLocs();
    // game.runHeroPhase();
    
    // game.runMonsterPhase();
    // cout << "--------------------------------------------------------------\n";
    // game.showLocs();
    return 0;
}

// clearScreen();
//     unordered_set<string> uniqePerkNames;
//     unordered_map<int , string> test;

//     for (auto perk : hero->getHeroPerks()) uniqePerkNames.insert(perk.name);
//     fmt::println("First choose which perk u want to play");

//     string perkName;
//     while(true) 
//     {
//         int i {0} , perkNum;
//         for(auto perk : uniqePerkNames)
//         {
//             fmt::println("{}. {}" , i + 1 , perk);
//             test[i] = perk;
//             i++;
//         }
//         fmt::println("{}. Back" , i + 1);
//         fmt::println("{}. Exit" , i + 2);
//         perkNum = getCommand("Enter your perk number");
//         if (perkNum > 0 && perkNum <= i){
//             perkName = test[i - 1];
//             hero->deletePerk(perkName);
//             break;
//         }
//         else if (perkNum == i + 1) { this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return; }
//         else if (perkNum == i + 2) { this->pageNumber = PageNumbers::EXIT_PAGE; return; }
//         else fmt::println("Wrong perk or option entered!!");
//     }
//     if (perkName == "Visit_from_the_Detective")
//     {
//         int n;
//         clearScreen();
//         fmt::println("you choosed Visit_from_the_Detective");
//         fmt::println("enter your place to put invisible man");
//         while(true) 
//         {
//             for(int i {}; i < sys->getLocations().size(); i++){
//                 fmt::println("{}. {}" , i+1, ((sys->getLocations())[i])->getName());
//             }
//             fmt::println("{}. Back" , sys->getLocations().size() + 1);
//             fmt::println("{}. Exit" , sys->getLocations().size() + 2);
//             n = getCommand("Your place is");
//             if (n > 0 && n <= sys->getLocations().size()){
//                 sys->moveMonster("invisibleMan" , sys->getLocations()[n - 1]->getName());
//                 this->pageNumber = PageNumbers::HERO_PHASE_PAGE; 
//                 return;
//             }
//             else if (n == sys->getLocations().size() + 1) { this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return; }
//             else if (n == sys->getLocations().size() + 2) { this->pageNumber = PageNumbers::EXIT_PAGE; return; }
//             else fmt::println("Invalid place entered!!");
//         }
//     }
//     else if (perkName == "Break_of_Dawn") {
//         doMonsterPhase = false;
//         for(int i {} ; i < 2; i++){
//             Item temp = sys->getRandomItem();
//             sys->putItemInPlace(temp.place , temp);
//         }
//         this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return;
//     }
//     else if (perkName == "Overstock"){
//         for(int i {} ; i < 2; i++){
//             Item temp = sys->getRandomItem();
//             sys->putItemInPlace(temp.place , temp);
//         }
//         this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return;
//     }
//     else if (perkName == "Late_into_the_Night") {
//         actions += 2;
//         this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return;
//     }
//     else if (perkName == "Repel") {
//         clearScreen();
//         fmt::println("Now we can move all heroes two times");
//         for (auto h : sys->getHeros()){
//             for (int j {}; j < 2; j++) {
//                 fmt::println("Move {} to ...." , h->getHeroName());
//                 string current = h->getCurrentPlace(); 
//                 vector<string> neis = (sys->getGameMap())[current];
//                 while (true){
//                     for(int i {}; i < neis.size(); i++){
//                         cout << i + 1 << ". " << neis[i] << '\n';
//                     }
//                     cout << neis.size() + 1 << ". Back" << '\n';
//                     cout << neis.size() + 2 << ". Exit" << '\n';
//                     int num = getCommand("Enter a number to choose where u want to go ");
    
//                     if (num > 0 && num <= neis.size()) {
//                         string x = h->getHeroName() , y = neis[num - 1];
//                         sys->moveHero(x , y);
//                         break;
//                     }
//                     else if (num == neis.size() + 1) { this->backButton(); return; }
//                     else if (num == neis.size() + 2) { this->pageNumber = PageNumbers::EXIT_PAGE; return; }
//                     else fmt::println("Invalid neighboer choies !!!");
//                 }
//             }
//         }
//         this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return;
//     }
//     else if (perkName == "Hurry") {
//         clearScreen();
//         fmt::println("Now we can move all heroes two times");
//         for (auto m : sys->getMonsters()){
//             for (int j {}; j < 2; j++) {
//                 fmt::println("Move {} to ...." , m->getMonsterName());
//                 string current = m->getCurrentLocation(); 
//                 vector<string> neis = (sys->getGameMap())[current];
//                 while (true){
//                     for(int i {}; i < neis.size(); i++){
//                         cout << i + 1 << ". " << neis[i] << '\n';
//                     }
//                     cout << neis.size() + 1 << ". Back" << '\n';
//                     cout << neis.size() + 2 << ". Exit" << '\n';
//                     int num = getCommand("Enter a number to choose where u want to go ");
    
//                     if (num > 0 && num <= neis.size()) {
//                         sys->moveMonster(m->getMonsterName() , neis[num - 1]);
//                         break;
//                     }
//                     else if (num == neis.size() + 1) { this->backButton(); return; }
//                     else if (num == neis.size() + 2) { this->pageNumber = PageNumbers::EXIT_PAGE; return; }
//                     else fmt::println("Invalid neighboer choies !!!");
//                 }
//             }
//         }
//         this->pageNumber = PageNumbers::HERO_PHASE_PAGE; return;
//     }