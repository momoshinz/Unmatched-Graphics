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
        "Unmatched");

    SetTargetFPS(60);

    std::cout << "Window created.\n";

    AssetManager assets;

    std::cout << "AssetManager created.\n";

    if (!assets.load())
    {
        std::cout << "ASSET LOADING FAILED!\n";

        CloseWindow();
        return 1;
    }

    std::cout << "ALL ASSETS LOADED!\n";

    LoadingScreen loadingScreen(assets, 15.0f);

    while (
        !WindowShouldClose() &&
        !loadingScreen.isFinished())
    {
        loadingScreen.update();

        BeginDrawing();

        ClearBackground(BLACK);

        loadingScreen.draw();

        EndDrawing();
    }

    assets.unload();

    CloseWindow();

    return 0;
}