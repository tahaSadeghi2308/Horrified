#pragma once 
#include <memory>

class HeroBase;

class PageBase {
public:
        PageBase() = default;
        virtual void update(std::shared_ptr<HeroBase> &cHero ,int &actions) = 0;
        virtual void draw(std::shared_ptr<HeroBase> &cHero ,int &actions) = 0;
};