#include "gui/player_setup_page.hpp"
#include "system.hpp"
#include <algorithm>
#include <cctype>

using namespace std;

PlayerSetupPage::PlayerSetupPage(vector<pair<string, int>> &pri) : players(pri) {
    bg = LoadTexture("../Horrified_Assets/menu_bg.jpg");
    font = LoadFont("../Horrified_Assets/menu_pages.ttf");
    float inputWidth = 400;
    float inputHeight = 60;
    float buttonWidth = 300;
    float buttonHeight = 80;
    float centerX = (SCREEN_WIDTH - inputWidth) / 2.0f;
    float startY = SCREEN_HEIGHT * 0.3f;
    float spacing = 100;
    
    nameInputBox = { centerX, startY, inputWidth, inputHeight };
    numberInputBox = { centerX, startY + spacing, inputWidth, inputHeight };
    submitButton = { centerX, startY + 2 * spacing, buttonWidth, buttonHeight };
    backButton = { centerX, startY + 3 * spacing, buttonWidth, buttonHeight };
    startGameButton = { centerX + buttonWidth + 50, startY + 2 * spacing, buttonWidth, buttonHeight };
    
    playerName = "";
    playerNumber = "";
    nameActive = false;
    numberActive = false;
}

PlayerSetupPage::~PlayerSetupPage() {
    UnloadTexture(bg);
    UnloadFont(font);
}

bool PlayerSetupPage::isValidName(const string& name) {
    if (name.empty()) return false;

    if (name.find(' ') != string::npos) return false;

    for (const auto& player : players) {
        if (player.first == name) return false;
    }
    
    return true;
}

bool PlayerSetupPage::isValidNumber(const string& number) {
    if (number.empty()) return false;

    for (char c : number) {
        if (!isdigit(c)) return false;
    }

    int num = stoi(number);
    return num > 0;
}

void PlayerSetupPage::validateAndAddPlayer() {
    if (isValidName(playerName) && isValidNumber(playerNumber)) {
        if (players.size() < 2) {
            int number = stoi(playerNumber);
            players.push_back({playerName, number});
            playerName = "";
            playerNumber = "";
            nameActive = false;
            numberActive = false;
            
            sort (players.begin(), players.end(), 
                [](const pair<string, int>& a, const pair<string, int>& b) {
                    return a.second < b.second;
                }
            );
        }
    }
}

void PlayerSetupPage::update(shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {
    Vector2 mouse = GetMousePosition();

    if (players.size() < 2) {
        if (CheckCollisionPointRec(mouse, nameInputBox)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                nameActive = true;
                numberActive = false;
            }
        } else if (CheckCollisionPointRec(mouse, numberInputBox)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                nameActive = false;
                numberActive = true;
            }
        } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            nameActive = false;
            numberActive = false;
        }
    } else {
        nameActive = false;
        numberActive = false;
    }

    if (nameActive) {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (playerName.length() < 20)) {
                playerName += (char)key;
            }
            key = GetCharPressed();
        }
        
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (!playerName.empty()) {
                playerName.pop_back();
            }
        }
    }
    else if (numberActive) {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= '0') && (key <= '9') && (playerNumber.length() < 10)) {
                playerNumber += (char)key;
            }
            key = GetCharPressed();
        }
        
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (!playerNumber.empty()) {
                playerNumber.pop_back();
            }
        }
    }

    if (CheckCollisionPointRec(mouse, submitButton)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (players.size() < 2) {
                validateAndAddPlayer();
            }
        }
    }

    if (CheckCollisionPointRec(mouse, backButton)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            cPage = PageNumbers::WELCOME_PAGE;
        }
    }

    if (CheckCollisionPointRec(mouse, startGameButton)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (players.size() == 2) {
                cPage = PageNumbers::HERO_SELECTION_PAGE;
            }
        }
    }

    if (IsKeyPressed(KEY_ENTER)) {
        validateAndAddPlayer();
    }
}

