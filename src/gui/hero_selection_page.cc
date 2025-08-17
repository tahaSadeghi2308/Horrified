#include "gui/hero_selection_page.hpp"
#include "system.hpp"
#include <algorithm>

using namespace std;

HeroSelectionPage::HeroSelectionPage(vector<pair<string, int>> &fp, 
                                   vector<pair<string, string>> &mp, 
                                   System *system) 
    : firstPri(fp), mainPri(mp), sys(system) {
    bg = LoadTexture("../Horrified_Assets/menu_bg.jpg");
    font = LoadFont("../Horrified_Assets/menu_pages.ttf");
    
    float buttonWidth = 300;
    float buttonHeight = 80;
    float centerX = (SCREEN_WIDTH - buttonWidth) / 2.0f;
    float startY = SCREEN_HEIGHT * 0.8f;
    
    backButton = { centerX, startY, buttonWidth, buttonHeight };
    startGameButton = { centerX + buttonWidth + 50, startY, buttonWidth, buttonHeight };
    
    currentPlayerIndex = 0;
    selectedHero = "";
}

HeroSelectionPage::~HeroSelectionPage() {
    UnloadTexture(bg);
    UnloadFont(font);
}

void HeroSelectionPage::addPlayerHero() {
    if (!selectedHero.empty() && currentPlayerIndex < firstPri.size()) {
        string playerName = firstPri[currentPlayerIndex].first;
        mainPri.push_back({playerName, selectedHero});
        selectedHero = "";
        currentPlayerIndex++;
    }
}

void HeroSelectionPage::drawHeroOptions() {
    Vector2 mouse = GetMousePosition();
    auto heroes = sys->getAllHerosAvailable();
    
    float heroSize = 150;
    float spacing = 50;
    float totalWidth = heroes.size() * heroSize + (heroes.size() - 1) * spacing;
    float startX = (SCREEN_WIDTH - totalWidth) / 2.0f;
    float startY = SCREEN_HEIGHT * 0.4f;
    
    for (size_t i = 0; i < heroes.size(); ++i) {
        float x = startX + i * (heroSize + spacing);
        Rectangle heroRect = { x, startY, heroSize, heroSize };

        bool heroTaken = false;
        for (const auto& playerHero : mainPri) {
            if (playerHero.second == heroes[i]->getHeroName()) {
                heroTaken = true;
                break;
            }
        }
        
        Color borderColor;
        if (heroTaken) {
            borderColor = RED; // Hero already taken
        } else if (selectedHero == heroes[i]->getHeroName()) {
            borderColor = YELLOW; // Currently selected
        } else if (CheckCollisionPointRec(mouse, heroRect)) {
            borderColor = LIGHTGRAY; // Hover
        } else {
            borderColor = WHITE;
        }

        DrawRectangleRec(heroRect, { 0, 0, 0, 150 });
        DrawRectangleLinesEx(heroRect, 3, borderColor);

        Texture2D heroTexture = heroes[i]->getAddress();
        if (heroTexture.id != 0) {
            float scaleX = heroRect.width / heroTexture.width;
            float scaleY = heroRect.height / heroTexture.height;
            float scale = min(scaleX, scaleY) * 0.8f;

            float textureWidth = heroTexture.width * scale;
            float textureHeight = heroTexture.height * scale;
            float textureX = heroRect.x + (heroRect.width - textureWidth) / 2.0f;
            float textureY = heroRect.y + (heroRect.height - textureHeight) / 2.0f;
            
            DrawTextureEx(heroTexture, {textureX, textureY}, 0.0f, scale, WHITE);
        }

        string heroName = heroes[i]->getHeroName();
        Vector2 textSize = MeasureTextEx(font, heroName.c_str(), 25, 0);
        Vector2 textPos = { x + (heroSize - textSize.x) / 2.0f, startY + heroSize + 10 };
        DrawTextEx(font, heroName.c_str(), textPos, 25, 0, borderColor);

        if (heroTaken) {
            Vector2 takenSize = MeasureTextEx(font, "TAKEN", 20, 0);
            Vector2 takenPos = { x + (heroSize - takenSize.x) / 2.0f, startY + heroSize + 35 };
            DrawTextEx(font, "TAKEN", takenPos, 20, 0, RED);
        }
    }
}

void HeroSelectionPage::drawCurrentPlayerInfo() {
    if (currentPlayerIndex < firstPri.size()) {
        string playerName = firstPri[currentPlayerIndex].first;
        int priority = firstPri[currentPlayerIndex].second;

        Vector2 titleSize = MeasureTextEx(font, "CHOOSE YOUR HERO", 50, 0);
        Vector2 titlePos = { (SCREEN_WIDTH - titleSize.x) / 2.0f, 50 };
        DrawTextEx(font, "CHOOSE YOUR HERO", titlePos, 50, 0, WHITE);
        
        string playerInfo = "Player: " + playerName + " (Priority: " + to_string(priority) + ")";
        Vector2 playerInfoSize = MeasureTextEx(font, playerInfo.c_str(), 35, 0);
        Vector2 playerInfoPos = { (SCREEN_WIDTH - playerInfoSize.x) / 2.0f, 120 };
        DrawTextEx(font, playerInfo.c_str(), playerInfoPos, 35, 0, WHITE);

        string instruction = "Click on a hero to select it";
        Vector2 instructionSize = MeasureTextEx(font, instruction.c_str(), 25, 0);
        Vector2 instructionPos = { (SCREEN_WIDTH - instructionSize.x) / 2.0f, 170 };
        DrawTextEx(font, instruction.c_str(), instructionPos, 25, 0, YELLOW);
    }
}

