#pragma once
#include <raylib.h>

class AssetManager
{
public:
    AssetManager();
    ~AssetManager();

    bool load();

    void unload();

    Texture2D getMainMenuBackground() const;
    Texture2D getLoadingBackground() const;
    Texture2D getBoard() const;

    Font getGameFont() const;

private:
    Texture2D mainMenuBackground;
    Texture2D loadingBackground;
    Texture2D board;

    Font gameFont;

    bool loaded;
};