#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <raylib.h>
#include "graphics/AssetManager.h"

class GameScreen
{
private:
    AssetManager *assets;

    void drawMap();
    void drawPlayerPanels();
    void drawTopButtons();

public:
    GameScreen(AssetManager *assets);

    int update();
    void draw();
};

#endif