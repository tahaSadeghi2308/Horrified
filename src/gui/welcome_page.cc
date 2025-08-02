#include "gui/welcome_page.hpp"
#include "system.hpp"

using namespace std;

WelcomePage::WelcomePage() {
    bg = LoadTexture("../Horrified_Assets/menu_bg.jpg");
    font = LoadFont("../Horrified_Assets/menu_pages.ttf");
    
    float buttonWidth = 300;
    float buttonHeight = 80;
    float centerX = (SCREEN_WIDTH - buttonWidth) / 2.0f;
    float startY = SCREEN_HEIGHT * 0.4f;
    float spacing = 100;
    
    start = { centerX, startY, buttonWidth, buttonHeight };
    load = { centerX, startY + spacing, buttonWidth, buttonHeight };
    exit = { centerX, startY + 2 * spacing, buttonWidth, buttonHeight };
}

WelcomePage::~WelcomePage() {
    UnloadTexture(bg);
    UnloadFont(font);
}

void WelcomePage::update(std::shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {
    Vector2 mouse = GetMousePosition();
    
    if (CheckCollisionPointRec(mouse, start)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            cPage = PageNumbers::HERO_PHASE_PAGE;
        }
    }
    else if (CheckCollisionPointRec(mouse, load)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // TODO: Implement load game functionality
            // cPage = PageNumbers::LOAD_GAME_PAGE;
        }
    }
    else if (CheckCollisionPointRec(mouse, exit)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            cPage = PageNumbers::EXIT_PAGE;
        }
    }
}

void WelcomePage::draw(std::shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {
    Rectangle src = { 0, 0, (float)bg.width, (float)bg.height };
    Rectangle dest = { 0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(bg, src, dest, origin, 0, WHITE);
    

    Vector2 titleSizeVec = MeasureTextEx(font, "HORRIFIED", 80, 0);
    Vector2 titlePos = { (SCREEN_WIDTH - titleSizeVec.x) / 2.0f, 100 };
    DrawTextEx(font, "HORRIFIED", titlePos, 80, 0, WHITE);
    
    Vector2 mouse = GetMousePosition();
    
    Color startColor = CheckCollisionPointRec(mouse, start) ? YELLOW : WHITE;
    DrawRectangleRec(start, { 0, 0, 0, 150 });
    DrawRectangleLinesEx(start, 3, startColor);
    
    Vector2 startTextSize = MeasureTextEx(font, "START GAME", 40, 0);
    Vector2 startTextPos = { start.x + (start.width - startTextSize.x) / 2.0f, 
                            start.y + (start.height - startTextSize.y) / 2.0f };
    DrawTextEx(font, "START GAME", startTextPos, 40, 0, startColor);
    
    Color loadColor = CheckCollisionPointRec(mouse, load) ? YELLOW : WHITE;
    DrawRectangleRec(load, { 0, 0, 0, 150 });
    DrawRectangleLinesEx(load, 3, loadColor);
    
    Vector2 loadTextSize = MeasureTextEx(font, "LOAD GAME", 40, 0);
    Vector2 loadTextPos = { load.x + (load.width - loadTextSize.x) / 2.0f, 
                           load.y + (load.height - loadTextSize.y) / 2.0f };
    DrawTextEx(font, "LOAD GAME", loadTextPos, 40, 0, loadColor);
    
    Color exitColor = CheckCollisionPointRec(mouse, exit) ? YELLOW : WHITE;
    DrawRectangleRec(exit, { 0, 0, 0, 150 });
    DrawRectangleLinesEx(exit, 3, exitColor);
    
    Vector2 exitTextSize = MeasureTextEx(font, "EXIT", 40, 0);
    Vector2 exitTextPos = { exit.x + (exit.width - exitTextSize.x) / 2.0f, 
                           exit.y + (exit.height - exitTextSize.y) / 2.0f };
    DrawTextEx(font, "EXIT", exitTextPos, 40, 0, exitColor);
}