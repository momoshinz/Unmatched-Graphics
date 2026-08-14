#pragma once
#include "game/Game.h"
#include <raylib.h>
#include <string>
#include <vector>
#include "board/Board.h"
#include "graphics/AssetManager.h"

class MainMenu
{
private:
    AssetManager *assets;
    Game *game;
    Board *board;
    
    void drawPlayerInput(
        Font font,
        const char *playerTitle,
        const std::string &name,
        const std::string &age,
        bool enteringName,
        bool enteringAge,
        const char *buttonText);

    enum class State
    {
        MAIN_MENU,
        PLAYER_1_INPUT,
        PLAYER_2_INPUT,
        READY,
        LOAD_GAME,
        HERO_SELECTION,
        PLACEMENT
    };

    enum Placement
    {
        YOUNGER_HERO,
        YOUNGER_SIDEKICKS,
        OLDER_HERO,
        OLDER_SIDEKICKS,
        FOG,
        FINISHED
    };

    Placement placement;
    int selectedStartSpace;

    int placementPlayer;
    bool placementHeroPlaced;
    int placementStartSpace;

    int placementSidekickIndex;
    int currentFogIndex;

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

    enum class HeroSelectionPlayer
    {
        PLAYER_1,
        PLAYER_2
    };

    HeroSelectionPlayer currentHeroPlayer;

    std::string player1Hero;
    std::string player2Hero;

    bool player1HeroSelected;
    bool player2HeroSelected;

    void drawHeroSelection(Font font);

    float getMapScale() const;

    Vector2 getMapPosition() const;

    Vector2 mapImageToScreen(Vector2 imagePosition) const;

public:
    MainMenu(AssetManager *assets, Game *game);

    void update();
    void draw();

    int handleInput();

    std::string getSelectedSave() const;

    void startHeroSelection();

    void drawHeroTexture(
        Texture2D texture,
        Rectangle box);

    void drawCenteredText(
        Font font,
        const char *text,
        float x,
        float y,
        float width,
        float fontSize);

    void selectHero(const std::string &hero);

    bool bothHeroesSelected() const;

    void startPlacement();
    void updatePlacement();
    void drawPlacement(Font font);

    bool placeHeroOnSpace(int spaceId);
    bool placeSidekickOnSpace(int spaceId);
    bool placeFogOnSpace(int spaceId);
    Fog *getCurrentFog();

    bool isValidSidekickPlacement(Space *space) const;

    void finishPlacement();
    void drawPlacedFighters(Font font);
};