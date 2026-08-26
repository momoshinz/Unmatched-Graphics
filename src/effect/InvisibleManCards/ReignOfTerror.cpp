#include "effect/InvisibleManCards/ReignOfTerror.h"
#include "game/Game.h"
#include "fighter/InvisibleMan.h"
#include "fighter/Fog.h"
#include "player/Player.h"
#include "board/Board.h"
#include "board/Space.h"
#include <stdexcept>
using namespace std;

void ReignOfTerror::apply(Game &game, Fighter &fighter, Fighter &target,
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
        throw runtime_error("\n[!] ERROR : Reign Of Terror can only be used by Invisible Man!\n");
    }

    Space *mySpace = fighter.getPosition();
    if (mySpace == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Invisible Man has NO position!\n");
    }

    if (!mySpace->hasFogToken())
    {
        return;
    }

    Player *opponentPlayer = game.selectOpponent(*player);
    if (opponentPlayer == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Opponent player NOT found!\n");
    }

    Hero *enemyHero = opponentPlayer->getHero();
    if (enemyHero != nullptr && enemyHero->isAlive())
    {
        enemyHero->takeDamage(2);
    }

    for (Sidekick *sidekick : opponentPlayer->getSideKicks())
    {
        if (sidekick == nullptr)
            continue;
        if (!sidekick->isAlive())
            continue;

        sidekick->takeDamage(2);
    }
}

string ReignOfTerror::getDescription() const
{
    return "> If Invisible Man is on a Fog token, deal 2 damage to every opposing fighter.";
}

Effect *ReignOfTerror::clone() const
{
    return new ReignOfTerror(*this);
}