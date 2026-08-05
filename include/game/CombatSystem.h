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

class CombatSystem
{
    private:
        Fighter *attacker;
        Fighter *defender;
        Card *attackCard;
        Card *defenceCard;
        Game *currentGame;
        bool combatResolved;

    public:
        CombatSystem();
        ~CombatSystem() = default;

        void setGame(Game* game);
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

        Fighter *getAttacker() const;
        Fighter *getDefender() const;
        Card *getAttackCard() const;
        Card *getDefenceCard() const;
        bool isCombatResolved() const;
        bool canFighterPlayCard(const Fighter &fighter, const Card &card) const;

};
#endif