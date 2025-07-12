#include "gui.hpp"
using namespace std;

Gui::Gui(System *s):sys(s)
{
    
}


void Gui::run() {
   
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Horrified");
    SetTargetFPS(60);

    
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

    UnloadTexture(gameMap);
    UnloadFont(GameFont);

    CloseWindow();
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

    if (selectedPlace) {
        PlaceInfo(selectedPlace);
    }


}

void Gui::PlaceInfo(shared_ptr<Place> selected)
{
    vector<string> temp;
    for(auto& hero:selected->getAllHeroes())
    {
        temp.push_back(hero->getHeroName());
    }
    for(auto& mon:selected->getMonsters())
    {
        temp.push_back(mon->getMonsterName());
    }
    for(auto& vill:selected->getVillagers())
    {
        temp.push_back(vill->getName());
    }
    for(auto& item:selected->getItems())
    {
        temp.push_back(item.name);
    }  
    float panelWidth = 400;
    float panelHeight = 300;

        Rectangle panel = {(SCREEN_WIDTH - panelWidth) / 2.0f,(SCREEN_HEIGHT - panelHeight) / 2.0f,panelWidth,panelHeight};

        Rectangle shadow = panel;
        shadow.x += 4;
        shadow.y += 4;
        DrawRectangleRec(shadow, Color{ 0, 0, 0, 100 }); 
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Color{ 0, 0, 0, 100 });




    int startY = panel.y + 40;
        int lineHeight = 20;
        for (size_t i = 0; i < temp.size(); ++i) 
        {
            std::string line = std::to_string(i + 1) + ". " + temp[i];
            Vector2 textPos = { panel.x + 20, startY + i * lineHeight };
            DrawTextEx(GameFont, line.c_str(), textPos, 18.0f, 2.0f, WHITE);
        }
}

Gui::~Gui()
{
    
}