#include "gui/advanced_page.hpp"
#include "system.hpp"

using namespace std;

AdvancedPage::AdvancedPage(const Font &f, System *s)
: font(f) , sys(s) , option(-1) , showErr(false) ,
    time(0.F) , checkRedPower(true)
{}

void AdvancedPage::update(std::shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {}

void AdvancedPage::draw(std::shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {
    Vector2 mouse { GetMousePosition() };
    
    if (option == 1) {
        bool isCoffin {false};
        for(auto coffin : sys ->getCoffins()){
            if (cHero->getCurrentPlace()->getName() == coffin) isCoffin = true;
        }

        if(!isCoffin) {
            showErr = true;
            time = GetTime();
            errText = "There is no coffin in your current place"; 
            if(sys->getCoffins().empty())
                errText = "He has been defeated";
            option = -1;
        } else {
            vector<Item> redItems;
            int itemsPowerSum = 0;
            for(auto i : cHero->getHeroItems()) {
                if (i.color == card::Color::R) { 
                    redItems.push_back(i); 
                    itemsPowerSum += i.power;

                    if(cHero->getHeroName() == "scientist")
                        ++itemsPowerSum;  
                }
            }
            if ( itemsPowerSum < 6 && checkRedPower ) {
                showErr = true;
                time = GetTime();
                errText = "You dont have enough item to advance dracula"; 
                option = -1;
            } else {
                checkRedPower = false ;
                float panelW = 800, panelH = 600, pad = 20, Size = 170;
                Rectangle panel = {(SCREEN_WIDTH - panelW) / 2,(SCREEN_HEIGHT - panelH) / 2,panelW, panelH};
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 0, 0, 0, 100 });
                DrawRectangleRec(panel, DARKGRAY);
                float x = panel.x + pad;
                float y = panel.y + pad;

                // static int redPower = 0;
                    
                for (auto& item : cHero->getHeroItems()) {
                    if (item.color != card::Color::R) continue;

                    Texture2D itemTexure = item.address;
                    float scale = Size / itemTexure.width;
                    Rectangle rec = { x, y, itemTexure.width * scale, itemTexure.height * scale };
                    DrawTextureEx(itemTexure, { x, y }, 0, scale, WHITE);
        
                    if (CheckCollisionPointRec(mouse, rec)) {
                        DrawRectangleLinesEx(rec, 2, YELLOW);
                        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                            redPower += item.power;
                            if(cHero->getHeroName() == "scientist")
                                ++redPower;

                            cHero->deleteItem(item.name);
                            sys->addItem(item);
                                            
                            if (redPower >= 6) {
                                option = -1;
                                cPage = PageNumbers::HERO_PHASE_PAGE;
                                sys->destroyClue("coffin", cHero->getCurrentPlace()->getName());
                                redPower = 0;
                                checkRedPower = true;
                                actions--;
                                return;
                            }
                        }                     
                    }
                    x += Size + pad;
                    if (x + Size > panel.x + panel.width) {
                        x = panel.x + pad;
                        y += Size + pad;
                    }
                }
            }
        }
    }
    else if (option == 2) {
        if(cHero->getCurrentPlace()->getName() == "precinct") {
            bool check{true};
            vector<Item> validItems;
            for(auto& evi:sys->getEvidence()) {
                for(auto& item: cHero->getHeroItems()){
                    if(item.place == evi){
                        check = false;
                        validItems.push_back(item);
                    }
                }
            }
        
            if(check){
                showErr = true;
                time = GetTime();
                errText = "You dont have a item that came from ";
                for(auto& left : sys->getEvidence())
                    errText += left + ',';

                if(sys->getEvidence().empty())
                    errText = "He has been defeated";

                option = -1;
            }
            else {
                float panelW = 800, panelH = 600, pad = 20, Size = 170;
                Rectangle panel = {(SCREEN_WIDTH - panelW) / 2,(SCREEN_HEIGHT - panelH) / 2,panelW, panelH};
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 0, 0, 0, 100 });
                DrawRectangleRec(panel, DARKGRAY);
                float x = panel.x + pad;
                float y = panel.y + pad;
                    
                for (auto& item : validItems) {
                    Texture2D itemTexture = item.address;
                    float scale = Size / itemTexture.width;
                    Rectangle rec = { x, y, itemTexture.width * scale, itemTexture.height * scale };
                    DrawTextureEx(itemTexture, { x, y }, 0, scale, WHITE);
        
                    if (CheckCollisionPointRec(mouse, rec)) {
                        DrawRectangleLinesEx(rec, 2, YELLOW);
                        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                            cHero->deleteItem(item.name);
                            sys->addItem(item);
                            sys->destroyClue("evidence", item.place );
                            actions--;              
                            option = -1;
                            cPage = PageNumbers::HERO_PHASE_PAGE;
                            break;
                        }                     
                    }
                    x += Size + pad;
                    if (x + Size > panel.x + panel.width) {
                        x = panel.x + pad;
                        y += Size + pad;
                    }
                }
            }    
        } else {
            showErr = true;
            time = GetTime();
            errText = "Your Not in precinct";
            option = -1;
        }
    } else {
        float panelW = 800, panelH = 600, pad = 20, Size = 250;
        Rectangle panel = {(SCREEN_WIDTH - panelW) / 2.0f,(SCREEN_HEIGHT - panelH) / 2.0f,panelW, panelH};
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
        DrawRectangleRec(panel, DARKGRAY);
        Vector2 textSize = MeasureTextEx(font,"Which one you want to advance" , 25, 0.0);
        DrawTextEx(font,"Which one you want to advance" , { panel.x + ((panel.width - textSize.x )/ 2)  , panel.y + pad} , 25 , 0.0 , BLACK );

        float x = panel.x + (pad * 4);
        float y = panel.y + (pad * 4);

        float scale = Size/panel.width;

        Rectangle checkInvisible;
        Rectangle checkDracula;

        int smallingRec = 50;

        for(auto& mon : sys->getAllMonsters()) {
            Texture2D png = mon->getAddress();
            DrawTextureEx(png , { x , y } , 0.0 , Size / panel.width , WHITE);
            if(mon->getMonsterName() == "dracula")  { checkDracula = {x,y, png.width*scale - 50 , png.height*scale - smallingRec}; x += Size + (pad*5) ;}
            else { checkInvisible = {x,y, (png.width*scale) - smallingRec  , (png.height*scale) - smallingRec }; x += Size + (pad*5) ;}
        }

        if (CheckCollisionPointRec(mouse, checkDracula ) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            option = 1;
        else if (CheckCollisionPointRec(mouse, checkInvisible ) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
           option = 2;
        
        if (IsKeyPressed(KEY_BACKSPACE)){
            cPage = PageNumbers::HERO_PHASE_PAGE;
        }
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