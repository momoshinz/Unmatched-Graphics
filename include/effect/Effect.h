#pragma once
#include <string>
#include <vector>

class Fighter;
class Game;
class Card;
class Space;

using namespace std;

struct EffectChoice
{
    Space *selectedSpace = nullptr;
    Space *secondSpace = nullptr;
    Fighter *selectedFighter = nullptr;
    int selectedCardIndex = -1;
    vector<int> selectedCardIndices;
    int selectedOrder = 0;
    int selectedFogId = -1;
    int selectedOptionIndex = -1;
};

enum class EffectInputKind
{
    None,
    ChooseAdjacentEmptySpace,
    ChooseReachableSpace,
    ChooseOpponentCardToBurn,
    ChooseEnemyFighter,
    ChooseTwoCardsAndOrder,
    ChooseFighterMoveThenFogMove,
    ChooseLurkingOption,
    ChooseFogSourceAndDestination,
    ChooseFogAndDestination,
    ChooseEnemyAndFogDestination,
    ChooseDefeatedSisterAndZoneSpace,
    ChooseCardsToDiscard,
    ChooseAnyEmptySpace,
    ChooseFighterAndReachableSpace,
    ChooseTargetAdjacentEmptySpace,
    ShowOpponentHand   
};

class Effect
{
public:
    Effect();
    virtual ~Effect() = default;

    virtual void apply(Game &game,
                       Fighter &fighter,
                       Fighter &target,
                       const Card &self,
                       Card *opponentCard,
                       bool didUserWin,
                       const EffectChoice &choice) = 0;

    virtual EffectInputKind getInputKind() const { return EffectInputKind::None; }
    virtual bool shouldRequestInput(Game &game, Fighter &user, Fighter &target, bool didUserWin) const
    {
        return true;
    }
    virtual int getMoveRange() const { return 0; }
    virtual int getFogMoveRange() const { return 0; }

    virtual string getDescription() const = 0;
    virtual Effect *clone() const = 0;
};