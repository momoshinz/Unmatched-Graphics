#include <raylib.h>
#include <iostream>

#include "graphics/AssetManager.h"
#include "graphics/LoadingScreen.h"
#include "graphics/MainMenu.h"

int main()
{
    // -----------------------------
    // WINDOW
    // -----------------------------

    InitWindow(1600, 900, "Unmatched");
    SetTargetFPS(60);

    std::cout << "Window created.\n";

    // -----------------------------
    // ASSET MANAGER
    // -----------------------------

    AssetManager assets;

    std::cout << "AssetManager created.\n";

    if (!assets.load())
    {
        std::cout << "ASSET LOADING FAILED!\n";

        CloseWindow();
        return 1;
    }

    std::cout << "ALL ASSETS LOADED!\n";

    // -----------------------------
    // LOADING SCREEN
    // -----------------------------

    float loadingTime = 5.0f;
    float elapsedTime = 0.0f;

    while (!WindowShouldClose() &&
           elapsedTime < loadingTime)
    {
        float deltaTime = GetFrameTime();
        elapsedTime += deltaTime;

        BeginDrawing();

        ClearBackground(BLACK);

        Texture2D loading =
            assets.getLoadingBackground();

        DrawTexturePro(
            loading,

            // قسمت مورد استفاده از تصویر
            Rectangle{
                0,
                0,
                (float)loading.width,
                (float)loading.height
            },

            // اندازه‌ای که روی پنجره نمایش داده می‌شود
            Rectangle{
                0,
                0,
                1600,
                900
            },

            Vector2{0, 0},
            0.0f,
            WHITE
        );

        DrawText(
            "LOADING...",
            700,
            450,
            30,
            WHITE
        );

        EndDrawing();
    }

    // -----------------------------
    // MAIN MENU TEST
    // -----------------------------

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        Texture2D mainMenu =
            assets.getMainMenuBackground();

        DrawTexturePro(
            mainMenu,

            Rectangle{
                0,
                0,
                (float)mainMenu.width,
                (float)mainMenu.height
            },

            Rectangle{
                0,
                0,
                1600,
                900
            },

            Vector2{0, 0},
            0.0f,
            WHITE
        );

        EndDrawing();
    }

    // -----------------------------
    // CLEANUP
    // -----------------------------

    assets.unload();

    CloseWindow();

    return 0;
}