void PlayerSetupPage::draw(shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {
    Rectangle src = { 0, 0, (float)bg.width, (float)bg.height };
    Rectangle dest = { 0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(bg, src, dest, origin, 0, WHITE);

    Vector2 titleSizeVec = MeasureTextEx(font, "PLAYER SETUP", 60, 0);
    Vector2 titlePos = { (SCREEN_WIDTH - titleSizeVec.x) / 2.0f, 50 };
    DrawTextEx(font, "PLAYER SETUP", titlePos, 60, 0, WHITE);
    
    Vector2 mouse = GetMousePosition();

    Color nameBoxColor;
    if (players.size() >= 2) {
        nameBoxColor = GRAY; // Disabled color
    } else {
        nameBoxColor = nameActive ? YELLOW : WHITE;
    }
    DrawRectangleRec(nameInputBox, { 0, 0, 0, 150 });
    DrawRectangleLinesEx(nameInputBox, 3, nameBoxColor);
    
    Vector2 nameLabelSize = MeasureTextEx(font, "Player Name (no spaces):", 30, 0);
    Vector2 nameLabelPos = { nameInputBox.x, nameInputBox.y - 40 };
    DrawTextEx(font, "Player Name (no spaces):", nameLabelPos, 30, 0, WHITE);
    
    Vector2 nameTextPos = { nameInputBox.x + 10, nameInputBox.y + (nameInputBox.height - 30) / 2.0f };
    DrawTextEx(font, playerName.c_str(), nameTextPos, 30, 0, WHITE);

    Color numberBoxColor;
    if (players.size() >= 2) {
        numberBoxColor = GRAY; // Disabled color
    } else {
        numberBoxColor = numberActive ? YELLOW : WHITE;
    }
    DrawRectangleRec(numberInputBox, { 0, 0, 0, 150 });
    DrawRectangleLinesEx(numberInputBox, 3, numberBoxColor);
    
    Vector2 numberLabelSize = MeasureTextEx(font, "Last time you ate garlic : ", 30, 0);
    Vector2 numberLabelPos = { numberInputBox.x, numberInputBox.y - 40 };
    DrawTextEx(font, "Last time you ate garlic :", numberLabelPos, 30, 0, WHITE);
    
    Vector2 numberTextPos = { numberInputBox.x + 10, numberInputBox.y + (numberInputBox.height - 30) / 2.0f };
    DrawTextEx(font, playerNumber.c_str(), numberTextPos, 30, 0, WHITE);

    Color submitColor;
    if (players.size() >= 2) {
        submitColor = GRAY; // Disabled color
    } else {
        submitColor = CheckCollisionPointRec(mouse, submitButton) ? YELLOW : WHITE;
    }
    DrawRectangleRec(submitButton, { 0, 0, 0, 150 });
    DrawRectangleLinesEx(submitButton, 3, submitColor);
    
    Vector2 submitTextSize = MeasureTextEx(font, "ADD PLAYER", 40, 0);
    Vector2 submitTextPos = { submitButton.x + (submitButton.width - submitTextSize.x) / 2.0f, 
                             submitButton.y + (submitButton.height - submitTextSize.y) / 2.0f };
    DrawTextEx(font, "ADD PLAYER", submitTextPos, 40, 0, submitColor);
    
    // Draw back button
    Color backColor = CheckCollisionPointRec(mouse, backButton) ? YELLOW : WHITE;
    DrawRectangleRec(backButton, { 0, 0, 0, 150 });
    DrawRectangleLinesEx(backButton, 3, backColor);
    
    Vector2 backTextSize = MeasureTextEx(font, "BACK", 40, 0);
    Vector2 backTextPos = { backButton.x + (backButton.width - backTextSize.x) / 2.0f, 
                           backButton.y + (backButton.height - backTextSize.y) / 2.0f };
    DrawTextEx(font, "BACK", backTextPos, 40, 0, backColor);

    if (players.size() == 2) {
        Color startGameColor = CheckCollisionPointRec(mouse, startGameButton) ? YELLOW : WHITE;
        DrawRectangleRec(startGameButton, { 0, 0, 0, 150 });
        DrawRectangleLinesEx(startGameButton, 3, startGameColor);
        
        Vector2 startGameTextSize = MeasureTextEx(font, "Continue", 40, 0);
        Vector2 startGameTextPos = { startGameButton.x + (startGameButton.width - startGameTextSize.x) / 2.0f, 
                                   startGameButton.y + (startGameButton.height - startGameTextSize.y) / 2.0f };
        DrawTextEx(font, "Continue", startGameTextPos, 40, 0, startGameColor);
    }

    Vector2 playersTitlePos = { 50, SCREEN_HEIGHT - 300 };
    DrawTextEx(font, "Current Players:", playersTitlePos, 35, 0, WHITE);
    
    Vector2 playerListPos = { 50, SCREEN_HEIGHT - 250 };
    for (size_t i = 0; i < players.size(); ++i) {
        string playerInfo = to_string(i + 1) + ". " + players[i].first + " - " + to_string(players[i].second);
        DrawTextEx(font, playerInfo.c_str(), playerListPos, 25, 0, WHITE);
        playerListPos.y += 30;
    }

    Vector2 instructionsPos = { 50, SCREEN_HEIGHT - 100 };
    DrawTextEx(font, "Press ENTER or click ADD PLAYER to add a player", instructionsPos, 20, 0, WHITE);
    instructionsPos.y += 25;
    DrawTextEx(font, "Click BACK to return to main menu", instructionsPos, 20, 0, WHITE);
    instructionsPos.y += 25;
    DrawTextEx(font, "Exactly 2 players are required to start the game", instructionsPos, 20, 0, YELLOW);
} 