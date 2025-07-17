#include "gui.hpp"
using namespace std;

Gui::Gui(System *s,const int width,const int height):sys(s),scroll(0.0f),SCREEN_WIDTH(width),SCREEN_HEIGHT(height)
{
    pageNumber = PageNumbers::HERO_PHASE_PAGE;
    isEnd = -1;
    round = 0;
    this->playerPriority.push_back("archaeologist"); //for test needs welcom page
    this->playerPriority.push_back("mayor");
}


void Gui::run() {
   
    gameMap = LoadTexture("../../Horrified_Assets/map.png");
    GameFont = LoadFont("../../Horrified_Assets/Melted.ttf");

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
                // else if (this->pageNumber == PageNumbers::SPECIALACTION_PAGE) this->specialActionPage(currentHero , actions);
                // else if (this->pageNumber == PageNumbers::ADVANCED_PAGE) this->advancedPage(currentHero , actions);
                // else if (this->pageNumber == PageNumbers::DEFEAT_PAGE) this->defeatPage(currentHero , actions);
                // else if (this->pageNumber == PageNumbers::PLAYPERK_PAGE) this->playPerkPage(currentHero , actions , doNextPhase);
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
            for (auto& v : selectedVillagers)
                sys->moveVillager(v, targetPlace);

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
    static vector<Item> selectedItems;
    Vector2 mouse = GetMousePosition();

    float panelW = 800, panelH = 600, pad = 20, Size = 170;
    Rectangle panel = {(SCREEN_WIDTH - panelW) / 2.0f,(SCREEN_HEIGHT - panelH) / 2.0f,panelW, panelH};
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
    DrawRectangleRec(panel, DARKGRAY);

    float x = panel.x + pad;
    float y = panel.y + pad;

    auto current = hero->getCurrentPlace();

    for(auto item : current ->getItems())
    {
        Texture2D itemTex = item.address;
        Vector2 pos = { x,y};
        DrawTextureEx(itemTex,pos,0.0,Size/panel.width,WHITE);

        Rectangle itemRec = {x,y,itemTex.width * (Size/itemTex.width), itemTex.height * (Size/itemTex.width) }; 
        
        if (CheckCollisionPointRec(mouse, itemRec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
                auto it = find(selectedItems.begin(),selectedItems.end(),item);
                if (it == selectedItems.end())
                    selectedItems.push_back(item);
                else
                    selectedItems.erase(it);
        }

        if (find(selectedItems.begin(),selectedItems.end(),item) != selectedItems.end())
        {
            DrawRectangleLinesEx(itemRec, 3, YELLOW);
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
            if(!selectedItems.empty())
            {
                actions--;
                for(auto& rmItem : selectedItems)
                {
                    hero->addHeroItems(rmItem);
                    current->removeItem(rmItem);
                }
                selectedItems.clear();
            }
        }
}

void Gui::guidePhase(shared_ptr<HeroBase>& hero ,int &actions)
{
    Vector2 mouse =GetMousePosition();

    static int option = -1;
    static vector<shared_ptr<Villager>> allVill; 

    if(option == 1)
    {
        float panelW = 800, panelH = 600, pad = 20, Size = 170;
        Rectangle panel = {(SCREEN_WIDTH - panelW) / 2.0f,(SCREEN_HEIGHT - panelH) / 2.0f,panelW, panelH};
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0,0,0,100});
        DrawRectangleRec(panel, DARKGRAY);
    
        float x = panel.x + pad;
        float y = panel.y + pad;
    
        

    }
    else if(option ==2)
    {

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
    }
}


Gui::~Gui()
{
    UnloadTexture(gameMap);
    UnloadFont(GameFont);

    CloseWindow();
}