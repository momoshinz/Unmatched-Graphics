#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "raylib.h"

class AssetManager
{
    public:
        Texture2D mainMenuBackground;
        Font gameFont;

        void load();
        void unload();
};

#endif