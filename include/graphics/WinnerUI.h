#pragma once
#include "raylib.h"
#include <string>

class AssetManager;
class Hero;

class WinnerUI
{
private:
    AssetManager *assets;
    bool open_ = false;
    bool returnRequested = false;

    Hero *winnerHero = nullptr;
    Rectangle returnTextBounds{};

public:
    explicit WinnerUI(AssetManager *assets);

    void open(Hero *winnerHero);
    void update();
    void draw();
    static Texture2D getHeroTextureForWinnerUI(AssetManager *assets, Hero *hero);

    bool isOpen() const;
    bool consumeReturnToMenu();
};