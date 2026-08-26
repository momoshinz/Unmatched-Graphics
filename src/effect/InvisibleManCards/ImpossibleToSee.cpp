#include "effect/InvisibleManCards/ImpossibleToSee.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "card/Card.h"
#include <stdexcept>
using namespace std;

void ImpossibleToSee::apply(Game &game, Fighter &fighter, Fighter &target,
                            const Card &self, Card *opponentCard, bool didUserWin,
                            const EffectChoice &choice)
{
    Player *player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Impossible To See can only be used by Invisible Man!\n");
    }

    if (opponentCard == nullptr)
    {
        return;
    }
    opponentCard->setTemporaryCombatValue(0);
}

string ImpossibleToSee::getDescription() const
{
    return "> The value of your opponents's attack or defense is 0 and cannot be changed by card effects.(other card effects still happen)";
}

Effect *ImpossibleToSee::clone() const
{
    return new ImpossibleToSee(*this);
}