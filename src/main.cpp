#include <raylib.h>
#include <iostream>
#include "graphics/AssetManager.h"

int main()
{
    InitWindow(1280, 720, "Unmatched");
    SetTargetFPS(60);

    std::cout << "Window created.\n";

    AssetManager assets;

    std::cout << "AssetManager created.\n";

    if (!assets.load())
    {
        std::cout << "ASSET LOADING FAILED!\n";

        while (!WindowShouldClose())
        {
            BeginDrawing();

            ClearBackground(BLACK);

            DrawText(
                "ASSET LOADING FAILED!",
                400,
                330,
                30,
                RED);

            EndDrawing();
        }

        CloseWindow();
        return 1;
    }

    std::cout << "ALL ASSETS LOADED!\n";

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        Texture2D menu = assets.getMainMenuBackground();

        DrawTexturePro(
            menu,
            Rectangle{
                0,
                0,
                (float)menu.width,
                (float)menu.height},
            Rectangle{
                0,
                0,
                1280,
                720},
            Vector2{0, 0},
            0.0f,
            WHITE);

        EndDrawing();
    }

    assets.unload();
    CloseWindow();

    return 0;
}