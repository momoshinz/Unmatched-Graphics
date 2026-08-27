#ifndef COMBATSYSTEM_H
#define COMBATSYSTEM_H
using namespace std;
#include <vector>
#include <string>
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
    AwaitingResultReveal,
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

    int pendingAttackValue;
    int pendingDefenceValue;

    string currentEffectDescription;

    void advance();
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

    void resolveCombat(Game &game, Fighter &attacker, Fighter &defender,
                       Card &attackCard, Card *defenceCard);

    void beginCombat(Game &game, Fighter &attacker, Fighter &defender,
                     Card &attackCard, Card *defenceCard);

    bool isWaitingForEffectInput() const;
    Effect *getPendingEffect() const;
    Fighter *getPendingUser() const;
    Fighter *getPendingTarget() const;
    void provideEffectChoice(const EffectChoice &choice);

    bool isAwaitingResultReveal() const;
    void acknowledgeResult();
    int getPendingAttackValue() const;
    int getPendingDefenceValue() const;

    const string &getCurrentEffectDescription() const;

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