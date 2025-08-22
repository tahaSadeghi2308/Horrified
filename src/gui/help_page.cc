#include "gui/help_page.hpp"
#include "system.hpp"
#include <fstream>
#include <sstream>

using namespace std;

HelpPage::HelpPage(const Font &f , System *s)
: font(f) , sys(s) , scrollOffset(0.0f)
{
    backButton = { 40.0f , 30.0f , 160.0f , 50.0f };
    loadManualFromFile("../data/manual.txt");
}

void HelpPage::loadManualFromFile(const std::string &path){
    manualLines.clear();
    ifstream file(path);
    if (!file.is_open()){
        manualLines.push_back("Manual file not found: " + path);
        manualLines.push_back("Create the file to customize in-game help.");
        return;
    }
    string line;
    while (getline(file , line)){
        manualLines.push_back(line);
    }
    file.close();
}

void HelpPage::update(shared_ptr<HeroBase> &cHero , int &actions , PageNumbers &cPage){
    float scrollSpeed = 30.0f;
    scrollOffset += GetMouseWheelMove() * scrollSpeed;

    // Compute document height
    float fontSize = 24.0f;
    float lineHeight = fontSize + 8.0f;
    float contentHeight = static_cast<float>(manualLines.size()) * lineHeight + 40.0f; // padding
    float viewportHeight = SCREEN_HEIGHT - 40.0f; // top area reserved for back btn

    if (contentHeight > viewportHeight){
        float minScroll = viewportHeight - contentHeight;
        if (scrollOffset > 0.0f) scrollOffset = 0.0f;
        if (scrollOffset < minScroll) scrollOffset = minScroll;
    } else {
        scrollOffset = 0.0f;
    }

    Vector2 mouse = GetMousePosition();
    if (CheckCollisionPointRec(mouse , backButton)){
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            cPage = PageNumbers::HERO_PHASE_PAGE;
        }
    }

    if (IsKeyPressed(KEY_BACKSPACE)){
        cPage = PageNumbers::HERO_PHASE_PAGE;
    }
}

void HelpPage::draw(shared_ptr<HeroBase> &cHero , int &actions , PageNumbers &cPage){
    ClearBackground(BLACK);

    // Centered panel
    float panelW = 900.0f;
    float panelH = 650.0f;
    Rectangle panel = { (SCREEN_WIDTH - panelW) / 2.0f , (SCREEN_HEIGHT - panelH) / 2.0f , panelW , panelH };
    Color panelBg = CLITERAL(Color){ 40, 40, 40, 255 };
    Color panelBorder = CLITERAL(Color){ 200, 200, 200, 255 };
    DrawRectangleRec(panel, panelBg);
    DrawRectangleLinesEx(panel, 2.0f, panelBorder);

    // Back button inside panel header
    Rectangle localBack = { panel.x + 20.0f , panel.y + 16.0f , 120.0f , 40.0f };
    DrawRectangleRec(localBack , DARKGREEN);
    const char *backTxt = "Back";
    Vector2 backSize = MeasureTextEx(font , backTxt , 26.0f , 0.0f);
    DrawTextEx(font , backTxt , { localBack.x + (localBack.width - backSize.x)/2.0f , localBack.y + (localBack.height - backSize.y)/2.0f } , 26.0f , 0.0f , WHITE);
    backButton = localBack;

    // Title
    const char *title = "In-Game Help";
    Vector2 titleSize = MeasureTextEx(font, title, 30.0f, 0.0f);
    DrawTextEx(font, title, { panel.x + panel.width - titleSize.x - 20.0f, panel.y + 20.0f }, 30.0f, 0.0f, WHITE);

    // Scrollable viewport inside panel body
    float margin = 20.0f;
    Rectangle viewport = { panel.x + margin , panel.y + 70.0f , panel.width - 2*margin , panel.height - 90.0f };
    DrawRectangleLinesEx(viewport , 1.0f , panelBorder);

    BeginScissorMode(viewport.x , viewport.y , viewport.width , viewport.height);
    float fontSize = 24.0f;
    float lineHeight = fontSize + 8.0f;
    float x = viewport.x + 12.0f;
    float y = viewport.y + 12.0f + scrollOffset;

    for (const auto &line : manualLines){
        DrawTextEx(font , line.c_str() , { x , y } , fontSize , 0.0f , RAYWHITE);
        y += lineHeight;
    }
    EndScissorMode();
}

