#include "gui.hpp"
using namespace std;

Gui::Gui(System *s,const int width,const int height):sys(s),scroll(0.0f),SCREEN_WIDTH(width),SCREEN_HEIGHT(height)
{
    pageNumber = PageNumbers::HERO_PHASE_PAGE;
    isEnd = -1;
    round = 0;
    gameMap = LoadTexture("../../Horrified_Assets/map.png");
    GameFont = LoadFont("../../Horrified_Assets/Melted.ttf");
    sys->setFont(GameFont);
    this->playerPriority.push_back("archaeologist"); //for test needs welcom page
    this->playerPriority.push_back("mayor");
}


void Gui::run() {
    Rectangle Src = { 0, 0, (float)gameMap.width, (float)gameMap.height };
    Rectangle Dest = {0, 0,(float)(SCREEN_WIDTH - RIGHT_PANEL_WIDTH), (float)SCREEN_HEIGHT };
    Vector2 origin = { 0, 0 };



    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);


        DrawTexturePro(gameMap, Src, Dest, origin, 0.0f, WHITE);

        DrawRectangle(SCREEN_WIDTH - RIGHT_PANEL_WIDTH, 0, RIGHT_PANEL_WIDTH, SCREEN_HEIGHT, WHITE);
        DrawLine(SCREEN_WIDTH - RIGHT_PANEL_WIDTH, 0, SCREEN_WIDTH - RIGHT_PANEL_WIDTH, SCREEN_HEIGHT, GRAY);

        handleInput();
        if (currentHero == nullptr) {
            string name = playerPriority[round % playerPriority.size()];
            for (auto& h : sys->getAllHeros()) {
                if (h->getHeroName() == name) { currentHero = h; break; }
            }
            actions = currentHero->getActionCount();
            doNextPhase = true;
            pageNumber = PageNumbers::HERO_PHASE_PAGE;
        }
        isEnd = sys->isEndGame();
            if (this->pageNumber != PageNumbers::HERO_PHASE_PAGE)
            {
                if (this->pageNumber == PageNumbers::MOVE_PAGE) this->MovePhase(currentHero , actions);
                else if (this->pageNumber == PageNumbers::GUIDE_PAGE) this->guidePhase(currentHero , actions);
                else if (this->pageNumber == PageNumbers::PICKUP_PAGE) this->pickUpPhase(currentHero , actions);
                else if (this->pageNumber == PageNumbers::SPECIALACTION_PAGE) currentHero -> speciallAction(sys,pageNumber,actions,SCREEN_WIDTH,SCREEN_HEIGHT);
                else if (this->pageNumber == PageNumbers::ADVANCED_PAGE) this->advancedPhase(currentHero , actions);
                else if (this->pageNumber == PageNumbers::DEFEAT_PAGE) this->defeatPhase(currentHero , actions);
                else if (this->pageNumber == PageNumbers::PLAYPERK_PAGE) this->playPerkPhase(currentHero , actions , doNextPhase);
                // else if (this->pageNumber == PageNumbers::HELP_PAGE) this->helpPage();
            }
        if (actions <= 0 && doNextPhase && isEnd == -1) {
            //monsterPhasePage(currentHero);
            currentHero = nullptr;
            round++;
        }

        EndDrawing();
    }

    
}

void Gui::drawMap() {
    Vector2 mouse = GetMousePosition();

    for (auto &p : sys->getAllLocations()) 
    {
        p->draw(mouse);
    }
}



void Gui::handleInput()
{
    if(pageNumber == PageNumbers::HERO_PHASE_PAGE)
    {
        drawMap();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ) {
            Vector2 mouse = GetMousePosition();
            for (auto &p : sys->getAllLocations()) 
            {
                if (p->isClicked(mouse)) 
                {
                    selectedPlace = p;
                    break;
                }
            }
        }    
        if (selectedPlace) 
            {
                PlaceInfo(selectedPlace);
                if(GetKeyPressed() == KEY_BACKSPACE)
                {
                    selectedPlace = nullptr;
                    scroll = 0;
                }
            }
    }
    if(GetKeyPressed() == KEY_M)
    {
        pageNumber = PageNumbers::MOVE_PAGE;
    }
    if (IsKeyPressed(KEY_P))
    {
        pageNumber = PageNumbers::PICKUP_PAGE;
    }
    if(IsKeyPressed(KEY_G))
    {
        pageNumber = PageNumbers::GUIDE_PAGE;
    }
    if(IsKeyPressed(KEY_A))
    {
        pageNumber = PageNumbers::ADVANCED_PAGE;
    }
    if(IsKeyPressed(KEY_D))
    {
        pageNumber = PageNumbers::DEFEAT_PAGE;
    }
    if(IsKeyPressed(KEY_S))
    {
        pageNumber = PageNumbers::SPECIALACTION_PAGE;
    }

}


