#pragma once

#include <raylib.h>
#include <string>
#include <vector>

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
            READY,
            LOAD_GAME
        };

        State state;

        std::vector<std::string> saveFiles;
        int selectedSave;

        std::string player1Name;
        std::string player1Age;
        std::string player2Name;
        std::string player2Age;

        bool enteringName;
        bool enteringAge;

        bool isPlayer1Complete() const;
        bool isPlayer2Complete() const;


    public:
        MainMenu(AssetManager *assets);

        void update();
        void draw();

        int handleInput();

        std::string getSelectedSave() const;

};