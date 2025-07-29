#include "gui/move_page.hpp"
#include "system.hpp"

using namespace std;

MovePage::MovePage(
    const Font &f,
    System *s
): font(f) , sys(s) , isThereVillager(false)
{}

void MovePage::movingAsset(shared_ptr<Place> &destination){
    Vector2 pos = destination -> getPosition();
    float w = 120, h = 120;
    Rectangle rect = {pos.x - w/2, pos.y - h/2, w, h}; // we should go back and up for 1/2 of the squere
    float expand = 7 * (sinf(GetTime() * 4 ) + 1); // get it with testing diffrent options 

    Rectangle fadingRec = {
        rect.x - expand,
        rect.y - expand, 
        rect.width + 2 * expand,
        rect.height + 2 * expand
    };

    DrawRectangleLinesEx(fadingRec, 3, Fade(YELLOW, 0.4f));
    DrawRectangleLinesEx(rect, 2, YELLOW);
}

void MovePage::draw(shared_ptr<HeroBase> &cHero ,int &actions , PageNumbers &cPage) {
    auto neis = cHero->getCurrentPlace()->getNeighbors();
    Vector2 mouse { GetMousePosition() };
    for (auto& place : neis){
        movingAsset(place);
    }
    if (isThereVillager){
        float panelW = 800, panelH = 600, pad = 20, Size = 170;
        Rectangle panel = {
            (SCREEN_WIDTH - panelW) / 2.0f,
            (SCREEN_HEIGHT - panelH) / 2.0f,
            panelW, panelH
        };
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
        DrawRectangleRec(panel, DARKGRAY);

        float x = panel.x + pad;
        float y = panel.y + pad;
        
        for (auto& vill : cHero->getCurrentPlace()->getVillagers()){
            Texture2D villTex = vill->getAddress();
            Vector2 pos = { x,y }; 
            DrawTextureEx(villTex, pos , 0, Size/villTex.width, WHITE);

            Rectangle villRec = { 
                x,y,
                villTex.width * (Size/villTex.width),
                villTex.height * (Size/villTex.width)
            };

            if (CheckCollisionPointRec(mouse, villRec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                auto it = find(selectedVillagers.begin(),selectedVillagers.end(),vill);
                if (it == selectedVillagers.end())
                    selectedVillagers.push_back(vill);
                else
                    selectedVillagers.erase(it);
            }
            if (find(selectedVillagers.begin(),selectedVillagers.end(),vill) != selectedVillagers.end()){
                DrawRectangleLinesEx(villRec, 3, YELLOW);
            }

            x += Size + pad;
            if (x + Size > panel.x + panel.width){
                x = panel.x + pad;
                y += Size + pad;
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE)){
            sys->moveHero(cHero, targetPlace);
            for (auto& vill : selectedVillagers){
                sys->moveVillager(vill, targetPlace);
                if(vill->getSafeZone() == vill->getVillagerLoc()){
                    sys->killVillager(vill);
                    cHero->addPerkCard(sys->getRandomPerk());
                }
            }
            selectedVillagers.clear();
            isThereVillager = false;
            actions--;
            cPage = PageNumbers::HERO_PHASE_PAGE;
        }
        return; 
    }
}

void MovePage::update(shared_ptr<HeroBase> &cHero ,int &actions , PageNumbers &cPage){
    Vector2 mouse { GetMousePosition() };
    if (!isThereVillager && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        auto neis = cHero->getCurrentPlace()->getNeighbors();
        for (auto& place : neis){
            if (place->isClicked(mouse)){
                targetPlace = place;
                auto villagers = cHero->getCurrentPlace()->getVillagers();
                if (!villagers.empty()){
                    isThereVillager = true;
                    selectedVillagers.clear();
                }
                else {
                    sys->moveHero(cHero, targetPlace);
                    actions--;
                    cPage = PageNumbers::HERO_PHASE_PAGE;
                    return;
                }   
            }
        }
    }

    if (IsKeyPressed(KEY_BACKSPACE)){
        cPage = PageNumbers::HERO_PHASE_PAGE;
    }
}