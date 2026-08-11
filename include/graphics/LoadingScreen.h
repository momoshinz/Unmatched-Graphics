#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H

#include <raylib.h>

class AssetManager;

class LoadingScreen
{
    private:
        AssetManager *assets;

        float elapsedTime;
        float duration;

    public:
        LoadingScreen(AssetManager *assets, float duration = 3.0f);

        void update();

        void draw();

        bool isFinished() const;

        void reset();
};

#endif