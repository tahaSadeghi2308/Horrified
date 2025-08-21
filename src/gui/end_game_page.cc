#include "gui/end_game_page.hpp"
#include "system.hpp"

using namespace std;

EndGamePage::EndGamePage(const Font &f, System *s)
: font(f), sys(s), endCode(-1) {
    float buttonWidth = 300;
    float buttonHeight = 80;
    float centerX = (SCREEN_WIDTH - buttonWidth) / 2.0f;
    float centerY = SCREEN_HEIGHT * 0.7f;
    exitButton = { centerX, centerY, buttonWidth, buttonHeight };
}

void EndGamePage::setEndCode(int code) {
    endCode = code;
    switch (endCode) {
        case 1: message = "No monster cards left. You survived!"; break;
        case 2: message = "All monsters defeated. You win!"; break;
        case 3: message = "Terror reached maximum. You lose."; break;
        default: message = "Game over."; break;
    }
}

void EndGamePage::update(shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {
    Vector2 mouse = GetMousePosition();
    if (CheckCollisionPointRec(mouse, exitButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        cPage = PageNumbers::EXIT_PAGE;
    }
}

void EndGamePage::draw(shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);

    const char *title = "GAME OVER";
    Vector2 titleSize = MeasureTextEx(font, title, 80, 0);
    Vector2 titlePos = { (SCREEN_WIDTH - titleSize.x) / 2.0f, SCREEN_HEIGHT * 0.2f };
    DrawTextEx(font, title, titlePos, 80, 0, WHITE);

    if (message.empty()) setEndCode(endCode);
    Vector2 msgSize = MeasureTextEx(font, message.c_str(), 40, 0);
    Vector2 msgPos = { (SCREEN_WIDTH - msgSize.x) / 2.0f, SCREEN_HEIGHT * 0.4f };
    DrawTextEx(font, message.c_str(), msgPos, 40, 0, WHITE);

    Vector2 mouse = GetMousePosition();
    Color border = CheckCollisionPointRec(mouse, exitButton) ? YELLOW : WHITE;
    DrawRectangleRec(exitButton, { 0, 0, 0, 150 });
    DrawRectangleLinesEx(exitButton, 3, border);

    const char *exitText = "EXIT";
    Vector2 exitTextSize = MeasureTextEx(font, exitText, 40, 0);
    Vector2 exitTextPos = { exitButton.x + (exitButton.width - exitTextSize.x) / 2.0f,
                            exitButton.y + (exitButton.height - exitTextSize.y) / 2.0f };
    DrawTextEx(font, exitText, exitTextPos, 40, 0, border);
}

