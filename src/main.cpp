#include <raylib.h>
#include <iostream>
#include "graphics/AssetManager.h"
#include "graphics/LoadingScreen.h"
#include "graphics/MainMenu.h"
#include "game/Game.h"

int main()
{
    const int screenWidth = 1338;
    const int screenHeight = 738;

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

    LoadingScreen loadingScreen(assets, 5.0f);  //the time we stay on loading screen

    while (!WindowShouldClose() &&
           !loadingScreen.isFinished())
    {
        loadingScreen.update();

        BeginDrawing();

        ClearBackground(BLACK);

        loadingScreen.draw();

        EndDrawing();
    }


    MainMenu mainMenu(&assets);

    while (!WindowShouldClose())
    {
        mainMenu.update();

        int result =
            mainMenu.handleInput();

        if (result == 1)
        {
            std::cout << "NEW GAME selected.\n";

            // فعلاً بعداً Game را اینجا اجرا می‌کنیم
        }
        else if (result == 2)
        {
            std::cout << "LOAD GAME selected.\n";

            Game game;

            try
            {
                if (game.loadMenu())
                {
                    game.run(true);
                }
            }
            catch (const std::exception &e)
            {
                std::cout << "\n[!] ERROR: "
                        << e.what()
                        << "\n";
            }
        }
        else if (result == 3)
        {
            break;
        }

        BeginDrawing();

        ClearBackground(BLACK);

        mainMenu.draw();

        EndDrawing();
    }

    // =========================================
    // Cleanup
    // =========================================

    assets.unload();

    CloseWindow();

    return 0;
}
