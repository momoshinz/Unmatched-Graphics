#pragma once

#include <raylib.h>
#include <string>

#include "graphics/AssetManager.h"

class MainMenu
{
private:
    AssetManager *assets;

    void drawPlayerInput(
    Font font,
    const char* playerTitle,
    const std::string& name,
    const std::string& age,
    bool enteringName,
    bool enteringAge,
    const char* buttonText);

    enum class State
    {
        MAIN_MENU,
        PLAYER_1_INPUT,
        PLAYER_2_INPUT,
        READY
    };

    State state;

    // -----------------------------
    // Player 1
    // -----------------------------

    std::string player1Name;
    std::string player1Age;

    // -----------------------------
    // Player 2
    // -----------------------------

    std::string player2Name;
    std::string player2Age;

    // -----------------------------
    // Input
    // -----------------------------

    bool enteringName;
    bool enteringAge;

    // -----------------------------
    // Helper
    // -----------------------------

    bool isPlayer1Complete() const;
    bool isPlayer2Complete() const;


public:
    MainMenu(AssetManager *assets);

    void update();
    void draw();

    int handleInput();

};