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

        float getMapScale() const;

        Vector2 getMapPosition() const;

    public:
        GameScreen(AssetManager *assets);

        int update();
        void draw();

        void calculateMapTransform(
        float &mapX,
        float &mapY,
        float &scale,
        float &mapWidth,
        float &mapHeight);

        int getClickedSpaceId();
};

#endif