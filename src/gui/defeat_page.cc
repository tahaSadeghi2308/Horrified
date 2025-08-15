#include "gui/defeat_page.hpp"
#include "system.hpp"

using namespace std;

DefeatPage::DefeatPage(const Font &f, System *s)
: font(f) , sys(s) , option(-1) , showErr(false),
    time(0.F) , PowerCheck(true) , YellowPower(0) , redPower(0)
{}

void DefeatPage::update(std::shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {

}

void DefeatPage::draw(std::shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {
    Vector2 mouse { GetMousePosition() };
    if (option == 1) {
        if (sys->foundCluesCount("coffin") != 4) {
            showErr = true;
            time = GetTime();
            errText = "You didnt smashed all the coffins"; 
            option = -1;
        } else {
            bool isInSamePlace {false};
            for(auto monst : sys->getAllMonsters()){
                if (monst->getMonsterName() == "dracula"){
                    if (monst->getCurrentLocation() == cHero->getCurrentPlace()) {
                        isInSamePlace = true; 
                        break; 
                    }
                }
            }

            if(!isInSamePlace) {
                showErr = true;
                time = GetTime();
                errText = "Your not in the same place with Dracula"; 
                option = -1;
            } else {
                vector<Item> YellowItems;
                int itemsPowerSum = 0;
                for(auto i : cHero->getHeroItems()) {
                    if (i.color == card::Color::Y) { 
                        YellowItems.push_back(i); 
                        itemsPowerSum += i.power;
                        if(cHero->getHeroName() == "scientist") 
                            ++itemsPowerSum; 
                    }
                }
                if ( itemsPowerSum < 9 && PowerCheck ) {
                    showErr = true;
                    time = GetTime();
                    errText = "You dont have enough Yellow item to kill dracula"; 
                    option = -1;
                } else {
                    PowerCheck = false;
                    float panelW = 800, panelH = 600, pad = 20, Size = 170;
                    Rectangle panel = {(SCREEN_WIDTH - panelW) / 2,(SCREEN_HEIGHT - panelH) / 2,panelW, panelH};
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 0, 0, 0, 100 });
                    DrawRectangleRec(panel, DARKGRAY);
                    float x = panel.x + pad;
                    float y = panel.y + pad;
                      
                    for (auto& item : cHero->getHeroItems()) {
                        if (item.color != card::Color::Y) continue;

                        Texture2D itemTexure = item.address;
                        float scale = Size / itemTexure.width;
                        Rectangle rec = { x, y, itemTexure.width * scale, itemTexure.height * scale };
                        DrawTextureEx(itemTexure, { x, y }, 0, scale, WHITE);
            
                        if (CheckCollisionPointRec(mouse, rec)) {
                            DrawRectangleLinesEx(rec, 2, YELLOW);
                            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                                YellowPower += item.power;
                                if(cHero->getHeroName() == "scientist")
                                    ++YellowPower;

                                cHero->deleteItem(item.name);
                                sys->addItem(item);
                                              
                                if (YellowPower >= 9) {
                                    option = -1;
                                    cPage = PageNumbers::HERO_PHASE_PAGE;
                                    for(auto& mon : sys->getAllMonsters()) {
                                        if(mon->getMonsterName() == "dracula")
                                        sys->killMonster(mon);
                                    }
                                    YellowPower = 0;
                                    PowerCheck = true;
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
    }
    else if (option == 2) {
        if (sys->foundCluesCount("evidence") != 5) {
            showErr = true;
            time = GetTime();
            errText = "You didnt collect all the evidence"; 
            option = -1;
        } else {
            bool isInSamePlace {false};
            for(auto monst : sys->getAllMonsters()){
                if (monst->getMonsterName() == "invisibleMan"){
                    if (monst->getCurrentLocation() == cHero->getCurrentPlace()) {
                        isInSamePlace = true; 
                        break; 
                    }
                }
            }

            if(!isInSamePlace) {
                showErr = true;
                time = GetTime();
                errText = "Your not in the same place with InvisibleMan"; 
                option = -1;
            } else {
                vector<Item> redItems;
                int itemsPowerSum = 0;
                for(auto i : cHero->getHeroItems())  {
                    if (i.color == card::Color::R) { 
                        redItems.push_back(i); 
                        itemsPowerSum += i.power;  
                        if(cHero->getHeroName() == "scientist")
                            ++itemsPowerSum;
                    }
                }
                if ( itemsPowerSum < 9 && PowerCheck)  {
                    showErr = true;
                    time =GetTime();
                    errText = "You dont have enough red item to kill invisibleMan"; 
                    option = -1;
                } else {
                    PowerCheck = false;
                    float panelW = 800, panelH = 600, pad = 20, Size = 170;
                    Rectangle panel = {(SCREEN_WIDTH - panelW) / 2,(SCREEN_HEIGHT - panelH) / 2,panelW, panelH};
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 0, 0, 0, 100 });
                    DrawRectangleRec(panel, DARKGRAY);
                    float x = panel.x + pad;
                    float y = panel.y + pad;

                      
                    for (auto& item : cHero->getHeroItems())
                    {
                        if (item.color != card::Color::R) continue;

                        Texture2D itemTexure = item.address;
                        float scale = Size / itemTexure.width;
                        Rectangle rec = { x, y, itemTexure.width * scale, itemTexure.height * scale };
                        DrawTextureEx(itemTexure, { x, y }, 0, scale, WHITE);
            
                        if (CheckCollisionPointRec(mouse, rec)){
                            DrawRectangleLinesEx(rec, 2, YELLOW);
                            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                            {
                                redPower += item.power;
                                if(cHero->getHeroName() == "scientist")
                                    ++redPower;

                                cHero->deleteItem(item.name);
                                sys->addItem(item);              
                                if (redPower >= 9)
                                {
                                    option = -1;
                                    cPage = PageNumbers::HERO_PHASE_PAGE;
                                    for(auto& mon : sys->getAllMonsters())
                                    {
                                        if(mon->getMonsterName() == "invisibleMan")
                                        sys->killMonster(mon);
                                    }
                                    redPower = 0;
                                    PowerCheck = true;
                                    actions--;
                                    return;
                                }
                            }                     
                        }
                        x += Size + pad;
                        if (x + Size > panel.x + panel.width)
                        {
                            x = panel.x + pad;
                            y += Size + pad;
                        }
                    }
                }
            }
        }
    }
    else
    {
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

        for(auto& mon:sys->getAllMonsters())
        {
            Texture2D png = mon->getAddress();
            DrawTextureEx(png , { x , y } , 0.0 , Size / panel.width , WHITE);
            if(mon->getMonsterName() == "dracula")  { checkDracula = {x,y, png.width*scale - 50 , png.height*scale - smallingRec}; x += Size + (pad*5) ;}
            else { checkInvisible = {x,y, (png.width*scale) - smallingRec  , (png.height*scale) - smallingRec }; x += Size + (pad*5) ;}
        }

        if (CheckCollisionPointRec(mouse, checkDracula ) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            option = 1;
        else if (CheckCollisionPointRec(mouse, checkInvisible ) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
           option = 2;
        if(IsKeyPressed(KEY_BACKSPACE))
        {
            cPage = PageNumbers::HERO_PHASE_PAGE;
        }
    }
    if(showErr)
    {
        if (GetTime() - time < 2.0)
        {
            float panelW = 500 , panelH = 75, pad = 20, Size = 250;
            Rectangle err = {0 , 20 , (float)SCREEN_WIDTH , panelH };
            DrawRectangleRec(err, RED);
            Vector2 textSize = MeasureTextEx(font,errText.c_str() , 25, 0 );
            DrawTextEx(font,errText.c_str() , {(SCREEN_WIDTH - textSize.x) / 2 + pad , err.y + pad} , 25, 0.0 , WHITE);
        }
        else
        {
            time = 0;
            showErr = false;
            errText = "";
        }
    }
}