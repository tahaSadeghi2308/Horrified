#include "gui/guide_page.hpp"
#include "system.hpp"

using namespace std;

GuidePage::GuidePage(const Font &f, System *s)
: font(f) , sys(s) , option(-1) , selected(false)
{}

void GuidePage::movingAsset(shared_ptr<Place> &destination){
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

void GuidePage::update(shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {
    Vector2 mouse { GetMousePosition() };
    vector<shared_ptr<Place>> possiblePlace = cHero->getCurrentPlace()->getNeighbors();
    if(option == 1) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            for (auto& place : possiblePlace){
                if (place->isClicked(mouse)){
                    choosenPlace = place;
                }
            }
        }
        if(IsKeyPressed(KEY_BACKSPACE)){
            if(selectedVill){
                sys->moveVillager(selectedVill , cHero->getCurrentPlace());
                if(selectedVill->getSafeZone() == selectedVill->getVillagerLoc()){
                    sys->killVillager(selectedVill);
                    cHero->addPerkCard(sys->getRandomPerk());
                }
                selectedVill = nullptr;
                cPage = PageNumbers::HERO_PHASE_PAGE;
                actions--;
                option = -1;
            }
            else {
                choosenPlace = nullptr;
                option = -1;
            }
        }
    }
    else if (option == 2) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            for (auto& place : possiblePlace){
                if (place->isClicked(mouse)){
                    sys->moveVillager(selectedVill,place);
                    if(selectedVill->getSafeZone() == selectedVill->getVillagerLoc()){
                        sys->killVillager(selectedVill);
                        cHero->addPerkCard(sys->getRandomPerk());
                    }
                    cPage = PageNumbers::HERO_PHASE_PAGE;
                    actions--;
                    selectedVill = nullptr;
                    selected = false;
                    option = -1;
                }
            }
        }
        if(IsKeyPressed(KEY_BACKSPACE)){
            if(cHero->getCurrentPlace()->getVillagers().empty() || !selectedVill){
                cPage = PageNumbers::HERO_PHASE_PAGE;
                option = -1;
            }
            else
                selected = true;
        }
    }
    else {
        if(IsKeyPressed(KEY_BACKSPACE)){
            cPage = PageNumbers::HERO_PHASE_PAGE;
        }
    }
}

void GuidePage::draw(shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {
    Vector2 mouse { GetMousePosition() };
    vector<shared_ptr<Place>> possiblePlace = cHero->getCurrentPlace()->getNeighbors();
    if(option == 1){
        for (auto& place : possiblePlace) {
            movingAsset(place);
        }
        if(choosenPlace) {
            float panelW = 800, panelH = 600, pad = 20, Size = 170;
            Rectangle panel = {
                (SCREEN_WIDTH - panelW) / 2.0f,
                (SCREEN_HEIGHT - panelH) / 2.0f,
                panelW,
                panelH
            };
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
            DrawRectangleRec(panel, DARKGRAY);

            float x = panel.x + pad;
            float y = panel.y + pad;

            for(auto& vill : choosenPlace->getVillagers()){
                Texture2D villTex = vill->getAddress();
                Vector2 pos = { x,y };
                DrawTextureEx(villTex, pos , 0, Size/villTex.width, WHITE);

                Rectangle villRec = {
                    x, y,
                    villTex.width * (Size/villTex.width),
                    villTex.height * (Size/villTex.width)
                };

                if(CheckCollisionPointRec(mouse , villRec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    selectedVill = vill;
                }

                if(vill == selectedVill)
                    DrawRectangleLinesEx(villRec, 3, YELLOW);

                x += Size + pad;
                if (x + Size > panel.x + panel.width){
                    x = panel.x + pad;
                    y += Size + pad;
                }
            }
        }
    }
    else if(option == 2) {
        if(selectedVill && selected){
            for (auto& place : possiblePlace ) {
                movingAsset(place);
            }
        }
        else {
            float panelW = 800, panelH = 600, pad = 20, Size = 170;
            Rectangle panel = {(SCREEN_WIDTH - panelW) / 2.0f,(SCREEN_HEIGHT - panelH) / 2.0f,panelW, panelH};
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
            DrawRectangleRec(panel, DARKGRAY);

            float x = panel.x + pad;
            float y = panel.y + pad;

            auto villInPlace = cHero->getCurrentPlace()->getVillagers();

            for(auto& vill : villInPlace){
                Texture2D villTex = vill->getAddress();
                Vector2 pos = { x,y };
                DrawTextureEx(villTex, pos , 0, Size/villTex.width, WHITE);

                Rectangle villRec = {x, y,villTex.width * (Size/villTex.width),villTex.height * (Size/villTex.width)};

                if(CheckCollisionPointRec(mouse , villRec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    selectedVill = vill;
                }
                if(vill == selectedVill)
                    DrawRectangleLinesEx(villRec, 3, YELLOW);

                x += Size + pad;
                if (x + Size > panel.x + panel.width){
                    x = panel.x + pad;
                    y += Size + pad;
                }
            }
        }
    }
    else {
        float panelW = 840 , panelH = 100  , padding = 10;
        float fontSize = 35 ;
        // Rectangle panel = {(SCREEN_WIDTH - panelW) / 2 ,(SCREEN_HEIGHT - panelH) / 2 , panelW , panelH};
        float borderThickness = 2.0f; // Thin border

        Rectangle panel = {
                (SCREEN_WIDTH - panelW) / 2.0f,
                (SCREEN_HEIGHT - panelH) / 2.0f,
                panelW,
                panelH
            };
        
        Color backgroundColor = CLITERAL(Color){ 62, 95, 68, 255 };     // #3E5F44
        Color borderColor = CLITERAL(Color){ 232, 255, 215, 255 }; 
        
        Rectangle innerRect = {
            panel.x + borderThickness,
            panel.y + borderThickness,
            panel.width - 2 * borderThickness,
            panel.height - 2 * borderThickness
        };
        
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
        DrawRectangleRounded(panel, 0.15f, 0, backgroundColor);
        DrawRectangleRounded(innerRect, 0.15F, 0, borderColor);
        string bring = "You can bring a villager to your location.";
        string put = "You can put a villager from your place to a neighbor location.";
        // DrawRectangleRec(panel,DARKGRAY);
        Color bringHover = BLACK;
        Color putHover = BLACK;
        Vector2 bringPos = {panel.x + padding,panel.y + padding };
        Vector2 putPos = {panel.x + padding,panel.y + fontSize + 20};

        Rectangle bringRect = { bringPos.x, bringPos.y, panelW - 20, fontSize };
        Rectangle putRect = { putPos.x , putPos.y , panelW  , fontSize };
        if(CheckCollisionPointRec(mouse , bringRect)) {
            bringHover = YELLOW;
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                option = 1;
            }
        }
        else if(CheckCollisionPointRec(mouse , putRect)) {
            putHover = YELLOW;
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                option = 2;
            }
        }
        DrawTextEx(font, bring.c_str(), bringPos , fontSize , 0.0 ,bringHover);
        DrawTextEx(font, put.c_str(), putPos , fontSize , 0.0 ,putHover);
    }
}
