#pragma once
#include "raylib.h"
#include <vector>
#include <string>
#include <utility>

class AssetManager;
class Player;
class Fighter;
class Card;
class Space;

enum class ManeuverState
{
    CLOSED,
    ASK_MOVE,
    SELECT_FIGHTER,
    ASK_BOOST,
    BURN_CARD,
    AWAITING_MOVES,
    SELECT_SPACE
};

class ManeuverUI
{
private:
    AssetManager *assets;
    ManeuverState state = ManeuverState::CLOSED;
    Player *player = nullptr;

    Rectangle yesButton{};
    Rectangle noButton{};

    std::vector<Fighter *> selectableFighters;
    std::vector<Rectangle> fighterBoxes;
    Fighter *selectedFighter = nullptr;

    std::vector<Card *> burnableCards;
    std::vector<Rectangle> burnCardBoxes;
    int burnSelectedIndex = -1;
    Rectangle burnButton{};
    int boostAmount = 0;

    std::vector<Space *> availableMoves;
    bool waitingForMoves = false;
    bool readyToFinalize = false;

public:
    explicit ManeuverUI(AssetManager *assets);

    void open(Player *player);
    void update();
    void draw();

    bool isOpen() const;
    bool isSelectingSpace() const;

    bool needsAvailableMoves() const;
    Fighter *getFighterNeedingMoves() const;
    int getMovementBudget() const;
    void beginSpaceSelection(const std::vector<Space *> &moves);

    Fighter *getSelectedFighter() const;
    const std::vector<Space *> &getAvailableMoves() const;
    static std::string getCardTextureKey(const Card *card, const std::string &heroName);

    void layoutAskButtons();
    void layoutFighterSelection();
    void layoutBurnCardWindow();
    void burnSelectedCard();

    void finishAfterMove();
    bool consumeReadyToFinalize();

    void reset();
};