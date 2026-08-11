#pragma once
#include <raylib.h>
#include "graphics/AssetManager.h"

class MainMenu
{
private:
    AssetManager *assets;

public:
    MainMenu(AssetManager *assets);

    void update();
    void draw();

    int handleInput() const;
};