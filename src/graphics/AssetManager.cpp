#include "graphics/AssetManager.h"

void AssetManager::load()
{
    mainMenuBackground = LoadTexture("assets/backgrounds/main_menu.png");
    gameFont = LoadFont("assets/fonts/game_font.ttf");
}

void AssetManager::unload()
{
    UnloadTexture(mainMenuBackground);
    UnloadFont(gameFont);
}