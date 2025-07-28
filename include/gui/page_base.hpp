#pragma once 
#include <memory>
#include "pageNumber.hpp"

class HeroBase;

constexpr int SCREEN_WIDTH { 1600 };
constexpr int SCREEN_HEIGHT { 900 };
class PageBase {
public:
        PageBase() = default;
        virtual void update(
                std::shared_ptr<HeroBase> &cHero,
                int &actions,
                PageNumbers &page
        ) = 0;
        virtual void draw(
                std::shared_ptr<HeroBase> &cHero,
                int &actions,
                PageNumbers &page
        ) = 0;
};