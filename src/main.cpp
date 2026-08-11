#include <raylib.h>
#include <iostream>

#include "graphics/AssetManager.h"
#include "graphics/LoadingScreen.h"

int main()
{
    // =========================================
    // WINDOW
    // =========================================

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
    // ASSET MANAGER
    // =========================================

    AssetManager assets;

    std::cout << "AssetManager created.\n";

    if (!assets.load())
    {
        std::cout << "ASSET LOADING FAILED!\n";

        CloseWindow();
        return 1;
    }

    std::cout << "ALL ASSETS LOADED!\n";


    // =========================================
    // LOADING SCREEN
    // =========================================

    LoadingScreen loadingScreen(
        &assets,
        5.0f
    );

    while (!WindowShouldClose() &&
           !loadingScreen.isFinished())
    {
        // -----------------------------
        // Update
        // -----------------------------

        loadingScreen.update();


        // -----------------------------
        // Draw
        // -----------------------------

        BeginDrawing();

        ClearBackground(BLACK);

        loadingScreen.draw();

        EndDrawing();
    }


    // =========================================
    // MAIN MENU TEST
    // =========================================

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        Texture2D mainMenu =
            assets.getMainMenuBackground();

        if (mainMenu.id != 0)
        {
            DrawTexturePro(
                mainMenu,

                // Source rectangle
                Rectangle{
                    0,
                    0,
                    (float)mainMenu.width,
                    (float)mainMenu.height
                },

                // Destination rectangle
                Rectangle{
                    0,
                    0,
                    (float)screenWidth,
                    (float)screenHeight
                },

                Vector2{
                    0,
                    0
                },

                0.0f,

                WHITE
            );
        }

        EndDrawing();
    }


    // =========================================
    // CLEANUP
    // =========================================

    assets.unload();

    CloseWindow();

    return 0;
}