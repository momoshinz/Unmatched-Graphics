#include "game/CombatSystem.h"
#include "game/Game.h"
#include "board/Board.h"
#include "fighter/Fighter.h"
#include "player/Player.h"
#include "card/Card.h"
#include "effect/DraculaCards/FeintDracula.h"
#include "effect/SherlockCards/DeduceStrategy.h"
#include "effect/SherlockCards/FeintSherlock.h"
#include <iostream>
#include <stdexcept>

using namespace std;

CombatSystem::CombatSystem()
    : attacker(nullptr),
      defender(nullptr),
      attackCard(nullptr),
      defenceCard(nullptr),
      currentGame(nullptr),
      combatResolved(false)
{
}

void CombatSystem::setGame(Game *game)
{
    currentGame = game;
}

bool CombatSystem::isInSameZone(const Fighter *attacker, const Fighter *defender) const
{
    if (attacker == nullptr || defender == nullptr)
    {
        return false;
    }

    if (attacker->getPosition() == nullptr || defender->getPosition() == nullptr)
    {
        return false;
    }

    int first = attacker->getPosition()->getId();

    int second = defender->getPosition()->getId();

    return currentGame->getBoard().sameZone(first, second);
}

bool CombatSystem::isInAttackRange(const Fighter *attacker, const Fighter *defender) const
{
    if (attacker == nullptr || defender == nullptr)
    {
        return false;
    }

    if (attacker->isMelee())
    {
        return attacker->isAdjacent(defender, currentGame->getBoard());
    }

    if (attacker->isRanged())
    {
        return attacker->isAdjacent(defender, currentGame->getBoard()) || isInSameZone(attacker, defender);
    }

    return false;
}

bool CombatSystem::isAttackValid() const
{
    if (attacker == nullptr)
    {
        return false;
    }

    if (defender == nullptr)
    {
        return false;
    }

    if (attackCard == nullptr)
    {
        return false;
    }

    if (!attacker->isAlive())
    {
        return false;
    }

    if (!defender->isAlive())
    {
        return false;
    }

    if (!isInAttackRange(attacker, defender))
    {
        return false;
    }

    return true;
}

int CombatSystem::calculateFinalAttackValue(const Card &card, const Fighter &user) const
{
    return card.getCombatValue() + user.getTempAttackBoost();
}

int CombatSystem::calculateFinalDefenseValue(const Card &card, const Fighter &user) const
{
    if (user.shouldUseOpponentBoostValue())
    {
        int baseDefence = card.getCombatValue();
        int opponentBoost = user.getOpponentBoostValue();
        return baseDefence + opponentBoost;
    }
    return card.getCombatValue();
}

int CombatSystem::calculateDamage(int attackValue, int defenceValue) const
{
    int damage = attackValue - defenceValue;

    if (damage < 0)
    {
        damage = 0;
    }

    return damage;
}

void CombatSystem::applyDamage(int damage, Fighter &defender)
{
    if (damage <= 0)
    {
        return;
    }

    defender.takeDamage(damage);

    cout << "\n[-] " << defender.getName() << " takes " << damage << " damage!\n";
}

void CombatSystem::applyEffects(Timing timing, Fighter &user, Fighter &target, Card &card, bool didUserWin)
{
    if (card.isEffectsCanceled())
    {
        cout << "\n[!] Effects of " << card.getName() << " are canceled by the Feint!\n";
        return;
    }

    if (!card.hasEffect())
    {
        return;
    }

    Effect *effect = card.getEffect();

    if (effect == nullptr)
    {
        return;
    }

    if (card.getTiming() != timing)
    {
        return;
    }

    Card* opponentCard = nullptr;
    if(&card == attackCard)
    {
        opponentCard = defenceCard;
    }
    else if(&card == defenceCard)
    {
        opponentCard = attackCard;
    }

    if(opponentCard == nullptr)
    {
        if(dynamic_cast<FeintDracula*>(effect) != nullptr || 
           dynamic_cast<FeintSherlock*>(effect) != nullptr ||
           dynamic_cast<DeduceStrategy*>(effect) != nullptr)
           {
                return;
           }
    }

    effect->apply(*currentGame, user, target, card, opponentCard, didUserWin);
}

