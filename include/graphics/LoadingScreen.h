#pragma once
#include <raylib.h>
#include "graphics/AssetManager.h"

class LoadingScreen
{
private:
    AssetManager &assets;

    float elapsedTime;
    float duration;

public:
    LoadingScreen(AssetManager &assets, float duration);

    void update();
    void draw();

    bool isFinished() const;
    void reset();
};