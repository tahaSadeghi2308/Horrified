#include "gui.hpp"

Gui::Gui(System *s):sys(s)
{

}

void Gui::run() {
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth,screenHeight, "Horrified");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        //handleInput();

        BeginDrawing();
            ClearBackground(WHITE);
            //drawMap();
            //drawRightPanel();
        EndDrawing();
    }

    CloseWindow();
}