#pragma once

#include "raylib.h"
#include <vector>
#include <string>

class AssetManager;
class Game;
class Hero;
class Player;
class Fighter;

enum class DraculaAbilityState
{
    CLOSED,
    ASK,
    NO_TARGET_MESSAGE,
    SELECT_TARGET
};

class DraculaAbilityUI
{
public:
    explicit DraculaAbilityUI(AssetManager *assets);

    void open(Game *game, Hero *dracula, Player *player);
    void update();
    void draw();

    bool isOpen() const;
    bool consumeFinished();

private:
    AssetManager *assets;
    DraculaAbilityState state = DraculaAbilityState::CLOSED;

    Game *game = nullptr;
    Hero *dracula = nullptr;
    Player *player = nullptr;

    bool finished = false;

    Rectangle yesButton{};
    Rectangle noButton{};
    Rectangle backButton{};
    Rectangle confirmButton{};

    std::vector<Fighter *> candidateFighters;
    std::vector<Rectangle> fighterBoxes;
    int selectedIndex = -1;

    void layoutAskButtons();
    void layoutNoTargetMessage();
    void layoutTargetSelection();

    void applyAbility(Fighter *target);
};