void Gui::PlaceInfo(shared_ptr<Place> selected)
{

    vector<Texture2D> temp;

    for(auto& hero : selected->getAllHeroes())
    {
        temp.push_back(hero->getAddress());
    }  
    for(auto& mon:selected->getMonsters())
    {
        temp.push_back(mon->getAddress());
    }
    for(auto& vill : selected->getVillagers())
    {
        temp.push_back(vill->getAddress());
    }  
    for(auto& item:selected->getItems())
    {
       temp.push_back(item.address);
    }  
    // creating panel
    float panelWidth = 800;
    float panelHeight = 600;
    Rectangle panel = { (SCREEN_WIDTH - panelWidth) / 2.0f, (SCREEN_HEIGHT - panelHeight) / 2.0f, panelWidth, panelHeight };
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Color{ 0, 0, 0, 100 }); // could change
    DrawRectangleRec(panel, DARKGRAY);

    // all sizes that we need
    float Size = 170.0f;
    float padding = 20.0f;
    float rowHeight = Size + 2 * padding;
    int itemsNum = (panelWidth - 2 * padding) / rowHeight; // two padding for left and right 
    int Rows = (temp.size() + itemsNum - 1) / itemsNum; // ceil doesnt work so i used manual ceil(if we go to next line we need the next line)
    float ContentHeight = Rows * rowHeight;
    float showingHeight = panel.height - 60.0f; 
    //add scrolling
    float scrollSpeed = 20.0f;
    scroll += GetMouseWheelMove() * scrollSpeed;
    float maxScroll = 0.0f;
    float minScroll = 0.0f;
    if (ContentHeight > showingHeight)
        minScroll = showingHeight - ContentHeight; 

    if (scroll > maxScroll) 
    scroll = maxScroll;
    if (scroll < minScroll) 
    scroll = minScroll;

    float startX = panel.x + 20.0f;
    float startY = panel.y + 40.0f + scroll; 
    float saveX = startX;
    float saveY = startY;

    BeginScissorMode(panel.x, panel.y, panel.width, panel.height); 

    for (size_t i = 0; i < temp.size(); ++i)
    {
        if (saveX + Size > panel.x + panel.width)
        {
            saveX = startX;
            saveY += rowHeight;
        }
        Vector2 pos = { saveX, saveY };
        DrawTextureEx(temp[i], pos, 0.0f, Size / (float)(temp[i]).width, WHITE);
        saveX += Size + padding;
    }
    EndScissorMode();
}
    