bool CombatSystem::canFighterPlayCard(const Fighter &fighter, const Card &card) const
{
    return card.isPlayableBy(fighter);
}

void CombatSystem::resolveCombat(Game &game, Fighter &attacker, Fighter &defender, Card &attackCard, Card *defenceCard)
{
    this->currentGame = &game;
    this->attacker = &attacker;
    this->defender = &defender;
    this->attackCard = &attackCard;
    this->defenceCard = defenceCard;
    this->combatResolved = false;

    cout << "\n========== COMBAT INITIATED ==========\n";

    if (!isAttackValid())
    {
        throw logic_error("\n[!] ERROR : Invalid attack!\n");
    }

    cout << "\n> Attacker : " << attacker.getName() << endl;

    attackCard.display();

    cout << "\n> Defender : " << defender.getName() << endl;

    if (defenceCard != nullptr)
    {
        defenceCard->display();
    }
    else
    {
        cout << "[!] Defender chose NOT to defend!\n";
    }

    if (defenceCard != nullptr && !defenceCard->isEffectsCanceled())
    {
        applyEffects(Timing::Immediately, defender, attacker, *defenceCard, false);
    }

    if(!attackCard.isEffectsCanceled())
    {
        applyEffects(Timing::Immediately, attacker, defender, attackCard, false);
    }

    if (defenceCard != nullptr && !defenceCard->isEffectsCanceled())
    {
        applyEffects(Timing::DuringCombat, defender, attacker, *defenceCard, false);
    }

    if(!attackCard.isEffectsCanceled())
    {
        applyEffects(Timing::DuringCombat, attacker, defender, attackCard, false);
    }
    int attackValue;

    if (defender.shouldUseOpponentBoostValue())
    {
        attackValue = attackCard.getBoost();
    }
    else
    {
        attackValue = calculateFinalAttackValue(attackCard, attacker);
    }

    int defenceValue = 0;

    if (defenceCard != nullptr)
    {
        if (attacker.shouldUseOpponentBoostValue())
        {
            defenceValue = defenceCard->getBoost();
        }
        else
        {
            defenceValue = calculateFinalDefenseValue(*defenceCard, defender);
        }
    }
    else
    {
        defenceValue = 0;
    }

    cout << "\n-> Attack Value : " << attackValue << endl;

    cout << "-> Defense Value : " << defenceValue << endl;

    int damage = calculateDamage(attackValue, defenceValue);
    bool attackerWon = (damage > 0);
    applyDamage(damage, defender);

    if (defenceCard != nullptr && !defenceCard->isEffectsCanceled())
    {
        applyEffects(Timing::AfterCombat, defender, attacker, *defenceCard, !attackerWon);
    }

    if(!attackCard.isEffectsCanceled())
    {
        applyEffects(Timing::AfterCombat, attacker, defender, attackCard, attackerWon);

    }
    attackCard.resetEffectsCancel();
    if (defenceCard != nullptr)
    {
        defenceCard->resetEffectsCancel();
    }

    attacker.resetTempAttackBoost();
    defender.resetTempAttackBoost();

    attacker.disableOpponentBoostValue();
    defender.disableOpponentBoostValue();

    Player *attackerPlayer = attacker.getOwner();
    Player *defenderPlayer = defender.getOwner();
    attackerPlayer->getDiscardPile().addCard(&attackCard);
    if (defenceCard != nullptr)
    {
        defenderPlayer->getDiscardPile().addCard(defenceCard);
    }
    combatResolved = true;

    cout << "\n========== COMBAT FINISHED ==========\n";
    cout << "\n[ - RESULT - ]\n";

    cout << "[o] " << attacker.getName() << " DEALT " << damage << " damage to " << defender.getName() << ".\n";

    cout << "[o] " << defender.getName() << " Health : " << defender.getHealth() << "/" << defender.getMaxHealth() << endl;

    cout << "\n[ - WINNER - ]\n";
    if (damage > 0)
    {
        cout << "[*] " << attacker.getName() << " WINS THE COMBAT!\n";
    }

    else
    {
        cout << "[*] " << defender.getName() << " WINS THE COMBAT!\n";
    }

    if (!defender.isAlive())
    {
        cout << "-{ " << defender.getName() << " has been DEFEATED! }-\n";
    }
}