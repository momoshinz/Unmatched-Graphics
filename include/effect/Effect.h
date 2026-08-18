#ifndef EFFECT_H
#define EFFECT_H
#include <string>
#include <vector>

class Fighter;
class Game;
class Card;
class Space;

using namespace std;

struct EffectChoice
{
    Space *selectedSpace = nullptr;     // خونه‌ی اصلی (حرکت فایتر یا مقصد فاگ)
    Space *secondSpace = nullptr;       // خونه‌ی دوم (وقتی هم فایتر هم فاگ حرکت می‌کنن)
    Fighter *selectedFighter = nullptr; // فایتر انتخاب‌شده (دشمن یا هدف)
    int selectedCardIndex = -1;         // یک کارت از یک دست
    vector<int> selectedCardIndices;    // چند کارت (مثل CodedNotes)
    int selectedOrder = 0;              // ترتیب انتخابی (CodedNotes)
    int selectedFogId = -1;             // ایندکس فاگِ انتخابی در player->getFogs()
    int selectedOptionIndex = -1;       // انتخاب بین چند گزینه (مثل Lurking)
};

enum class EffectInputKind
{
    None,
    ChooseAdjacentEmptySpace,
    ChooseReachableSpace,
    ChooseOpponentCardToBurn,
    ChooseEnemyFighter,
    ChooseTwoCardsAndOrder,        // CodedNotes
    ChooseFighterMoveThenFogMove,  // IntoThinAir
    ChooseLurkingOption,           // Lurking
    ChooseFogSourceAndDestination, // RollingFog
    ChooseFogAndDestination,       // SlipAway
    ChooseEnemyAndFogDestination,   // StepLightly
    ChooseDefeatedSisterAndZoneSpace, // Baptism Of Blood
    ChooseCardsToDiscard,
    ChooseAnyEmptySpace,
    ChooseFighterAndReachableSpace,
    ChooseTargetAdjacentEmptySpace
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
    virtual int getMoveRange() const { return 0; }    // بُرد حرکت فایتر
    virtual int getFogMoveRange() const { return 0; } // بُرد حرکت فاگ

    virtual string getDescription() const = 0;
    virtual Effect *clone() const = 0;
};

#endif