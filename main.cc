#include "tui.hpp"
#include "gui.hpp"
#include <iostream>
#include "system.hpp"
#include <fmt/core.h>
#include <thread>

using namespace std;

int main(){
    
    const int SCREEN_WIDTH = 1600;
    const int SCREEN_HEIGHT = 900;
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT,"Horrified");
    SetTargetFPS(60);

    BeginDrawing();
    ClearBackground(BLACK);

    const char* loadingText = "Loading...";
    int fontSize = 40;
    int textWidth = MeasureText(loadingText, fontSize);
    int x = (SCREEN_WIDTH - textWidth) / 2;
    int y = (SCREEN_HEIGHT - fontSize) / 2;

    DrawText(loadingText, x, y, fontSize, WHITE);

    EndDrawing();

    System *s;
    Gui t(s,SCREEN_WIDTH,SCREEN_HEIGHT);
    t.run();

    return 0;
}