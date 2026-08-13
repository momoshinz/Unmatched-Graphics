#ifndef GAMESCREEN_H
#define GAMESCREEN_H
#include <raylib.h>
#include "graphics/AssetManager.h"
#include "game/Game.h"

class GameScreen
{
private:
    AssetManager *assets;
    Game *game;

    bool guideOpen = false;

    void drawMap();
    void drawPlayerPanels();
    void drawTopButtons();
    void drawGuidePopup();
    void drawSpaces();
    void drawPlacedFighters();

    Vector2 mapImageToScreen(Vector2 imagePosition) const;

public:
    GameScreen(
        AssetManager *assets,
        Game *game);

    int update();
    void draw();

    void calculateMapTransform(
        float &mapX,
        float &mapY,
        float &scale,
        float &mapWidth,
        float &mapHeight) const;

    int getClickedSpaceId();
};

#endif