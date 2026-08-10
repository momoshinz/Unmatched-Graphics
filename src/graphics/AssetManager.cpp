#include "graphics/AssetManager.h"

AssetManager::AssetManager()
    : mainMenuBackground{},
      loadingBackground{},
      board{},
      gameFont{},
      loaded(false)
{
}

bool AssetManager::load()
{
    mainMenuBackground = LoadTexture("Unmatched_Assets/main_menu.png");

    loadingBackground = LoadTexture("Unmatched_Assets/loading.png");

    board = LoadTexture("Unmatched_Assets/board.png");

    gameFont = LoadFont("Unmatched_Assets/fonts/game_font.ttf");

    if (mainMenuBackground.id == 0 || loadingBackground.id == 0 || board.id == 0 || gameFont.texture.id == 0)
    {
        unload();
        return false;
    }

    loaded = true;
    return true;
}

void AssetManager::unload()
{
    if (mainMenuBackground.id != 0)
    {
        UnloadTexture(mainMenuBackground);
        mainMenuBackground = {};
    }

    if (loadingBackground.id != 0)
    {
        UnloadTexture(loadingBackground);
        loadingBackground = {};
    }

    if (board.id != 0)
    {
        UnloadTexture(board);
        board = {};
    }

    if (gameFont.texture.id != 0)
    {
        UnloadFont(gameFont);
        gameFont = {};
    }

    loaded = false;
}

AssetManager::~AssetManager()
{
    unload();
}

Texture2D AssetManager::getMainMenuBackground() const
{
    return mainMenuBackground;
}

Texture2D AssetManager::getLoadingBackground() const
{
    return loadingBackground;
}

Texture2D AssetManager::getBoard() const
{
    return board;
}

Font AssetManager::getGameFont() const
{
    return gameFont;
}