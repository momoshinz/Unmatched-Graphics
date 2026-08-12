#pragma once

#include "raylib.h"
#include "graphics/AssetManager.h"

class GameScreen
{
private:
    AssetManager *assets;

public:
    GameScreen(AssetManager *assets);

    void update();
    void draw();

private:
    void drawMap();
    void drawPlayerPanels();
};