#include "gui/load_match.hpp"
#include "system.hpp"
#include <filesystem>
#include <algorithm>

using namespace std;
namespace fs = std::filesystem;

static vector<string> readAfterGameFolders() {
    vector<string> names;
    fs::path root = fs::path("../data/after_game");
    if (!fs::exists(root)) return names;
    for (const auto &entry : fs::directory_iterator(root)) {
        if (entry.is_directory()) {
            names.push_back(entry.path().filename().string());
        }
    }
    sort(names.begin(), names.end(), [](const string &a, const string &b){
        // numeric sort if both are numbers; fall back to lexicographic
        try {
            int ai = stoi(a), bi = stoi(b);
            return ai < bi;
        } catch (...) {
            return a < b;
        }
    });
    return names;
}

LoadMatchPage::LoadMatchPage(int &selectedFolderRef) : selectedFolderNumber(selectedFolderRef) {
    bg = LoadTexture("../Horrified_Assets/menu_bg.jpg");
    font = LoadFont("../Horrified_Assets/menu_pages.ttf");

    float buttonWidth = 300;
    float buttonHeight = 80;
    float pad = 30;
    backButton = { pad, (float)SCREEN_HEIGHT - buttonHeight - pad, buttonWidth, buttonHeight };
    startGameButton = { (float)SCREEN_WIDTH - buttonWidth - pad, (float)SCREEN_HEIGHT - buttonHeight - pad, buttonWidth, buttonHeight };

    selectedIndex = -1;
    scrollOffset = 0.0f;
    folderNames = readAfterGameFolders();
}

LoadMatchPage::~LoadMatchPage() {
    UnloadTexture(bg);
    UnloadFont(font);
}

void LoadMatchPage::update(shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {
    Vector2 mouse = GetMousePosition();
    float listTop = 120.0f;
    float rowHeight = 60.0f;
    float listHeight = (float)SCREEN_HEIGHT - listTop - 180.0f; // leave space for buttons

    // scroll
    float maxContent = (float)folderNames.size() * rowHeight;
    float minScroll = min(0.0f, listHeight - maxContent);
    scrollOffset += GetMouseWheelMove() * 25.0f;
    if (scrollOffset > 0.0f) scrollOffset = 0.0f;
    if (scrollOffset < minScroll) scrollOffset = minScroll;

    // click on rows
    Rectangle listRect = { (float)SCREEN_WIDTH * 0.15f, listTop, (float)SCREEN_WIDTH * 0.7f, listHeight };
    if (CheckCollisionPointRec(mouse, listRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        float localY = mouse.y - listRect.y - scrollOffset;
        int index = (int)(localY / rowHeight);
        if (index >= 0 && index < (int)folderNames.size()) {
            selectedIndex = index;
            try {
                selectedFolderNumber = stoi(folderNames[selectedIndex]);
            } catch (...) {
                selectedFolderNumber = -1;
            }
        }
    }

    // back
    if (CheckCollisionPointRec(mouse, backButton)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            cPage = PageNumbers::WELCOME_PAGE;
        }
    }

    // start game: rely on Gui to read selected id; we'll set a global via System? We'll write into a file? Not needed.
    // Here we simply switch page; Gui will probe a dedicated variable. We'll use actions as a scratch? Better not.
    if (CheckCollisionPointRec(mouse, startGameButton) && selectedIndex >= 0) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // We encode the chosen folder into actions temporarily is unsafe. Instead, we rely on Gui reading a getter from this page.
            // Since PageBase has no data bridge, we'll store the chosen number in System singleton? Not available.
            // Alternative: Name folders as numbers, and Gui::run before switching page cannot read it. We'll set a hidden global.
            // Simpler: Change page to HERO_PHASE_PAGE after Gui::loadState is called externally.
            // We'll switch to HERO_PHASE_PAGE_LOAD sentinel; Gui will detect and call load.
            cPage = PageNumbers::HERO_PHASE_PAGE;
        }
    }
}

void LoadMatchPage::draw(shared_ptr<HeroBase> &cHero, int &actions, PageNumbers &cPage) {
    Rectangle src = { 0, 0, (float)bg.width, (float)bg.height };
    Rectangle dest = { 0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(bg, src, dest, origin, 0, WHITE);

    Vector2 titleSize = MeasureTextEx(font, "LOAD MATCH", 60, 0);
    Vector2 titlePos = { (SCREEN_WIDTH - titleSize.x) / 2.0f, 40 };
    DrawTextEx(font, "LOAD MATCH", titlePos, 60, 0, WHITE);

    string countText = to_string(folderNames.size()) + string(" matches found");
    Vector2 countSize = MeasureTextEx(font, countText.c_str(), 25, 0);
    Vector2 countPos = { (SCREEN_WIDTH - countSize.x) / 2.0f, titlePos.y + titleSize.y + 10 };
    DrawTextEx(font, countText.c_str(), countPos, 25, 0, LIGHTGRAY);

    Vector2 mouse = GetMousePosition();

    float listTop = 120.0f;
    float rowHeight = 60.0f;
    Rectangle listRect = { (float)SCREEN_WIDTH * 0.15f, listTop, (float)SCREEN_WIDTH * 0.7f, (float)SCREEN_HEIGHT - listTop - 180.0f };
    DrawRectangleRec(listRect, Color{ 0, 0, 0, 150 });

    BeginScissorMode(listRect.x, listRect.y, listRect.width, listRect.height);
    float y = listRect.y + scrollOffset;
    for (size_t i = 0; i < folderNames.size(); ++i) {
        string rowText = folderNames[i] + " match data";
        Rectangle rowRect = { listRect.x, y, listRect.width, rowHeight };
        bool hovered = CheckCollisionPointRec(mouse, rowRect);
        Color color = (int)i == selectedIndex ? YELLOW : hovered ? LIGHTGRAY : WHITE;
        DrawRectangleRec(rowRect, Color{ 0, 0, 0, 0 });
        DrawRectangleLinesEx(rowRect, 2, color);
        Vector2 txtSize = MeasureTextEx(font, rowText.c_str(), 30, 0);
        Vector2 txtPos = { rowRect.x + 15, rowRect.y + (rowHeight - txtSize.y) / 2.0f };
        DrawTextEx(font, rowText.c_str(), txtPos, 30, 0, color);
        y += rowHeight;
    }
    EndScissorMode();

    // buttons
    Color backColor = CheckCollisionPointRec(mouse, backButton) ? YELLOW : WHITE;
    DrawRectangleRec(backButton, { 0, 0, 0, 150 });
    DrawRectangleLinesEx(backButton, 3, backColor);
    Vector2 backTextSize = MeasureTextEx(font, "BACK", 40, 0);
    Vector2 backTextPos = { backButton.x + (backButton.width - backTextSize.x) / 2.0f,
                            backButton.y + (backButton.height - backTextSize.y) / 2.0f };
    DrawTextEx(font, "BACK", backTextPos, 40, 0, backColor);

    Color startColor = (selectedIndex >= 0 && CheckCollisionPointRec(mouse, startGameButton)) ? YELLOW : WHITE;
    DrawRectangleRec(startGameButton, { 0, 0, 0, 150 });
    DrawRectangleLinesEx(startGameButton, 3, startColor);
    Vector2 startTextSize = MeasureTextEx(font, "START GAME", 40, 0);
    Vector2 startTextPos = { startGameButton.x + (startGameButton.width - startTextSize.x) / 2.0f,
                             startGameButton.y + (startGameButton.height - startTextSize.y) / 2.0f };
    DrawTextEx(font, "START GAME", startTextPos, 40, 0, startColor);
}
