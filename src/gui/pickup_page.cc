#include "gui/pickup_page.hpp"
#include "system.hpp"

using namespace std;

PickupPage::PickupPage(const Font &f, System *s)
: font(f) , sys(s) , isPicked(false) , panelW(800) , panelH(600),
    Size(170) , pad(20)
{
    this->panel = {(SCREEN_WIDTH - panelW) / 2,(SCREEN_HEIGHT - panelH) / 2 ,panelW, panelH };
}

void PickupPage::draw(std::shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {
    Vector2 mouse { GetMousePosition() };
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
    DrawRectangleRec(panel, DARKGRAY);
    auto current = cHero->getCurrentPlace();
    float x = panel.x + pad;
    float y = panel.y + pad;
    for(auto item : current -> getItems()){
        Texture2D itemTex = item.address;
        Vector2 pos = { x,y};
        DrawTextureEx(itemTex,pos,0.0,Size/panel.width,WHITE);

        Rectangle itemRec = {
            x , y,
            itemTex.width * (Size/itemTex.width),
            itemTex.height * (Size/itemTex.width)
        };

        if (CheckCollisionPointRec(mouse, itemRec)){
            DrawRectangleLinesEx(itemRec, 2, YELLOW);
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                cHero->addHeroItems(item);
                cHero->getCurrentPlace()->removeItem(item);
                isPicked = true;
            }
        }

        x += Size + pad;
        if (x + Size > panel.x + panel.width){
            x = panel.x + pad;
            y += Size + pad;
        }
    }
}

void PickupPage::update(std::shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {
    if(IsKeyPressed(KEY_BACKSPACE)){
        cPage = PageNumbers::HERO_PHASE_PAGE;
        if(isPicked) {
            actions--;
            isPicked = false;
        }
    }
}