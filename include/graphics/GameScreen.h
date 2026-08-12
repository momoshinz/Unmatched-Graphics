#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <raylib.h>
#include "graphics/AssetManager.h"

class GameScreen
{
    private:
        AssetManager *assets;
        bool guideOpen = false;

        void drawMap();
        void drawPlayerPanels();
        void drawTopButtons();
        void drawGuidePopup();

    public:
        GameScreen(AssetManager *assets);

        int update();
        void draw();
};

#endif