void Gui::MovePhase(std::shared_ptr<HeroBase>& hero, int &actions)
{
    static bool isThereVillager = false;
    static std::shared_ptr<Place> targetPlace;
    static std::vector<std::shared_ptr<Villager>> selectedVillagers;
    Vector2 mouse = GetMousePosition();

    auto neis = hero->getCurrentPlace()->getNeighbors();

    for (auto& place : neis)
    {
        Vector2 pos = place->getPosition();
        float radius = 25.0f; // now just for test 
        DrawCircleV(pos, radius, BLUE);
        if (CheckCollisionPointCircle(mouse, pos, radius))
            DrawCircleLines(pos.x, pos.y, radius + 4.0f, YELLOW);
    }

    if (!isThereVillager && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        for (auto& place : neis)
        {
            if (place->isClicked(mouse))
            {
                targetPlace = place;
                auto villagers = hero->getCurrentPlace()->getVillagers();
                if (!villagers.empty())
                {
                    isThereVillager = true;
                    selectedVillagers.clear();
                }
                else
                {
                    sys->moveHero(hero, targetPlace);
                    actions--;
                    pageNumber = PageNumbers::HERO_PHASE_PAGE;
                    return;
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
        
        for (auto& vill : hero->getCurrentPlace()->getVillagers())
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
            sys->moveHero(hero, targetPlace);
            for (auto& vill : selectedVillagers)
            {
                sys->moveVillager(vill, targetPlace);

                if(vill->getSafeZone() == vill->getVillagerLoc())
                {
                    sys->killVillager(vill);
                    hero->addPerkCard(sys->getRandomPerk());
                }
            }
            selectedVillagers.clear();
            isThereVillager = false;
            actions--;
            pageNumber = PageNumbers::HERO_PHASE_PAGE;
        }

        return; 
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        pageNumber = PageNumbers::HERO_PHASE_PAGE;
    }
}

void Gui::pickUpPhase(shared_ptr<HeroBase>& hero ,int &actions)
{
    static bool isPicked = false;
    Vector2 mouse = GetMousePosition();

    float panelW = 800, panelH = 600, pad = 20, Size = 170;
    Rectangle panel = {(SCREEN_WIDTH - panelW) / 2,(SCREEN_HEIGHT - panelH) / 2 ,panelW, panelH};
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
    DrawRectangleRec(panel, DARKGRAY);

    float x = panel.x + pad;
    float y = panel.y + pad;

    auto current = hero->getCurrentPlace();

    for(auto item : current -> getItems())
    {
        Texture2D itemTex = item.address;
        Vector2 pos = { x,y};
        DrawTextureEx(itemTex,pos,0.0,Size/panel.width,WHITE);

        Rectangle itemRec = {x,y,itemTex.width * (Size/itemTex.width), itemTex.height * (Size/itemTex.width) }; 
        
        if (CheckCollisionPointRec(mouse, itemRec))
        {
            DrawRectangleLinesEx(itemRec, 2, YELLOW);
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                hero->addHeroItems(item);
                hero->getCurrentPlace()->removeItem(item);
                isPicked = true;
            }
        }

        x += Size + pad;
        if (x + Size > panel.x + panel.width)
        {
            x = panel.x + pad;
            y += Size + pad;
        }
    }


    if(IsKeyPressed(KEY_BACKSPACE))
        {
            pageNumber = PageNumbers::HERO_PHASE_PAGE;
            if(isPicked)
            {
                actions--;
                isPicked = false;
            }
        }
}

void Gui::guidePhase(shared_ptr<HeroBase>& hero ,int &actions)
{
    Vector2 mouse =GetMousePosition();
    static int option = -1;
    static shared_ptr<Villager> selectedVill; 
    static shared_ptr<Place> choosenPlace;

    if(option == 1)
    {
        vector<shared_ptr<Place>> possiblePlace = hero ->getCurrentPlace()-> getNeighbors();   
        
        for (auto& place : possiblePlace)
        {
            Vector2 pos = place->getPosition();
            float radius = 25.0f; // now just for test 
            DrawCircleV(pos, radius, BLUE);
            if (CheckCollisionPointCircle(mouse, pos, radius))
            DrawCircleLines(pos.x, pos.y, radius + 4.0f, YELLOW);
        }
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            for (auto& place : possiblePlace)
            {
                if (place->isClicked(mouse))
                {
                    choosenPlace = place;
                }
            }
        }
        if(choosenPlace)
        {
            float panelW = 800, panelH = 600, pad = 20, Size = 170;
            Rectangle panel = {(SCREEN_WIDTH - panelW) / 2.0f,(SCREEN_HEIGHT - panelH) / 2.0f,panelW, panelH};
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
            DrawRectangleRec(panel, DARKGRAY);
        
            float x = panel.x + pad;
            float y = panel.y + pad;

            for(auto& vill : choosenPlace->getVillagers())
            {
                Texture2D villTex = vill->getAddress();
                Vector2 pos = { x,y }; 
                DrawTextureEx(villTex, pos , 0, Size/villTex.width, WHITE);

                Rectangle villRec = {x, y,villTex.width * (Size/villTex.width),villTex.height * (Size/villTex.width)};

                if(CheckCollisionPointRec(mouse , villRec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    selectedVill = vill;
                }

                if(vill == selectedVill)
                DrawRectangleLinesEx(villRec, 3, YELLOW);

                x += Size + pad;
                if (x + Size > panel.x + panel.width)
                {
                    x = panel.x + pad;
                    y += Size + pad;
                }
            }
        }
        if(IsKeyPressed(KEY_BACKSPACE))
        {
            if(selectedVill)
            {
                sys->moveVillager(selectedVill , hero->getCurrentPlace());
                if(selectedVill->getSafeZone() == selectedVill->getVillagerLoc())
                {
                    sys->killVillager(selectedVill);
                    hero->addPerkCard(sys->getRandomPerk());
                }
                selectedVill = nullptr; 
                pageNumber = PageNumbers::HERO_PHASE_PAGE;
                actions--;
                option = -1;
            }
            else
            {
                choosenPlace = nullptr;
                option = -1;
            }
        }
    }
    else if(option == 2)
    {
        static bool selected = false;
        vector<shared_ptr<Place>> possiblePlace = hero ->getCurrentPlace()-> getNeighbors();
        if(selectedVill && selected)
        {
            for (auto& place : possiblePlace )
            {
                Vector2 pos = place->getPosition();
                float radius = 25.0f; // now just for test 
                DrawCircleV(pos, radius, BLUE);
                if (CheckCollisionPointCircle(mouse, pos, radius))
                DrawCircleLines(pos.x, pos.y, radius + 4.0f, YELLOW);
            }
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                for (auto& place : possiblePlace)
                {
                    if (place->isClicked(mouse))
                    {
                        sys->moveVillager(selectedVill,place);
                        if(selectedVill->getSafeZone() == selectedVill->getVillagerLoc())
                        {
                            sys->killVillager(selectedVill);
                            hero->addPerkCard(sys->getRandomPerk());
                        }
                        pageNumber = PageNumbers::HERO_PHASE_PAGE;
                        actions--;
                        selectedVill = nullptr;
                        selected = false;
                        option = -1;
                    }
                }
            }
        }
        else
        {
            float panelW = 800, panelH = 600, pad = 20, Size = 170;
            Rectangle panel = {(SCREEN_WIDTH - panelW) / 2.0f,(SCREEN_HEIGHT - panelH) / 2.0f,panelW, panelH};
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
            DrawRectangleRec(panel, DARKGRAY);
            
            float x = panel.x + pad;
            float y = panel.y + pad;

            auto villInPlace = hero->getCurrentPlace()->getVillagers();

            for(auto& vill : villInPlace)
            {
                Texture2D villTex = vill->getAddress();
                Vector2 pos = { x,y }; 
                DrawTextureEx(villTex, pos , 0, Size/villTex.width, WHITE);

                Rectangle villRec = {x, y,villTex.width * (Size/villTex.width),villTex.height * (Size/villTex.width)};

                if(CheckCollisionPointRec(mouse , villRec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    selectedVill = vill;
                }

                if(vill == selectedVill)
                DrawRectangleLinesEx(villRec, 3, YELLOW);

                x += Size + pad;
                if (x + Size > panel.x + panel.width)
                {
                    x = panel.x + pad;
                    y += Size + pad;
                }
            }
                if(IsKeyPressed(KEY_BACKSPACE))
                {
                    if(hero->getCurrentPlace()->getVillagers().empty() || !selectedVill)
                    {
                        pageNumber = PageNumbers::HERO_PHASE_PAGE;
                        option = -1;
                    }
                    else
                        selected = true;
                }
            
        }
    }
    else
    {
        float panelW = 840 , panelH = 100  , padding = 10;
        float fontSize = 35 ;
        Rectangle panel = {(SCREEN_WIDTH - panelW) / 2 ,(SCREEN_HEIGHT - panelH) / 2 , panelW , panelH};
        string bring = "You can bring a villager to your location.";
        string put = "You can put a villager from your place to a neighbor location.";
        DrawRectangleRec(panel,DARKGRAY);
        Color bringHover = BLACK;
        Color putHover = BLACK;
        Vector2 bringPos = {panel.x + padding,panel.y + padding };
        Vector2 putPos = {panel.x + padding,panel.y + fontSize + 20};
        
        Rectangle bringRect = { bringPos.x, bringPos.y, panelW - 20, fontSize };
        Rectangle putRect = { putPos.x , putPos.y , panelW  , fontSize };
        if(CheckCollisionPointRec(mouse , bringRect))
        {
            bringHover = YELLOW;
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                option = 1;
            }
        }
        else if(CheckCollisionPointRec(mouse , putRect))
        {
            putHover = YELLOW;
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                option = 2;
            }
        }

        DrawTextEx(GameFont, bring.c_str(), bringPos , fontSize , 0.0 ,bringHover);
        DrawTextEx(GameFont, put.c_str(), putPos , fontSize , 0.0 ,putHover);

        if(IsKeyPressed(KEY_BACKSPACE))
        {
            pageNumber = PageNumbers::HERO_PHASE_PAGE;
        }
    }
}

