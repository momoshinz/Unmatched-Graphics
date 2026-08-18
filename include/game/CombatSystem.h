#ifndef COMBATSYSTEM_H
#define COMBATSYSTEM_H
using namespace std;
#include <vector>
#include "utils/Types.h"

class Game;
class Card;
class Fighter;
class Player;
class Board;
class Effect;
struct EffectChoice;

enum class CombatPhase
{
    Idle,
    ImmediatelyDefender,
    ImmediatelyAttacker,
    DuringCombatDefender,
    DuringCombatAttacker,
    ApplyDamage,
    AfterCombatDefender,
    AfterCombatAttacker,
    Finished
};

class CombatSystem
{
private:
    Fighter *attacker;
    Fighter *defender;
    Card *attackCard;
    Card *defenceCard;
    Game *currentGame;
    bool combatResolved;

    // ---------------------------------------------
    // State machine جدید (برای پشتیبانی افکت‌های تعاملی)
    // ---------------------------------------------
    CombatPhase phase;
    bool waitingForInput;

    Effect *pendingEffect;
    Fighter *pendingUser;
    Fighter *pendingTarget;
    Card *pendingCard;
    Card *pendingOpponentCard;
    bool pendingDidUserWin;

    int lastDamage;
    bool attackerWon;

    // اجرای فازها تا جایی که ممکنه (متوقف می‌شه اگه ورودی تعاملی لازم بشه)
    void advance();

    // تلاش برای اجرای افکت یک کارت در یک تایمینگ مشخص.
    // اگه نیاز به ورودی تعاملی داشت، pending* رو ست می‌کنه و false برمی‌گردونه.
    // در غیر این صورت (اجرا شد یا رد شد) true برمی‌گردونه.
    bool tryApplyEffect(Timing timing, Fighter &user, Fighter &target, Card &card, bool didUserWin);

    void finalizeCombat();

public:
    CombatSystem();
    ~CombatSystem() = default;

    void setGame(Game *game);
    bool isAttackValid() const;
    bool isInAttackRange(const Fighter *attacker, const Fighter *defender) const;
    bool isInSameZone(const Fighter *attacker, const Fighter *defender) const;
    int calculateFinalAttackValue(const Card &card, const Fighter &user) const;
    int calculateFinalDefenseValue(const Card &card, const Fighter &user) const;
    int calculateDamage(int attackValue, int defenceValue) const;
    void applyEffects(Timing timing, Fighter &user, Fighter &target, Card &card, bool didUserWin);
    void applyDamage(int damage, Fighter &defender);

    // نسخه‌ی قدیمی (بلاکینگ) - برای سازگاری با کد ترمینالی قدیم، دست‌نخورده می‌مونه
    void resolveCombat(Game &game, Fighter &attacker, Fighter &defender,
                       Card &attackCard, Card *defenceCard);

    // ---------------------------------------------
    // API جدید (غیربلاکینگ) - برای GameScreen
    // ---------------------------------------------
    void beginCombat(Game &game, Fighter &attacker, Fighter &defender,
                     Card &attackCard, Card *defenceCard);

    bool isWaitingForEffectInput() const;
    Effect *getPendingEffect() const;
    Fighter *getPendingUser() const;
    Fighter *getPendingTarget() const;

    void provideEffectChoice(const EffectChoice &choice);

    bool isFinished() const;
    int getLastDamage() const;
    bool didAttackerWin() const;

    Fighter *getAttacker() const;
    Fighter *getDefender() const;
    Card *getAttackCard() const;
    Card *getDefenceCard() const;
    bool isCombatResolved() const;
    bool canFighterPlayCard(const Fighter &fighter, const Card &card) const;
};
#endif