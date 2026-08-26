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

    InitWindow(screenWidth, screenHeight, "UNMATCHED");
    InitAudioDevice();
    Image icon = LoadImage("Unmatched_Assets/icon.png");

    if (icon.data != nullptr)
    {
        SetWindowIcon(icon);
        UnloadImage(icon);
    }
    SetTargetFPS(60);

    AssetManager assets;
    if (!assets.load())
    {
        CloseAudioDevice();
        CloseWindow();
        return 1;
    }

    LoadingScreen loadingScreen(assets, 5.0f);

    while (!WindowShouldClose() && !loadingScreen.isFinished())
    {
        loadingScreen.update();
        BeginDrawing();
        ClearBackground(BLACK);
        loadingScreen.draw();
        EndDrawing();
    }

    Music gameMusic = assets.getGameMusic();
    if (IsMusicValid(gameMusic))
    {
        PlayMusicStream(gameMusic);
    }

    Game game;
    MainMenu mainMenu(&assets, &game);
    GameScreen gameScreen(&assets, &game);

    enum class Screen
    {
        MAIN_MENU,
        GAME
    };

    Screen currentScreen = Screen::MAIN_MENU;
    Screen nextScreen = Screen::MAIN_MENU;

    Transition transition;
    transition.setFont(assets.getLoadingFont());

    auto returnToMainMenu = [&]()
    {
        game.resetGame();
        mainMenu = MainMenu(&assets, &game);

        if (IsMusicValid(gameMusic))
        {
            StopMusicStream(gameMusic);
            PlayMusicStream(gameMusic);
        }

        currentScreen = Screen::MAIN_MENU;
        nextScreen = Screen::MAIN_MENU;

        transition = Transition();
        transition.setFont(assets.getLoadingFont());
    };

    // whenever an error occurs during the game this try-catch brings it back to the main menu
    while (!WindowShouldClose())
    {
        try
        {
            float deltaTime = GetFrameTime();

            if (IsMusicValid(gameMusic))
            {
                UpdateMusicStream(gameMusic);
            }

            if (transition.isActive())
            {
                transition.update(deltaTime);
                if (transition.shouldSwitch())
                {
                    currentScreen = nextScreen;
                    if (currentScreen == Screen::GAME)
                    {
                        if (IsMusicValid(gameMusic))
                        {
                            StopMusicStream(gameMusic);
                        }
                        gameScreen.resetPlayerPanelAnimations();
                    }
                    transition.finishSwitch();
                }
            }

            else
            {
                if (currentScreen == Screen::MAIN_MENU)
                {
                    mainMenu.update();
                    int result = mainMenu.handleInput();
                    if (result == 3)
                    {
                        break;
                    }

                    else if (result == 4)
                    {
                        game.beginTurns();
                        nextScreen = Screen::GAME;
                        transition.setText("GET READY FIGHTERS . . .");

                        transition.start(TransitionType::Fade, 1.0f, 2.5f);
                    }

                    else if (result == 5)
                    {
                        nextScreen = Screen::GAME;
                        transition.setText("GET READY FIGHTERS . . .");
                        transition.start(TransitionType::Fade, 1.0f, 2.5f);
                    }
                }

                else if (currentScreen == Screen::GAME)
                {
                    int result = gameScreen.update();
                    if (result == 1)
                    {
                        game.resetGame();
                        mainMenu = MainMenu(&assets, &game);

                        if (IsMusicValid(gameMusic))
                        {
                            StopMusicStream(gameMusic);
                            PlayMusicStream(gameMusic);
                        }

                        nextScreen = Screen::MAIN_MENU;
                        transition.setText("LOADING . . .");
                        transition.start(TransitionType::Fade, 1.0f, 1.5f);
                    }
                }
            }

            BeginDrawing();
            ClearBackground(BLACK);

            if (currentScreen == Screen::MAIN_MENU)
            {
                mainMenu.draw();
            }
            else if (currentScreen == Screen::GAME)
            {
                gameScreen.draw();
            }

            transition.draw();
            EndDrawing();
        }
        catch (const std::exception &e)
        {
            std::cerr << "\n[!] UNHANDLED ERROR : " << e.what();
            returnToMainMenu();
        }
        catch (...)
        {
            std::cerr << "\n[!] UNKNOWN UNHANDLED ERROR!";
            returnToMainMenu();
        }
    }

    assets.unload();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}