void HeroSelectionPage::update(shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {
    Vector2 mouse = GetMousePosition();
    
    // Handle hero selection
    if (currentPlayerIndex < firstPri.size()) {
        auto heroes = sys->getAllHerosAvailable();
        float heroSize = 150;
        float spacing = 50;
        float totalWidth = heroes.size() * heroSize + (heroes.size() - 1) * spacing;
        float startX = (SCREEN_WIDTH - totalWidth) / 2.0f;
        float startY = SCREEN_HEIGHT * 0.4f;
        
        for (size_t i = 0; i < heroes.size(); ++i) {
            float x = startX + i * (heroSize + spacing);
            Rectangle heroRect = { x, startY, heroSize, heroSize };

            bool heroTaken = false;
            for (const auto& playerHero : mainPri) {
                if (playerHero.second == heroes[i]->getHeroName()) {
                    heroTaken = true;
                    break;
                }
            }
            
            if (CheckCollisionPointRec(mouse, heroRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (!heroTaken) {
                    selectedHero = heroes[i]->getHeroName();
                }
            }
        }

        if (IsKeyPressed(KEY_ENTER) && !selectedHero.empty()) {
            addPlayerHero();
        }
    }

    if (CheckCollisionPointRec(mouse, backButton)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            cPage = PageNumbers::WELCOME_PAGE;
        }
    }

    if (CheckCollisionPointRec(mouse, startGameButton)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (currentPlayerIndex >= firstPri.size()) {
                cPage = PageNumbers::HERO_PHASE_PAGE;
                sys->setHeroInitLoacation(mainPri);
                sys->setAllHeros(mainPri);
            }
        }
    }
}

void HeroSelectionPage::draw(shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {
    Rectangle src = { 0, 0, (float)bg.width, (float)bg.height };
    Rectangle dest = { 0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(bg, src, dest, origin, 0, WHITE);
    
    Vector2 mouse = GetMousePosition();

    drawCurrentPlayerInfo();

    drawHeroOptions();

    Color backColor = CheckCollisionPointRec(mouse, backButton) ? YELLOW : WHITE;
    DrawRectangleRec(backButton, { 0, 0, 0, 150 });
    DrawRectangleLinesEx(backButton, 3, backColor);
    
    Vector2 backTextSize = MeasureTextEx(font, "BACK", 40, 0);
    Vector2 backTextPos = { backButton.x + (backButton.width - backTextSize.x) / 2.0f, 
                           backButton.y + (backButton.height - backTextSize.y) / 2.0f };
    DrawTextEx(font, "BACK", backTextPos, 40, 0, backColor);

    if (currentPlayerIndex >= firstPri.size()) {
        Color startGameColor = CheckCollisionPointRec(mouse, startGameButton) ? YELLOW : WHITE;
        DrawRectangleRec(startGameButton, { 0, 0, 0, 150 });
        DrawRectangleLinesEx(startGameButton, 3, startGameColor);
        
        Vector2 startGameTextSize = MeasureTextEx(font, "START GAME", 40, 0);
        Vector2 startGameTextPos = { startGameButton.x + (startGameButton.width - startGameTextSize.x) / 2.0f, 
                                   startGameButton.y + (startGameButton.height - startGameTextSize.y) / 2.0f };
        DrawTextEx(font, "START GAME", startGameTextPos, 40, 0, startGameColor);
    }

    Vector2 playersTitlePos = { 50, SCREEN_HEIGHT - 300 };
    DrawTextEx(font, "Selected Players:", playersTitlePos, 35, 0, WHITE);
    
    Vector2 playerListPos = { 50, SCREEN_HEIGHT - 250 };
    for (size_t i = 0; i < mainPri.size(); ++i) {
        string playerInfo = to_string(i + 1) + ". " + mainPri[i].first + " - " + mainPri[i].second;
        DrawTextEx(font, playerInfo.c_str(), playerListPos, 25, 0, WHITE);
        playerListPos.y += 30;
    }

    Vector2 instructionsPos = { 50, SCREEN_HEIGHT - 100 };
    DrawTextEx(font, "Click on a hero to select it, then press ENTER", instructionsPos, 20, 0, WHITE);
    instructionsPos.y += 25;
    DrawTextEx(font, "Red heroes are already taken by other players", instructionsPos, 20, 0, RED);
} 