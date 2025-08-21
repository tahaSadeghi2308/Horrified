#pragma once

#include "gui/page_base.hpp"
#include <raylib.h>

class HeroBase;
class System;
class Place;

constexpr int RIGHT_PANEL_WIDTH { 300 };
constexpr int LEFT_PANEL_WIDTH { 300 };
constexpr int UP_PANEL_WIDTH { SCREEN_WIDTH - LEFT_PANEL_WIDTH - RIGHT_PANEL_WIDTH };
constexpr int UP_PANEL_HEIGHT { 100 };

class HeroPhasePage final : public PageBase {
        System *sys;
        Font font;
        Texture2D gameMapImg , coffins , smahsedCoffins;
        float pad , panelH , panelW , scroll;
        float leftScroll;
        Rectangle moveRec;
        Rectangle PickRec;
        Rectangle GuidRec;
        Rectangle speciallRec;
        Rectangle AdvanceRec;
        Rectangle DefeatRec;
        Rectangle PerkRec;
        Rectangle exitANDsave;
        Rectangle Help;
        Rectangle exitOnly;
        Rectangle firstHero;
        Rectangle secondHero;
        Color buttonColor;
        std::shared_ptr<Place> selectedPlace;
public:
        HeroPhasePage(
                const Font &f,
                System *s
        );
        void draw(std::shared_ptr<HeroBase> &cHero ,int &actions , PageNumbers &cPage) override;
        void update(std::shared_ptr<HeroBase> &cHero ,int &actions ,PageNumbers &cPage) override;
        void drawRightPanel(std::shared_ptr<HeroBase>& hero );
        void drawUpPanel(std::shared_ptr<HeroBase>& heroInfo , int actions);
        void drawLeftPanel();
        void drawCoffin();
        void drawEvindence();
        void drawMap();
        void PlaceInfo(std::shared_ptr<Place> selected);
};