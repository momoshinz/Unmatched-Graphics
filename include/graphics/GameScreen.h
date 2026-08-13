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

        Vector2 mapImageToScreen(Vector2 imagePosition) const;

    public:
        GameScreen(AssetManager *assets);

        int update();
        void draw();

        void calculateMapTransform (
        float &mapX,
        float &mapY,
        float &scale,
        float &mapWidth,
        float &mapHeight) const;

        int getClickedSpaceId();
};

#endif