void Gui::advancedPhase(std::shared_ptr<HeroBase>& hero,int &actions)
{
    Vector2 mouse = GetMousePosition();
    static int option = -1;
    static bool showErr = false;
    static float time = 0.0;
    static string errText;
    if(option == 1)
    {
        bool isCoffin {false};

        for(auto coffin : sys ->getCoffins())
            if (hero->getCurrentPlace()->getName() == coffin) isCoffin = true;
            if(!isCoffin)
            {
                showErr = true;
                time = GetTime();
                errText = "There is no coffin in your current place"; 
                if(sys->getCoffins().empty())
                    errText = "He has been defeated";
                option = -1;
            }

            else 
            {
                vector<Item> redItems;
                int itemsPowerSum = 0;
                for(auto i : hero->getHeroItems()) 
                {
                    if (i.color == card::Color::R) 
                    { 
                        redItems.push_back(i); 
                        itemsPowerSum += i.power;  
                    }
                }
                if ( itemsPowerSum < 6 ) 
                {
                    showErr = true;
                    time =GetTime();
                    errText = "You dont have enough item to advance dracula"; 
                    option = -1;
                }
                else 
                {
                    float panelW = 800, panelH = 600, pad = 20, Size = 170;
                    Rectangle panel = {(SCREEN_WIDTH - panelW) / 2,(SCREEN_HEIGHT - panelH) / 2,panelW, panelH};
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 0, 0, 0, 100 });
                    DrawRectangleRec(panel, DARKGRAY);
                    float x = panel.x + pad;
                    float y = panel.y + pad;

                    static int redPower = 0;
                      
                    for (auto& item : hero->getHeroItems())
                    {
                        if (item.color != card::Color::R) continue;

                        Texture2D itemTexure = item.address;
                        float scale = Size / itemTexure.width;
                        Rectangle rec = { x, y, itemTexure.width * scale, itemTexure.height * scale };
                        DrawTextureEx(itemTexure, { x, y }, 0, scale, WHITE);
            
                        if (CheckCollisionPointRec(mouse, rec))
                        {
                            DrawRectangleLinesEx(rec, 2, YELLOW);
                            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                            {
                                redPower += item.power;
                                hero->deleteItem(item.name);
                                sys->addItem(item);
                                actions--;              
                                if (redPower >= 6)
                                {
                                    option = -1;
                                    pageNumber = PageNumbers::HERO_PHASE_PAGE;
                                    sys->destroyClue("coffin", hero->getCurrentPlace()->getName());
                                    redPower = 0;
                                }
                                break;
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
    else if(option == 2)
    {
        if(hero->getCurrentPlace()->getName() == "precinct")
        {
            bool check{true};
            vector<Item> validItems;
            for(auto& evi:sys->getEvidence())
            {
                for(auto& item:hero->getHeroItems())
                {
                    if(item.place == evi)
                    {
                        check=false;
                        validItems.push_back(item);
                    }
                }
            }
        
            if(check)
            {
                showErr = true;
                time = GetTime();
                errText = "You dont have a item that came from ";
                for(auto& left :sys->getEvidence())
                    errText += left + ',';
                if(sys->getEvidence().empty())
                    errText = "He has been defeated";
                option = -1;
            }
            else
            {
                float panelW = 800, panelH = 600, pad = 20, Size = 170;
                    Rectangle panel = {(SCREEN_WIDTH - panelW) / 2,(SCREEN_HEIGHT - panelH) / 2,panelW, panelH};
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 0, 0, 0, 100 });
                    DrawRectangleRec(panel, DARKGRAY);
                    float x = panel.x + pad;
                    float y = panel.y + pad;
                      
                    for (auto& item : validItems)
                    {
                        Texture2D itemTexture = item.address;
                        float scale = Size / itemTexture.width;
                        Rectangle rec = { x, y, itemTexture.width * scale, itemTexture.height * scale };
                        DrawTextureEx(itemTexture, { x, y }, 0, scale, WHITE);
            
                        if (CheckCollisionPointRec(mouse, rec))
                        {
                            DrawRectangleLinesEx(rec, 2, YELLOW);
                            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                            {
                                hero->deleteItem(item.name);
                                sys->addItem(item);
                                sys->destroyClue("evidence", item.place );
                                actions--;              
                                option = -1;
                                pageNumber = PageNumbers::HERO_PHASE_PAGE;
                                break;
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
        else
        {
            showErr = true;
            time = GetTime();
            errText = "Your Not in precinct";
            option = -1;
        }
    
    }

    else
    {
        float panelW = 800, panelH = 600, pad = 20, Size = 250;
        Rectangle panel = {(SCREEN_WIDTH - panelW) / 2.0f,(SCREEN_HEIGHT - panelH) / 2.0f,panelW, panelH};
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
        DrawRectangleRec(panel, DARKGRAY);
        Vector2 textSize = MeasureTextEx(GameFont,"Which one you want to advance" , 25, 0.0);
        DrawTextEx(GameFont,"Which one you want to advance" , { panel.x + ((panel.width - textSize.x )/ 2)  , panel.y + pad} , 25 , 0.0 , BLACK );

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
            pageNumber = PageNumbers::HERO_PHASE_PAGE;
        }
    }
    if(showErr)
    {
        if (GetTime() - time < 2.0)
        {
            float panelW = 500 , panelH = 75, pad = 20, Size = 250;
            Rectangle err = {0 , 20 , (float)SCREEN_WIDTH , panelH };
            DrawRectangleRec(err, RED);
            Vector2 textSize = MeasureTextEx(GameFont,errText.c_str() , 25, 0 );
            DrawTextEx(GameFont,errText.c_str() , {(SCREEN_WIDTH - textSize.x) / 2 + pad , err.y + pad} , 25, 0.0 , WHITE);
        }
        else
        {
            time = 0;
            showErr = false;
            errText = "";
        }
    }
}

void Gui::defeatPhase(std::shared_ptr<HeroBase>& hero , int &actions)
{
    static int option = -1;
    Vector2 mouse = GetMousePosition();
    static bool showErr = false;
    static float time = 0.0;
    static string errText;
    if(option == 1)
    {
        if (sys->foundCluesCount("coffin") != 4)
        {
            showErr = true;
            time = GetTime();
            errText = "You didnt smashed all the coffins"; 
            option = -1;
        }
        else
        {
            bool isInSamePlace {false};
            for(auto monst : sys->getAllMonsters()){
                if (monst->getMonsterName() == "dracula"){
                    if (monst->getCurrentLocation() == hero->getCurrentPlace()) { isInSamePlace = true; break; }
                }
            }

            if(!isInSamePlace)
            {
                showErr = true;
                time = GetTime();
                errText = "Your not in the same place with Dracula"; 
                option = -1;
            }
            else 
            {
                vector<Item> YellowItems;
                int itemsPowerSum = 0;
                for(auto i : hero->getHeroItems()) 
                {
                    if (i.color == card::Color::Y) 
                    { 
                        YellowItems.push_back(i); 
                        itemsPowerSum += i.power;  
                    }
                }
                if ( itemsPowerSum < 9 ) 
                {
                    showErr = true;
                    time =GetTime();
                    errText = "You dont have enough Yellow item to kill dracula"; 
                    option = -1;
                }
                else 
                {
                    float panelW = 800, panelH = 600, pad = 20, Size = 170;
                    Rectangle panel = {(SCREEN_WIDTH - panelW) / 2,(SCREEN_HEIGHT - panelH) / 2,panelW, panelH};
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 0, 0, 0, 100 });
                    DrawRectangleRec(panel, DARKGRAY);
                    float x = panel.x + pad;
                    float y = panel.y + pad;

                    static int YellowPower = 0;
                      
                    for (auto& item : hero->getHeroItems())
                    {
                        if (item.color != card::Color::Y) continue;

                        Texture2D itemTexure = item.address;
                        float scale = Size / itemTexure.width;
                        Rectangle rec = { x, y, itemTexure.width * scale, itemTexure.height * scale };
                        DrawTextureEx(itemTexure, { x, y }, 0, scale, WHITE);
            
                        if (CheckCollisionPointRec(mouse, rec))
                        {
                            DrawRectangleLinesEx(rec, 2, YELLOW);
                            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                            {
                                YellowPower += item.power;
                                hero->deleteItem(item.name);
                                sys->addItem(item);
                                actions--;              
                                if (YellowPower >= 9)
                                {
                                    option = -1;
                                    pageNumber = PageNumbers::HERO_PHASE_PAGE;
                                    for(auto& mon : sys->getAllMonsters())
                                    {
                                        if(mon->getMonsterName() == "dracula")
                                        sys->killMonster(mon);
                                    }
                                    YellowPower = 0;
                                }
                                break;
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
    else if(option == 2)
    {
        if (sys->foundCluesCount("evidence") != 5)
        {
            showErr = true;
            time = GetTime();
            errText = "You didnt collect all the evidence"; 
            option = -1;
        }
        else
        {
            bool isInSamePlace {false};
            for(auto monst : sys->getAllMonsters()){
                if (monst->getMonsterName() == "invisibleMan"){
                    if (monst->getCurrentLocation() == hero->getCurrentPlace()) { isInSamePlace = true; break; }
                }
            }

            if(!isInSamePlace)
            {
                showErr = true;
                time = GetTime();
                errText = "Your not in the same place with InvisibleMan"; 
                option = -1;
            }
            else 
            {
                vector<Item> redItems;
                int itemsPowerSum = 0;
                for(auto i : hero->getHeroItems()) 
                {
                    if (i.color == card::Color::R) 
                    { 
                        redItems.push_back(i); 
                        itemsPowerSum += i.power;  
                    }
                }
                if ( itemsPowerSum < 9 ) 
                {
                    showErr = true;
                    time =GetTime();
                    errText = "You dont have enough red item to kill invisibleMan"; 
                    option = -1;
                }
                else 
                {
                    float panelW = 800, panelH = 600, pad = 20, Size = 170;
                    Rectangle panel = {(SCREEN_WIDTH - panelW) / 2,(SCREEN_HEIGHT - panelH) / 2,panelW, panelH};
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, { 0, 0, 0, 100 });
                    DrawRectangleRec(panel, DARKGRAY);
                    float x = panel.x + pad;
                    float y = panel.y + pad;

                    static int redPower = 0;
                      
                    for (auto& item : hero->getHeroItems())
                    {
                        if (item.color != card::Color::R) continue;

                        Texture2D itemTexure = item.address;
                        float scale = Size / itemTexure.width;
                        Rectangle rec = { x, y, itemTexure.width * scale, itemTexure.height * scale };
                        DrawTextureEx(itemTexure, { x, y }, 0, scale, WHITE);
            
                        if (CheckCollisionPointRec(mouse, rec))
                        {
                            DrawRectangleLinesEx(rec, 2, YELLOW);
                            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                            {
                                redPower += item.power;
                                hero->deleteItem(item.name);
                                sys->addItem(item);
                                actions--;              
                                if (redPower >= 9)
                                {
                                    option = -1;
                                    pageNumber = PageNumbers::HERO_PHASE_PAGE;
                                    for(auto& mon : sys->getAllMonsters())
                                    {
                                        if(mon->getMonsterName() == "invisibleMan")
                                        sys->killMonster(mon);
                                    }
                                    redPower = 0;
                                }
                                break;
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
        Vector2 textSize = MeasureTextEx(GameFont,"Which one you want to advance" , 25, 0.0);
        DrawTextEx(GameFont,"Which one you want to advance" , { panel.x + ((panel.width - textSize.x )/ 2)  , panel.y + pad} , 25 , 0.0 , BLACK );

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
            pageNumber = PageNumbers::HERO_PHASE_PAGE;
        }
    }
    if(showErr)
    {
        if (GetTime() - time < 2.0)
        {
            float panelW = 500 , panelH = 75, pad = 20, Size = 250;
            Rectangle err = {0 , 20 , (float)SCREEN_WIDTH , panelH };
            DrawRectangleRec(err, RED);
            Vector2 textSize = MeasureTextEx(GameFont,errText.c_str() , 25, 0 );
            DrawTextEx(GameFont,errText.c_str() , {(SCREEN_WIDTH - textSize.x) / 2 + pad , err.y + pad} , 25, 0.0 , WHITE);
        }
        else
        {
            time = 0;
            showErr = false;
            errText = "";
        }
    }
}

void Gui::playPerkPhase( std::shared_ptr<HeroBase>& hero , int &actions , bool &doMonsterPhase)
{
    static bool found = false;
    if(found)
    {

    }
    else
    {
        
    }
}


Gui::~Gui()
{
    UnloadTexture(gameMap);
    UnloadFont(GameFont);

    CloseWindow();
}