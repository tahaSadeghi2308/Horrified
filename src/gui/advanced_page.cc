#include "gui/advanced_page.hpp"
#include "system.hpp"

using namespace std;

AdvancedPage::AdvancedPage(const Font &f, System *s)
: font(f) , sys(s) , option(-1) , showErr(false) ,
    time(0.F) , checkRedPower(true)
{}

void AdvancedPage::update(std::shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {

}

void AdvancedPage::draw(std::shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {
    if (option == 1) {

    }
    if(showErr) {
        if (GetTime() - time < 2.0) {
            float panelW = 500 , panelH = 75, pad = 20, Size = 250;
            Rectangle err = {0 , 20 , (float)SCREEN_WIDTH , panelH };
            DrawRectangleRec(err, RED);
            Vector2 textSize = MeasureTextEx(font,errText.c_str() , 25, 0 );
            DrawTextEx(font,errText.c_str() , {(SCREEN_WIDTH - textSize.x) / 2 + pad , err.y + pad} , 25, 0.0 , WHITE);
        }
        else {
            time = 0;
            showErr = false;
            errText = "";
        }
    }
}