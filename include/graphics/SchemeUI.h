#pragma once
#include "raylib.h"
#include <vector>
#include <string>

class AssetManager;
class Hand;
class Card;

class SchemeUI
{
private:
    AssetManager *assets;

    bool open = false;
    bool confirmed = false;

    std::vector<Card *> selectableCards;
    std::vector<Rectangle> cardBoxes;

    int selectedIndex = -1;

    Rectangle playButton{};

    static std::string getCardTextureKey(const Card *card);
    Rectangle backButton{};
    bool emptyMessage = false;

public:
    explicit SchemeUI(AssetManager *assets);

    void openScheme(const Hand &hand);
    void update();
    void draw();

    bool isOpen() const;
    bool isConfirmed() const;
    void resetConfirmed();
    Card *getSelectedCard() const;
};