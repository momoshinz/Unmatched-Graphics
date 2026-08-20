#pragma once
#include "raylib.h"
#include "effect/Effect.h"
#include <vector>
#include <string>

class AssetManager;
class Game;
class Fighter;
class Card;
class Space;
class Player;
class Fog;

class EffectUI
{
private:
    AssetManager *assets;
    Game *game = nullptr;
    Effect *effect = nullptr;
    Fighter *fighter = nullptr;
    Fighter *target = nullptr;
    Player *actingPlayer = nullptr;

    EffectInputKind inputKind = EffectInputKind::None;
    EffectChoice choice;

    bool open_ = false;
    bool ready = false;

    int subPhase = 0;
    int pendingFirstCardIndex = -1;

    // ---- خونه‌های قابل‌کلیک روی نقشه (مشترک بین چند نوع) ----
    std::vector<Space *> candidateSpaces;
    int fogMoveRange = 0;

    // ---- انتخاب کارت (دست حریف برای سوزوندن / دست خود برای CodedNotes) ----
    std::vector<Card *> candidateCards;
    std::vector<Rectangle> cardBoxes;
    int selectedCardIndex = -1;
    Rectangle confirmButton{};

    // ---- انتخاب فایتر (دشمن / دشمن مجاور) ----
    std::vector<Fighter *> candidateFighters;
    std::vector<Rectangle> fighterBoxes;

    // ---- انتخاب فاگ ----
    std::vector<Fog *> candidateFogs;
    std::vector<Rectangle> fogBoxes;
    Fog *selectedFog = nullptr;
    Space *fogSourceSpace = nullptr;

    // ---- دکمه‌های عمومی Yes/No و دو-گزینه‌ای ----
    Rectangle yesButton{};
    Rectangle noButton{};
    Rectangle optionButtonA{};
    Rectangle optionButtonB{};
    std::string optionTextA;
    std::string optionTextB;

    static std::string getCardTextureKey(const Card *card, const std::string &heroName);

    // ---- setup برای هر نوع ----
    void setupChooseAdjacentEmptySpace();
    void setupChooseReachableSpace();
    void setupChooseOpponentCardToBurn();
    void setupChooseEnemyFighter();
    void setupChooseTwoCardsAndOrder();
    void setupChooseFighterMoveThenFogMove();
    void setupChooseLurkingOption();
    void setupChooseFogSourceAndDestination();
    void setupChooseFogAndDestination();
    void setupChooseEnemyAndFogDestination();
    void setupChooseDefeatedSisterAndZoneSpace();
    void setupChooseCardsToDiscard();
    void setupChooseAnyEmptySpace();
    void setupChooseFighterAndReachableSpace();
    void setupChooseTargetAdjacentEmptySpace();

    // ---- layout کمکی ----
    void layoutCardWindow(const std::vector<Card *> &cards);
    void layoutFighterWindow(const std::vector<Fighter *> &fighters);
    void layoutFogWindow();
    void layoutYesNo();
    void layoutTwoOptions(const std::string &textA, const std::string &textB);

    void beginFogDestinationStage(int range, bool excludeSource);

    void finalizeReady();
    void setupShowOpponentHand();

public:
    explicit EffectUI(AssetManager *assets);

    void open(Game *game, Effect *effect, Fighter *fighter, Fighter *target);

    void update();
    void draw();

    bool isOpen() const;
    bool isReady() const;
    const EffectChoice &getChoice() const;

    void reset();

    void selectSpace(Space *space);

    const std::vector<Space *> &getCandidateSpaces() const;
    bool isChoosingSpace() const;

    void finishCardSelection();
};