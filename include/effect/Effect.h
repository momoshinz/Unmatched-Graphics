#ifndef EFFECT_H
#define EFFECT_H
#include <string>

class Fighter;
class Game;
class Card;
class Space;

using namespace std;

// ============================================================
// اطلاعاتی که GUI از قبل جمع می‌کنه و به apply() می‌ده
// ============================================================
struct EffectChoice
{
    Space *selectedSpace = nullptr;     // خونه‌ی انتخاب‌شده (AdministerAid, TheGameIsAfoot)
    int selectedCardIndex = -1;         // ایندکس کارت انتخاب‌شده از یک دست (EliminateTheImpossible)
    Fighter *selectedFighter = nullptr; // فایتر انتخاب‌شده (MasterOfDisguise)
};

// ============================================================
// نوع ورودی‌ای که این افکت از UI نیاز داره
// ============================================================
enum class EffectInputKind
{
    None,                     // خودکار، نیازی به تعامل نیست
    ChooseAdjacentEmptySpace, // انتخاب یک خونه‌ی خالی مجاورِ یک فایتر مشخص (AdministerAid)
    ChooseOpponentCardToBurn, // انتخاب یک کارت از دست حریف (EliminateTheImpossible)
    ChooseEnemyFighter,       // انتخاب یک فایتر از حریف (MasterOfDisguise)
    ChooseReachableSpace      // انتخاب یک خونه در محدوده‌ی حرکتِ خود فایتر (TheGameIsAfoot)
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

    virtual EffectInputKind getInputKind() const
    {
        return EffectInputKind::None;
    }

    // فقط برای افکت‌هایی که ChooseReachableSpace هستن معنا داره
    virtual int getMoveRange() const
    {
        return 0;
    }

    virtual string getDescription() const = 0;
    virtual Effect *clone() const = 0;
};

#endif