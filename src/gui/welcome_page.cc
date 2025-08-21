#include "gui/welcome_page.hpp"
#include "system.hpp"

using namespace std;

WelcomePage::WelcomePage() : showBusyPopup(false), busyPopupStart(0.0) {
    bg = LoadTexture("../Horrified_Assets/menu_bg.jpg");
    font = LoadFont("../Horrified_Assets/menu_pages.ttf");
    
    float buttonWidth = 300;
    float buttonHeight = 80;
    float centerX = (SCREEN_WIDTH - buttonWidth) / 2.0f;
    float startY = SCREEN_HEIGHT * 0.4f;
    float spacing = 90;
    
    start = { centerX, startY, buttonWidth, buttonHeight };
    resume = { centerX, startY + spacing, buttonWidth, buttonHeight };
    load = { centerX, startY + 2 * spacing, buttonWidth, buttonHeight };
    exit = { centerX, startY + 3 * spacing, buttonWidth, buttonHeight };
}

WelcomePage::~WelcomePage() {
    UnloadTexture(bg);
    UnloadFont(font);
}

void WelcomePage::update(std::shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {
    Vector2 mouse = GetMousePosition();
    bool hasGame = (cHero != nullptr);

    // Recompute layout dynamically to avoid gaps when resume is hidden
    float buttonWidth = 300;
    float buttonHeight = 80;
    float centerX = (SCREEN_WIDTH - buttonWidth) / 2.0f;
    float startY = SCREEN_HEIGHT * 0.4f;
    float spacing = 90;

    start = { centerX, startY, buttonWidth, buttonHeight };
    if (hasGame) {
        resume = { centerX, startY + spacing, buttonWidth, buttonHeight };
        load   = { centerX, startY + 2 * spacing, buttonWidth, buttonHeight };
        exit   = { centerX, startY + 3 * spacing, buttonWidth, buttonHeight };
    } else {
        load   = { centerX, startY + spacing, buttonWidth, buttonHeight };
        exit   = { centerX, startY + 2 * spacing, buttonWidth, buttonHeight };
    }
    
    if (CheckCollisionPointRec(mouse, start)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (hasGame) {
                showBusyPopup = true;
                busyPopupStart = GetTime();
            } else {
                cPage = PageNumbers::PLAYER_SETUP_PAGE;
            }
        }
    }
    else if (hasGame && CheckCollisionPointRec(mouse, resume)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            cPage = PageNumbers::HERO_PHASE_PAGE;
        }
    }
    else if (CheckCollisionPointRec(mouse, load)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (hasGame) {
                showBusyPopup = true;
                busyPopupStart = GetTime();
            } else {
                cPage = PageNumbers::LOAD_MATCH_PAGE;
            }
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
    bool hasGame = (cHero != nullptr);

    // Rectangles are updated in update; recompute here too in case draw is called first
    float buttonWidth = 300;
    float buttonHeight = 80;
    float centerX = (SCREEN_WIDTH - buttonWidth) / 2.0f;
    float startY = SCREEN_HEIGHT * 0.4f;
    float spacing = 90;
    start = { centerX, startY, buttonWidth, buttonHeight };
    if (hasGame) {
        resume = { centerX, startY + spacing, buttonWidth, buttonHeight };
        load   = { centerX, startY + 2 * spacing, buttonWidth, buttonHeight };
        exit   = { centerX, startY + 3 * spacing, buttonWidth, buttonHeight };
    } else {
        load   = { centerX, startY + spacing, buttonWidth, buttonHeight };
        exit   = { centerX, startY + 2 * spacing, buttonWidth, buttonHeight };
    }
    
    Color startColor = CheckCollisionPointRec(mouse, start) ? YELLOW : WHITE;
    DrawRectangleRec(start, { 0, 0, 0, 150 });
    DrawRectangleLinesEx(start, 3, startColor);
    
    Vector2 startTextSize = MeasureTextEx(font, "START GAME", 40, 0);
    Vector2 startTextPos = { start.x + (start.width - startTextSize.x) / 2.0f, 
                            start.y + (start.height - startTextSize.y) / 2.0f };
    DrawTextEx(font, "START GAME", startTextPos, 40, 0, startColor);
    
    if (hasGame) {
        Color resumeColor = CheckCollisionPointRec(mouse, resume) ? YELLOW : WHITE;
        DrawRectangleRec(resume, { 0, 0, 0, 150 });
        DrawRectangleLinesEx(resume, 3, resumeColor);
        Vector2 resumeTextSize = MeasureTextEx(font, "RESUME GAME", 40, 0);
        Vector2 resumeTextPos = { resume.x + (resume.width - resumeTextSize.x) / 2.0f,
                               resume.y + (resume.height - resumeTextSize.y) / 2.0f };
        DrawTextEx(font, "RESUME GAME", resumeTextPos, 40, 0, resumeColor);
    }

    // Busy popup if user tries start/load while a game is in progress
    if (showBusyPopup) {
        double elapsed = GetTime() - busyPopupStart;
        if (elapsed < 3.0) {
            const char *msg = "You can only resume the current game.\nFinish or exit to start/load.";
            float panelW = 800, panelH = 140;
            Rectangle panel = { (SCREEN_WIDTH - panelW) / 2.0f, 40, panelW, panelH };
            DrawRectangleRec(panel, RED);
            Vector2 line1Size = MeasureTextEx(font, "You can only resume the current game.", 30, 0);
            Vector2 line2Size = MeasureTextEx(font, "Finish or exit to start/load.", 30, 0);
            Vector2 line1Pos = { panel.x + (panelW - line1Size.x) / 2.0f, panel.y + 25 };
            Vector2 line2Pos = { panel.x + (panelW - line2Size.x) / 2.0f, panel.y + 75 };
            DrawTextEx(font, "You can only resume the current game.", line1Pos, 30, 0, WHITE);
            DrawTextEx(font, "Finish or exit to start/load.", line2Pos, 30, 0, WHITE);
        } else {
            showBusyPopup = false;
            busyPopupStart = 0.0;
        }
    }

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