#include <raylib.h>
#include <iostream>

#include "graphics/AssetManager.h"
#include "graphics/LoadingScreen.h"
#include "graphics/MainMenu.h"
#include "graphics/GameScreen.h"
#include "game/Game.h"

int main()
{
    const int screenWidth = 1338;
    const int screenHeight = 738;

    // =========================================
    // Window
    // =========================================

    InitWindow(
        screenWidth,
        screenHeight,
        "Unmatched"
    );

    SetTargetFPS(60);

    std::cout
        << "Window created.\n";

    // =========================================
    // Asset Manager
    // =========================================

    AssetManager assets;

    if (!assets.load())
    {
        std::cout
            << "ASSET LOADING FAILED!\n";

        CloseWindow();

        return 1;
    }

    std::cout
        << "ALL ASSETS LOADED!\n";

    // =========================================
    // Loading Screen
    // =========================================

    LoadingScreen loadingScreen(
        assets,
        5.0f
    );

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

    // =========================================
    // Screens
    // =========================================

    MainMenu mainMenu(&assets);

    GameScreen gameScreen(&assets);

    // =========================================
    // Current Screen
    // =========================================

    enum class Screen
    {
        MAIN_MENU,
        GAME
    };

    Screen currentScreen =
        Screen::MAIN_MENU;

    // =========================================
    // Main Loop
    // =========================================

    while (!WindowShouldClose())
    {
        // =====================================
        // MAIN MENU
        // =====================================

        if (currentScreen ==
            Screen::MAIN_MENU)
        {
            mainMenu.update();

            int result =
                mainMenu.handleInput();

            // ---------------------------------
            // NEW GAME
            // ---------------------------------

            if (result == 1)
            {
                std::cout
                    << "NEW GAME selected.\n";
            }

            // ---------------------------------
            // LOAD GAME
            // ---------------------------------

            else if (result == 2)
            {
                std::cout
                    << "LOAD GAME selected.\n";

                Game game;

                try
                {
                    if (game.loadMenu())
                    {
                        game.run(true);
                    }
                }
                catch (
                    const std::exception &e)
                {
                    std::cout
                        << "\n[!] ERROR: "
                        << e.what()
                        << "\n";
                }
            }

            // ---------------------------------
            // EXIT APPLICATION
            // ---------------------------------

            else if (result == 3)
            {
                break;
            }

            // ---------------------------------
            // FINISH HERO SELECTION
            // ---------------------------------

            else if (result == 4)
            {
                std::cout
                    << "Starting game screen...\n";

                currentScreen =
                    Screen::GAME;
            }
        }

        // =====================================
        // GAME SCREEN
        // =====================================

        else if (currentScreen ==
                 Screen::GAME)
        {
            int result =
                gameScreen.update();

            // ---------------------------------
            // EXIT GAME SCREEN
            // ---------------------------------

            if (result == 1)
            {
                std::cout
                    << "Returning to main menu...\n";

                // ---------------------------------
                // Recreate MainMenu
                //
                // This guarantees that the menu
                // starts from MAIN_MENU state.
                // ---------------------------------

                mainMenu =
                    MainMenu(&assets);

                currentScreen =
                    Screen::MAIN_MENU;
            }
        }

        // =====================================
        // DRAW
        // =====================================

        BeginDrawing();

        // -------------------------------------
        // Main Menu
        // -------------------------------------

        if (currentScreen ==
            Screen::MAIN_MENU)
        {
            mainMenu.draw();
        }

        // -------------------------------------
        // Game Screen
        // -------------------------------------

        else if (currentScreen ==
                 Screen::GAME)
        {
            gameScreen.draw();
        }

        EndDrawing();
    }

    // =========================================
    // Cleanup
    // =========================================

    assets.unload();

    CloseWindow();

    return 0;
}