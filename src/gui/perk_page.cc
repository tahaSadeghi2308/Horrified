#include "gui/perk_page.hpp"
#include "system.hpp"

using namespace std;

PerkPage::PerkPage(const Font &f, System *s, bool doPhase)
: font(f) , sys(s) , count(0) , found(false) , doMonsterPhase(doPhase) , isThereVillager(false)
{}

void PerkPage::movingAsset(shared_ptr<Place>& destination) {
    Vector2 pos = destination -> getPosition();
    float w = 120, h = 120;
    Rectangle rect = {pos.x - w/2, pos.y - h/2, w, h}; // we should go back and up for 1/2 of the squere
    float expand = 7 * (sinf(GetTime() * 4 ) + 1); // get it with testing diffrent options 

    Rectangle fadingRec = {rect.x - expand , rect.y - expand , rect.width + 2 * expand , rect.height + 2 * expand};

    DrawRectangleLinesEx(fadingRec, 3, Fade(YELLOW, 0.4f));
    DrawRectangleLinesEx(rect, 2, YELLOW);
}

void PerkPage::update(std::shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {

}

void PerkPage::draw(std::shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {
    Vector2 mouse { GetMousePosition() };
    if(found)
    {
        if (selectedPerk == "Visit_from_the_Detective")
        {
           for(auto& loc : sys->getAllLocations())
           {
                Vector2 pos = loc->getPosition();
                float radius = 25.0f; 
                movingAsset(loc);
                if (CheckCollisionPointCircle(mouse, pos, radius))
                {
                    DrawCircleLines(pos.x, pos.y, radius + 4.0f, YELLOW);
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        for(auto& mon : sys->getAllMonsters())
                        {
                            if(mon->getMonsterName() == "invisibleMan")
                            {
                                sys->moveMonster(mon,loc);
                                found = false;
                                selectedPerk = "" ;
                                cHero->deletePerk("Visit_from_the_Detective");
                                cPage = PageNumbers::HERO_PHASE_PAGE;
                                break;
                            }
                        }
                    }
                }
            }
        }
        else if (selectedPerk == "Break_of_Dawn")
        {
            doMonsterPhase = false;
            for(int i {} ; i < 2; i++){
                Item temp = sys->getRandomItem();
                cHero->getCurrentPlace()->addItem(temp);
            }
            found = false;
            selectedPerk = "" ;
            cHero->deletePerk("Break_of_Dawn");
            cPage = PageNumbers::HERO_PHASE_PAGE;
        }
        else if (selectedPerk == "Overstock") {
            for (auto her : sys->getAllHeros()) {
                Item temp = sys->getRandomItem();
                if (her->getCurrentPlace())
                    sys->putItemInPlace(her->getCurrentPlace()->getName() , temp);
            }
            found = false;
            selectedPerk = "" ;
            cHero->deletePerk("Overstock");
            cPage = PageNumbers::HERO_PHASE_PAGE;
        }
        else if (selectedPerk == "Late_into_the_Night") {
            actions += 2;
            found = false;
            selectedPerk = "" ;
            cHero->deletePerk("Late_into_the_Night");
            cPage = PageNumbers::HERO_PHASE_PAGE; 
        }
        else if (selectedPerk == "Repel") 
        {
            // NOTE : bug potential here !!!
            static string nameMon = sys->getAllMonsters()[0] ->getMonsterName();

           if(count < 2)
            {
                for(auto& mon : sys->getAllMonsters())
                {    
                    if(nameMon == mon->getMonsterName()) {
                        float x = 0, y = 20 , pad = 20;
                        float panelH = 75;
                        Rectangle rec = {x,y,(float)SCREEN_WIDTH,panelH};
                        DrawRectangleRec(rec , BLACK);
                        string name = mon->getMonsterName();
                        Vector2 size = MeasureTextEx(font,name.c_str(),25,0);
                        DrawTextEx(font,name.c_str(),{((SCREEN_WIDTH-size.x)/2)+pad , y + pad },25,0,WHITE);
                        auto neis = mon->getCurrentLocation()->getNeighbors();
                        for (auto& place : neis)
                        {
                            movingAsset(place);
                        }

                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                        {
                            for (auto& place : neis)
                            {
                                if (place->isClicked(mouse))
                                {
                                    sys->moveMonster(mon, place);
                                    count++;   
                                }
                            }
                        }
                    }
                }        
            } 
            else
            {
                if(count >= 4)
                {
                    cPage = PageNumbers::HERO_PHASE_PAGE;
                    cHero->deletePerk("Repel");
                    found = false;
                    selectedPerk = "" ;
                    count = 0;
                    return;
                }
                for(auto& mon : sys->getAllMonsters()) {
                    if(mon->getMonsterName() != nameMon) {
                        float x = 0, y = 20 , pad = 20;
                        float panelH = 75;
                        Rectangle rec = {x,y,(float)SCREEN_WIDTH,panelH};
                        DrawRectangleRec(rec , BLACK);
                        string name = mon->getMonsterName();
                        Vector2 size = MeasureTextEx(font,name.c_str(),25,0);
                        DrawTextEx(font,name.c_str(),{((SCREEN_WIDTH-size.x)/2)+pad , y + pad },25,0,WHITE);
                        auto neis = mon->getCurrentLocation()->getNeighbors();

                        for (auto& place : neis)
                        {
                            movingAsset(place);
                        }

                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                        {
                            for (auto& place : neis)
                            {
                                if (place->isClicked(mouse))
                                {
                                    sys->moveMonster(mon, place);
                                    count++;   
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (selectedPerk == "Hurry") 
        {
            if(count < 2)
            {
                float x = 0, y = 20 , pad = 20;
                float panelH = 75;
                Rectangle rec = {x,y,(float)SCREEN_WIDTH,panelH};
                DrawRectangleRec(rec , BLACK);
                string name = cHero->getHeroName();
                Vector2 size = MeasureTextEx(font,name.c_str(),25,0);
                DrawTextEx(font,name.c_str(),{((SCREEN_WIDTH-size.x)/2)+pad , y + pad },25,0,WHITE);
                
                
                auto neis = cHero->getCurrentPlace()->getNeighbors();

                for (auto& place : neis)
                {
                    movingAsset(place);
                }

                if (!isThereVillager && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    for (auto& place : neis)
                    {
                        if (place->isClicked(mouse))
                        {
                            targetPlace = place;
                            auto villagers = cHero->getCurrentPlace()->getVillagers();
                            if (!villagers.empty())
                            {
                                isThereVillager = true;
                                selectedVillagers.clear();
                            }
                            else
                            {
                                sys->moveHero(cHero, targetPlace);
                                count++;
                                //pageNumber = PageNumbers::HERO_PHASE_PAGE;
                            }   
                        }
                    }
                }

                if (isThereVillager)
                {
                    float panelW = 800, panelH = 600, pad = 20, Size = 170;
                    Rectangle panel = {(SCREEN_WIDTH - panelW) / 2.0f,(SCREEN_HEIGHT - panelH) / 2.0f,panelW, panelH};
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
                    DrawRectangleRec(panel, DARKGRAY);

                    float x = panel.x + pad;
                    float y = panel.y + pad;
                    
                    for (auto& vill : cHero->getCurrentPlace()->getVillagers())
                    {
                        Texture2D villTex = vill->getAddress();
                        Vector2 pos = { x,y }; 
                        DrawTextureEx(villTex, pos , 0, Size/villTex.width, WHITE);

                        Rectangle villRec = {x, y,villTex.width * (Size/villTex.width),villTex.height * (Size/villTex.width)};

                        if (CheckCollisionPointRec(mouse, villRec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                        {
                            auto it = find(selectedVillagers.begin(),selectedVillagers.end(),vill);
                            if (it == selectedVillagers.end())
                                selectedVillagers.push_back(vill);
                            else
                                selectedVillagers.erase(it);
                        }
                        if (find(selectedVillagers.begin(),selectedVillagers.end(),vill) != selectedVillagers.end())
                        {
                            DrawRectangleLinesEx(villRec, 3, YELLOW);
                        }

                        x += Size + pad;
                        if (x + Size > panel.x + panel.width)
                        {
                            x = panel.x + pad;
                            y += Size + pad;
                        }
                    }

                    if (IsKeyPressed(KEY_BACKSPACE))
                    {
                        sys->moveHero(cHero, targetPlace);
                        count++;
                        for (auto& vill : selectedVillagers)
                        {
                            sys->moveVillager(vill, targetPlace);

                            if(vill->getSafeZone() == vill->getVillagerLoc())
                            {
                                sys->killVillager(vill);
                                cHero->addPerkCard(sys->getRandomPerk());
                            }
                        }
                        selectedVillagers.clear();
                        isThereVillager = false;
                        //pageNumber = PageNumbers::HERO_PHASE_PAGE;
                    } 
                }
            }
            else
            {
                if(count >= 4)
                {
                    cPage = PageNumbers::HERO_PHASE_PAGE;
                    cHero->deletePerk("Hurry");
                    found = false;
                    selectedPerk = "" ;
                    count = 0;
                    return;
                }
                
                for(auto& second : sys->getAllHeros())
                {
                
                    if(second->getHeroName() != cHero->getHeroName())
                    {
                    float x = 0, y = 20 , pad = 20;
                    float panelH = 75;
                    Rectangle rec = {x,y,(float)SCREEN_WIDTH,panelH};
                    DrawRectangleRec(rec , BLACK);
                    string name = second->getHeroName();
                    Vector2 size = MeasureTextEx(font,name.c_str(),25,0);
                    DrawTextEx(font,name.c_str(),{((SCREEN_WIDTH-size.x)/2)+pad , y + pad },25,0,WHITE);
                    

                    auto neis = second->getCurrentPlace()->getNeighbors();

                    for (auto& place : neis)
                    {
                        movingAsset(place);
                    }

                    if (!isThereVillager && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        for (auto& place : neis)
                        {
                            if (place->isClicked(mouse))
                            {
                                targetPlace = place;
                                auto villagers = second->getCurrentPlace()->getVillagers();
                                if (!villagers.empty())
                                {
                                    isThereVillager = true;
                                    selectedVillagers.clear();
                                }
                                else
                                {
                                    sys->moveHero(second, targetPlace);
                                    count++;
                                    //pageNumber = PageNumbers::HERO_PHASE_PAGE;
                                }   
                            }
                        }
                    }

                    if (isThereVillager)
                    {
                        float panelW = 800, panelH = 600, pad = 20, Size = 170;
                        Rectangle panel = {(SCREEN_WIDTH - panelW) / 2.0f,(SCREEN_HEIGHT - panelH) / 2.0f,panelW, panelH};
                        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
                        DrawRectangleRec(panel, DARKGRAY);

                        float x = panel.x + pad;
                        float y = panel.y + pad;
                        
                        for (auto& vill : second->getCurrentPlace()->getVillagers())
                        {
                            Texture2D villTex = vill->getAddress();
                            Vector2 pos = { x,y }; 
                            DrawTextureEx(villTex, pos , 0, Size/villTex.width, WHITE);

                            Rectangle villRec = {x, y,villTex.width * (Size/villTex.width),villTex.height * (Size/villTex.width)};

                            if (CheckCollisionPointRec(mouse, villRec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                            {
                                auto it = find(selectedVillagers.begin(),selectedVillagers.end(),vill);
                                if (it == selectedVillagers.end())
                                    selectedVillagers.push_back(vill);
                                else
                                    selectedVillagers.erase(it);
                            }
                            if (find(selectedVillagers.begin(),selectedVillagers.end(),vill) != selectedVillagers.end())
                            {
                                DrawRectangleLinesEx(villRec, 3, YELLOW);
                            }

                            x += Size + pad;
                            if (x + Size > panel.x + panel.width)
                            {
                                x = panel.x + pad;
                                y += Size + pad;
                            }
                        }

                        if (IsKeyPressed(KEY_BACKSPACE))
                        {
                            sys->moveHero(second, targetPlace);
                            count++;
                            for (auto& vill : selectedVillagers)
                            {
                                sys->moveVillager(vill, targetPlace);

                                if(vill->getSafeZone() == vill->getVillagerLoc())
                                {
                                    sys->killVillager(vill);
                                    second->addPerkCard(sys->getRandomPerk());
                                }
                            }
                            selectedVillagers.clear();
                            isThereVillager = false;
                            //pageNumber = PageNumbers::HERO_PHASE_PAGE;
                        } 
                    } 
                }   
            }
        }
    }
}
    else
    {
        float panelW = 900, panelH = 800, pad = 20, Size = 120;
        Rectangle panel = {(SCREEN_WIDTH - panelW) / 2.0f,(SCREEN_HEIGHT - panelH) / 2.0f,panelW, panelH};
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
        DrawRectangleRec(panel, DARKGRAY);
        
        float x = panel.x + pad;
        float y = panel.y + pad;

        for(auto& perk : cHero->getHeroPerks())
        {
            Texture2D png = perk.address;
            Vector2 pos {x,y};
            float scale = Size/panel.width;
            DrawTextureEx(png , pos,0,scale,WHITE);
            Rectangle pickPerk = {x,y,scale*png.width,scale*png.height};

            if(CheckCollisionPointRec(mouse,pickPerk))
            {
                DrawRectangleLinesEx(pickPerk,3,YELLOW);
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    found = true;
                    selectedPerk = perk.name;
                }
            }
            x += Size + pad;
            if(x + Size > panel.x + panel.width)
            {
                x = panel.x + pad;
                y += Size + pad;
            }
        }
        if(IsKeyPressed(KEY_BACKSPACE))
        {
            cPage = PageNumbers::HERO_PHASE_PAGE;
        }
    }
}