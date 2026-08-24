#include "effect/DraculaCards/FeintDracula.h"
#include "fighter/Fighter.h"
#include "fighter/Hero.h"
#include "game/Game.h"
#include "player/Player.h"
#include "card/Card.h"

#include <iostream>
#include <stdexcept>

using namespace std;

void FeintDracula::apply(Game &game,
                         Fighter &fighter,
                         Fighter &target,
                         const Card &self,
                         Card *opponentCard,
                         bool didUserWin,
                         const EffectChoice &choice)
{
    Player *player = fighter.getOwner();

    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    if (opponentCard == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Opponent has no card to cancel.\n");
    }

    if (!opponentCard->hasEffect())
    {
        throw runtime_error("\n[!] ERROR : Opponent's card has no effect to cancel.\n");
    }

    // --------------------------------------------------------
    // چک ایمیونیتی: شرلوک و واتسون هرگز نمی‌تونن کنسل بشن
    // --------------------------------------------------------

    Player *opponentPlayer = target.getOwner();

    if (opponentPlayer != nullptr)
    {
        Hero *opponentHero = opponentPlayer->getHero();

        if (opponentHero != nullptr && opponentHero->isAbilityImmune())
        {
            cout << "\n========================================\n";
            cout << "-< Feint >- BLOCKED!\n";
            cout << "[!] " << opponentHero->getName()
                 << "'s and Dr. Watson's abilities can never be disabled.\n";
            cout << "========================================\n";

            game.setFeintBlocked(
                opponentHero->getName() +
                " and Dr. Watson's abilities can never be disabled!");

            return;
        }
    }

    cout << "\n========================================\n";
    cout << "-< Feint >- ACTIVATED!\n";

    opponentCard->cancelEffects();

    cout << "[+] Effects of " << opponentCard->getName() << " have been canceled!\n";

    cout << "========================================\n";
}

EffectInputKind FeintDracula::getInputKind() const
{
    return EffectInputKind::None;
}

string FeintDracula::getDescription() const
{
    return "> Cancel all effects on your opponent's card.";
}

Effect *FeintDracula::clone() const
{
    return new FeintDracula(*this);
}