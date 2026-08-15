#include <raylib.h>
#include <iostream>

#include "graphics/AssetManager.h"
#include "graphics/LoadingScreen.h"
#include "graphics/MainMenu.h"
#include "graphics/GameScreen.h"
#include "game/Game.h"

int main()
{
    const int screenWidth = 1630;
    const int screenHeight = 930;

    // =========================================
    // Window
    // =========================================

    InitWindow(
        screenWidth,
        screenHeight,
        "Unmatched");

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

    LoadingScreen loadingScreen(
        assets,
        5.0f);

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
    // Game Objects
    // =========================================

    Game game;

    MainMenu mainMenu(
        &assets,
        &game);

    GameScreen gameScreen(&assets, &game);

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
        // =====================================================
        // UPDATE
        // =====================================================

        if (currentScreen == Screen::MAIN_MENU)
        {
            // -------------------------------------------------
            // MainMenu update
            //
            // This also handles:
            // Player input
            // Hero selection
            // Placement
            // -------------------------------------------------

            mainMenu.update();

            int result =
                mainMenu.handleInput();

            // -------------------------------------------------
            // NEW GAME
            //
            // Do nothing here.
            // MainMenu itself handles the transition
            // from player input -> ready -> hero selection.
            // -------------------------------------------------

            if (result == 1)
            {
                std::cout
                    << "NEW GAME selected.\n";
            }

            // -------------------------------------------------
            // LOAD GAME
            // -------------------------------------------------

            else if (result == 2)
            {
                std::cout
                    << "LOAD GAME selected.\n";

                try
                {
                    if (game.loadMenu())
                    {
                        game.run(true);
                    }
                }
                catch (const std::exception &e)
                {
                    std::cout
                        << "\n[!] ERROR: "
                        << e.what()
                        << "\n";
                }
            }

            // -------------------------------------------------
            // EXIT APPLICATION
            // -------------------------------------------------

            else if (result == 3)
            {
                break;
            }

            // -------------------------------------------------
            // GAME START
            //
            // IMPORTANT:
            //
            // This result should ONLY happen after
            // the whole placement process is finished.
            //
            // Hero selection itself must NOT return 4 anymore.
            // -------------------------------------------------

            else if (result == 4)
            {
                std::cout
                    << "Placement finished.\n";

                std::cout
                    << "Starting game screen...\n";

               currentScreen = Screen::GAME;
            }
        }

        // =====================================================
        // GAME SCREEN
        // =====================================================

        else if (currentScreen == Screen::GAME)
        {
            int result =
                gameScreen.update();

            // -------------------------------------------------
            // RETURN TO MAIN MENU
            // -------------------------------------------------

            if (result == 1)
            {
                std::cout
                    << "Returning to main menu...\n";

                // Recreate MainMenu so that:
                // state = MAIN_MENU
                // previous player information is cleared
                // placement state is reset
                // hero selection state is reset

                mainMenu =
                    MainMenu(
                        &assets,
                        &game);

                currentScreen =
                    Screen::MAIN_MENU;
            }
        }

        // =====================================================
        // DRAW
        // =====================================================

        BeginDrawing();

        // -----------------------------------------------------
        // MAIN MENU
        // -----------------------------------------------------

        if (currentScreen == Screen::MAIN_MENU)
        {
            mainMenu.draw();
        }

        // -----------------------------------------------------
        // GAME SCREEN
        // -----------------------------------------------------

        else if (currentScreen == Screen::GAME)
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