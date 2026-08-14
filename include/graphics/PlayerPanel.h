#ifndef PLAYER_PANEL_H
#define PLAYER_PANEL_H

#include "raylib.h"
#include <string>

class AssetManager;
class Player;
class Hero;
class Sidekick;

class PlayerPanel
{
private:
    AssetManager *assets;
    Rectangle bounds;

    std::string getHeroTextureKey(
        const Hero *hero) const;

    std::string getSidekickTextureKey(
        const Player *player) const;

public:
    PlayerPanel(
        AssetManager *assets);

    void setBounds(Rectangle bounds);

    void draw(
        const Player *player) const;

    void drawWrappedText(
        Font font,
        const std::string &text,
        float x,
        float y,
        float maxWidth,
        float fontSize,
        float spacing,
        float lineHeight) const;
};

#endif