#pragma once
#include "raylib.h"
#include <vector>
#include <string>

class AssetManager;
class Hand;
class Card;

class SchemeUI
{
public:
    explicit SchemeUI(AssetManager *assets);

    void openScheme(const Hand &hand);
    void update();
    void draw();

    bool isOpen() const;
    bool isConfirmed() const;
    void resetConfirmed();
    Card *getSelectedCard() const;

private:
    AssetManager *assets;

    bool open = false;
    bool confirmed = false;

    std::vector<Card *> selectableCards;
    std::vector<Rectangle> cardBoxes;

    int selectedIndex = -1;

    Rectangle playButton{};

    static std::string getCardTextureKey(const Card *card);
};