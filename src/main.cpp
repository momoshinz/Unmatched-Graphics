#include <raylib.h>
#include <iostream>

#include "graphics/AssetManager.h"
#include "graphics/LoadingScreen.h"
#include "graphics/MainMenu.h"
#include "graphics/GameScreen.h"
#include "graphics/Transition.h"
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
        "UNMATCHED");

    // =========================================
    // Audio
    // =========================================

    InitAudioDevice();

    Image icon =
        LoadImage(
            "Unmatched_Assets/icon.png");

    if (icon.data == nullptr)
    {
        std::cout
            << "Failed to load window icon!\n";
    }
    else
    {
        SetWindowIcon(icon);
        UnloadImage(icon);
    }

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

        CloseAudioDevice();
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
        20.0f);

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
    // START MAIN MENU MUSIC
    // =========================================

    Music gameMusic =
        assets.getGameMusic();

    if (IsMusicValid(gameMusic))
    {
        PlayMusicStream(gameMusic);

        std::cout
            << "Main Menu music started.\n";
    }
    else
    {
        std::cout
            << "Main Menu music is invalid!\n";
    }

    // =========================================
    // Game Objects
    // =========================================

    Game game;

    MainMenu mainMenu(
        &assets,
        &game);

    GameScreen gameScreen(
        &assets,
        &game);

    // =========================================
    // Screen State
    // =========================================

    enum class Screen
    {
        MAIN_MENU,
        GAME
    };

    Screen currentScreen =
        Screen::MAIN_MENU;

    Screen nextScreen =
        Screen::MAIN_MENU;

    // =========================================
    // Transition
    // =========================================

    Transition transition;

    transition.setFont(
        assets.getLoadingFont());

    // =========================================
    // Main Loop
    // =========================================

    while (!WindowShouldClose())
    {
        float deltaTime =
            GetFrameTime();

        // =====================================================
        // MUSIC UPDATE
        // =====================================================

        if (IsMusicValid(gameMusic))
        {
            UpdateMusicStream(gameMusic);
        }

        // =====================================================
        // TRANSITION UPDATE
        // =====================================================

        if (transition.isActive())
        {
            transition.update(deltaTime);

            // -------------------------------------------------
            // Switch screen when the black part of the
            // transition is finished.
            // -------------------------------------------------

            if (transition.shouldSwitch())
            {
                currentScreen = nextScreen;

                // -------------------------------------------------
                // Entering Game Screen
                // Stop Main Menu music.
                // -------------------------------------------------

                if (currentScreen == Screen::GAME)
                {
                    if (IsMusicValid(gameMusic))
                    {
                        StopMusicStream(gameMusic);

                        std::cout
                            << "Main Menu music stopped.\n";
                    }

                    // =========================================
                    // RESET PLAYER PANEL TEXT ANIMATION
                    // =========================================

                    gameScreen.resetPlayerPanelAnimations();
                }

                transition.finishSwitch();

                std::cout
                    << "Screen switched.\n";
            }
        }

        // =====================================================
        // NORMAL UPDATE
        // =====================================================

        else
        {
            // =================================================
            // MAIN MENU
            // =================================================

            if (currentScreen ==
                Screen::MAIN_MENU)
            {
                mainMenu.update();

                int result =
                    mainMenu.handleInput();

                // =============================================
                // NEW GAME
                // =============================================

                if (result == 1)
                {
                    std::cout
                        << "NEW GAME selected.\n";
                }

                // =============================================
                // LOAD GAME
                // =============================================

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
                    catch (
                        const std::exception &e)
                    {
                        std::cout
                            << "\n[!] ERROR: "
                            << e.what()
                            << "\n";
                    }
                }

                // =============================================
                // EXIT
                // =============================================

                else if (result == 3)
                {
                    break;
                }

                // =============================================
                // START GAME
                // =============================================

                else if (result == 4)
                {
                    std::cout
                        << "Placement finished.\n";

                    // -----------------------------------------
                    // Start game logic
                    // -----------------------------------------

                    game.beginTurns();

                    std::cout
                        << "Starting transition to game...\n";

                    // -----------------------------------------
                    // Destination
                    // -----------------------------------------

                    nextScreen =
                        Screen::GAME;

                    // -----------------------------------------
                    // Transition Text
                    // -----------------------------------------

                    transition.setText(
                        "GET READY FIGHTERS . . .");

                    // -----------------------------------------
                    // Transition
                    // -----------------------------------------

                    transition.start(
                        TransitionType::Fade,
                        1.0f,
                        2.5f);
                }
            }

            // =================================================
            // GAME SCREEN
            // =================================================

            else if (currentScreen ==
                     Screen::GAME)
            {
                int result =
                    gameScreen.update();

                // =============================================
                // RETURN TO MAIN MENU
                // =============================================

                if (result == 1)
                {
                    std::cout
                        << "Returning to main menu...\n";

                    // -----------------------------------------
                    // Restart Main Menu music from the beginning
                    // -----------------------------------------

                    if (IsMusicValid(gameMusic))
                    {
                        StopMusicStream(gameMusic);
                        PlayMusicStream(gameMusic);

                        std::cout
                            << "Main Menu music restarted.\n";
                    }

                    // -----------------------------------------
                    // Destination
                    // -----------------------------------------

                    nextScreen =
                        Screen::MAIN_MENU;

                    // -----------------------------------------
                    // Change transition text
                    // -----------------------------------------

                    transition.setText(
                        "LOADING . . .");

                    // -----------------------------------------
                    // Transition
                    // -----------------------------------------

                    transition.start(
                        TransitionType::Fade,
                        1.0f,
                        1.5f);
                }
            }
        }

        // =====================================================
        // DRAW
        // =====================================================

        BeginDrawing();

        ClearBackground(BLACK);

        // =====================================================
        // Current Screen
        // =====================================================

        if (currentScreen ==
            Screen::MAIN_MENU)
        {
            mainMenu.draw();
        }
        else if (currentScreen ==
                 Screen::GAME)
        {
            gameScreen.draw();
        }

        // =====================================================
        // Transition
        // =====================================================

        transition.draw();

        EndDrawing();
    }

    // =========================================
    // Cleanup
    // =========================================

    assets.unload();

    CloseAudioDevice();

    CloseWindow();

    return 0;
}