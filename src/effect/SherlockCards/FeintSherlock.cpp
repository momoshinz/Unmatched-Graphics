#include "effect/SherlockCards/FeintSherlock.h"
#include "fighter/Fighter.h"
#include "game/Game.h"
#include "player/Player.h"
#include "card/Card.h"
#include <stdexcept>
using namespace std;

void FeintSherlock::apply(Game &game, Fighter &fighter, Fighter &target,
                          const Card &self, Card *opponentCard, bool didUserWin,
                          const EffectChoice &choice)
{
    Player *player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    if (opponentCard == nullptr)
    {
        return;
    }

    if (!opponentCard->hasEffect())
    {
        return;
    }
    opponentCard->cancelEffects();
}

string FeintSherlock::getDescription() const
{
    return "> Cancel all effects on your opponent's card.";
}

Effect *FeintSherlock::clone() const
{
    return new FeintSherlock(*this);
}