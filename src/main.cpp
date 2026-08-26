#include <raylib.h>
#include <iostream>
#include <string>

#include "graphics/AssetManager.h"
#include "graphics/LoadingScreen.h"
#include "graphics/MainMenu.h"
#include "graphics/GameScreen.h"
#include "graphics/Transition.h"
#include "graphics/WinnerUI.h"
#include "fighter/Hero.h"
#include "game/Game.h"

int main()
{
    // =========================================================
    // WINDOW
    // =========================================================

    const int screenWidth = 1630;
    const int screenHeight = 930;

    InitWindow(
        screenWidth,
        screenHeight,
        "UNMATCHED");

    InitAudioDevice();

    Image icon = LoadImage("Unmatched_Assets/icon.png");

    if (icon.data != nullptr)
    {
        SetWindowIcon(icon);
        UnloadImage(icon);
    }

    SetTargetFPS(60);

    // =========================================================
    // ASSETS
    // =========================================================

    AssetManager assets;

    if (!assets.load())
    {
        CloseAudioDevice();
        CloseWindow();
        return 1;
    }

    // =========================================================
    // LOADING SCREEN
    // =========================================================

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

    // =========================================================
    // MUSIC
    // =========================================================

    Music gameMusic = assets.getGameMusic();

    if (IsMusicValid(gameMusic))
    {
        PlayMusicStream(gameMusic);
    }

    // =========================================================
    // GAME OBJECTS
    // =========================================================

    Game game;

    MainMenu mainMenu(
        &assets,
        &game);

    GameScreen gameScreen(
        &assets,
        &game);

    WinnerUI winnerUI(
        &assets);

    // =========================================================
    // WINNER STATE
    // =========================================================

    Hero *pendingWinnerHero = nullptr;

    bool winnerTransitionStarted = false;

    // =========================================================
    // SCREEN STATE
    // =========================================================

    enum class Screen
    {
        MAIN_MENU,
        GAME,
        WINNER
    };

    Screen currentScreen = Screen::MAIN_MENU;
    Screen nextScreen = Screen::MAIN_MENU;

    // =========================================================
    // TRANSITION
    // =========================================================

    Transition transition;

    transition.setFont(
        assets.getLoadingFont());

    // =========================================================
    // RETURN TO MAIN MENU
    // =========================================================

    auto returnToMainMenu = [&]()
    {
        // -----------------------------------------------------
        // Reset game
        // -----------------------------------------------------

        game.resetGame();

        // -----------------------------------------------------
        // Reset GameScreen
        // -----------------------------------------------------

        gameScreen.resetState();

        // -----------------------------------------------------
        // Recreate MainMenu
        // -----------------------------------------------------

        mainMenu =
            MainMenu(
                &assets,
                &game);

        // -----------------------------------------------------
        // Restart music
        // -----------------------------------------------------

        if (IsMusicValid(gameMusic))
        {
            StopMusicStream(gameMusic);
            PlayMusicStream(gameMusic);
        }

        // -----------------------------------------------------
        // Reset winner state
        // -----------------------------------------------------

        winnerTransitionStarted = false;
        pendingWinnerHero = nullptr;

        // -----------------------------------------------------
        // Reset screen
        // -----------------------------------------------------

        currentScreen = Screen::MAIN_MENU;
        nextScreen = Screen::MAIN_MENU;

        // -----------------------------------------------------
        // Reset transition
        // -----------------------------------------------------

        transition = Transition();

        transition.setFont(
            assets.getLoadingFont());
    };

    // =========================================================
    // MAIN LOOP
    // =========================================================

    while (!WindowShouldClose())
    {
        try
        {
            float deltaTime = GetFrameTime();

            // =====================================================
            // UPDATE MUSIC
            // =====================================================

            if (IsMusicValid(gameMusic))
            {
                UpdateMusicStream(gameMusic);
            }

            // =====================================================
            // TRANSITION
            // =====================================================

            if (transition.isActive())
            {
                transition.update(deltaTime);

                if (transition.shouldSwitch())
                {
                    currentScreen = nextScreen;

                    // -------------------------------------------------
                    // Entering GAME
                    // -------------------------------------------------

                    if (currentScreen == Screen::GAME)
                    {
                        if (IsMusicValid(gameMusic))
                        {
                            StopMusicStream(gameMusic);
                        }

                        gameScreen.resetPlayerPanelAnimations();
                    }

                    // -------------------------------------------------
                    // Finish transition
                    // -------------------------------------------------

                    transition.finishSwitch();

                    // -------------------------------------------------
                    // Entering WINNER
                    // -------------------------------------------------

                    if (currentScreen == Screen::WINNER)
                    {
                        winnerUI.open(
                            pendingWinnerHero);
                    }
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

                if (currentScreen == Screen::MAIN_MENU)
                {
                    mainMenu.update();

                    int result =
                        mainMenu.handleInput();

                    // -------------------------------------------------
                    // EXIT
                    // -------------------------------------------------

                    if (result == 3)
                    {
                        break;
                    }

                    // -------------------------------------------------
                    // START GAME - NORMAL
                    // -------------------------------------------------

                    else if (result == 4)
                    {
                        game.beginTurns();

                        nextScreen = Screen::GAME;

                        transition.setText(
                            "GET READY FIGHTERS . . .");

                        transition.start(
                            TransitionType::Fade,
                            1.0f,
                            2.5f);
                    }

                    // -------------------------------------------------
                    // START GAME - OTHER
                    // -------------------------------------------------

                    else if (result == 5)
                    {
                        nextScreen = Screen::GAME;

                        transition.setText(
                            "GET READY FIGHTERS . . .");

                        transition.start(
                            TransitionType::Fade,
                            1.0f,
                            2.5f);
                    }
                }

                // =================================================
                // GAME
                // =================================================

                else if (currentScreen == Screen::GAME)
                {
                    int result =
                        gameScreen.update();

                    // -------------------------------------------------
                    // CHECK GAME OVER
                    // -------------------------------------------------

                    if (!winnerTransitionStarted)
                    {
                        Hero *winnerHero = nullptr;

                        if (
                            gameScreen.consumeGameOver(
                                winnerHero))
                        {
                            pendingWinnerHero =
                                winnerHero;

                            winnerTransitionStarted =
                                true;

                            // -----------------------------------------
                            // Winner text
                            // -----------------------------------------

                            transition.setText("GAME OVER");

                            nextScreen =
                                Screen::WINNER;

                            transition.start(
                                TransitionType::Fade,
                                1.0f,
                                2.0f);
                        }
                    }

                    // -------------------------------------------------
                    // EXIT GAME
                    // -------------------------------------------------

                    if (result == 1)
                    {
                        game.resetGame();

                        gameScreen.resetState();

                        mainMenu =
                            MainMenu(
                                &assets,
                                &game);

                        // Restart music
                        if (IsMusicValid(gameMusic))
                        {
                            StopMusicStream(gameMusic);
                            PlayMusicStream(gameMusic);
                        }

                        nextScreen =
                            Screen::MAIN_MENU;

                        transition.setText(
                            "LOADING . . .");

                        transition.start(
                            TransitionType::Fade,
                            1.0f,
                            1.5f);
                    }
                }

                // =================================================
                // WINNER
                // =================================================

                else if (currentScreen == Screen::WINNER)
                {
                    winnerUI.update();

                    // -------------------------------------------------
                    // Return to Main Menu
                    // -------------------------------------------------

                    if (
                        winnerUI.consumeReturnToMenu())
                    {
                        game.resetGame();

                        gameScreen.resetState();

                        mainMenu =
                            MainMenu(
                                &assets,
                                &game);

                        // Restart music
                        if (IsMusicValid(gameMusic))
                        {
                            StopMusicStream(gameMusic);
                            PlayMusicStream(gameMusic);
                        }

                        // Reset winner state
                        winnerTransitionStarted =
                            false;

                        pendingWinnerHero =
                            nullptr;

                        // Return to Main Menu
                        currentScreen =
                            Screen::MAIN_MENU;

                        nextScreen =
                            Screen::MAIN_MENU;

                        // Reset transition
                        transition =
                            Transition();

                        transition.setFont(
                            assets.getLoadingFont());
                    }
                }
            }

            // =====================================================
            // DRAW
            // =====================================================

            BeginDrawing();

            ClearBackground(BLACK);

            // -----------------------------------------------------
            // MAIN MENU
            // -----------------------------------------------------

            if (currentScreen == Screen::MAIN_MENU)
            {
                mainMenu.draw();
            }

            // -----------------------------------------------------
            // GAME
            // -----------------------------------------------------

            else if (currentScreen == Screen::GAME)
            {
                gameScreen.draw();
            }

            // -----------------------------------------------------
            // WINNER
            // -----------------------------------------------------

            else if (currentScreen == Screen::WINNER)
            {
                winnerUI.draw();
            }

            // -----------------------------------------------------
            // TRANSITION
            // -----------------------------------------------------

            transition.draw();

            EndDrawing();
        }

        // =========================================================
        // EXCEPTION HANDLING
        // =========================================================

        catch (const std::exception &e)
        {
            std::cerr
                << "\n[!] UNHANDLED ERROR : "
                << e.what()
                << std::endl;

            returnToMainMenu();
        }

        catch (...)
        {
            std::cerr
                << "\n[!] UNKNOWN UNHANDLED ERROR!"
                << std::endl;

            returnToMainMenu();
        }
    }

    // =========================================================
    // CLEANUP
    // =========================================================

    assets.unload();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}