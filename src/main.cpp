#include <raylib.h>
#include <iostream>

#include "graphics/AssetManager.h"
#include "graphics/LoadingScreen.h"

int main()
{
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(
        screenWidth,
        screenHeight,
        "Unmatched"
    );

    SetTargetFPS(60);

    std::cout << "Window created.\n";

    // =========================================
    // Asset Manager
    // =========================================

    AssetManager assets;

    if (!assets.load())
    {
        std::cout << "ASSET LOADING FAILED!\n";

        CloseWindow();
        return 1;
    }

    std::cout << "ALL ASSETS LOADED!\n";

    // =========================================
    // Loading Screen
    // =========================================

    LoadingScreen loadingScreen(5.0f);

    if (!loadingScreen.load())
    {
        std::cout << "LOADING SCREEN FAILED!\n";

        CloseWindow();
        return 1;
    }

    // =========================================
    // Loading loop
    // =========================================

    while (!WindowShouldClose() &&
           !loadingScreen.isFinished())
    {
        loadingScreen.update();

        BeginDrawing();

        ClearBackground(BLACK);

        loadingScreen.draw();

        EndDrawing();
    }

    // =========================================
    // Test after loading
    // =========================================

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        DrawText(
            "LOADING FINISHED!",
            600,
            430,
            30,
            WHITE
        );

        EndDrawing();
    }

    // =========================================
    // Cleanup
    // =========================================

    loadingScreen.unload();
    assets.unload();

    CloseWindow();

    return 0;
}