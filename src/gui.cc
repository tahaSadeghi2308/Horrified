#include "gui.hpp"
using namespace std;

Gui::Gui(System *s,const int width,const int height):sys(s),scroll(0.0f),SCREEN_WIDTH(width),SCREEN_HEIGHT(height)
{
    
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

        DrawRectangle(SCREEN_WIDTH - RIGHT_PANEL_WIDTH, 0, RIGHT_PANEL_WIDTH, SCREEN_HEIGHT, DARKGRAY);
        DrawLine(SCREEN_WIDTH - RIGHT_PANEL_WIDTH, 0, SCREEN_WIDTH - RIGHT_PANEL_WIDTH, SCREEN_HEIGHT, GRAY);

        drawMap();
        handleInput();

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
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
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

Gui::~Gui()
{
    UnloadTexture(gameMap);
    UnloadFont(GameFont);

    CloseWindow();
}