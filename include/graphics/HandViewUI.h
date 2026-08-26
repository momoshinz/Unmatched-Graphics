#pragma once

#include "raylib.h"
#include <vector>
#include <string>

class AssetManager;
class Player;
class Card;

class HandViewUI
{
public:
    explicit HandViewUI(AssetManager *assets);

    void open(Player *player);
    void update();
    void draw();

    bool isOpen() const;
    void close();

private:
    AssetManager *assets;
    bool open_ = false;

    Player *player = nullptr;

    std::vector<Card *> cards;
    std::vector<Rectangle> cardBoxes;
    Rectangle backButton{};

    static std::string getCardTextureKeyForHandView(const Card *card, const std::string &heroName);
    void layout();
};