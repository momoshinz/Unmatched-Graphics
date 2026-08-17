#include "effect/InvisibleManCards/ReignOfTerror.h"
#include "game/Game.h"
#include "fighter/InvisibleMan.h"
#include "fighter/Fog.h"
#include "player/Player.h"
#include "board/Board.h"
#include "board/Space.h"
#include <iostream>
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
        cout << "\n[!] Invisible Man is not on a Fog token.\n    Reign Of Terror has no effect!\n";
        return;
    }

    Player *opponentPlayer = game.selectOpponent(*player);

    if (opponentPlayer == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Opponent player NOT found!\n");
    }

    cout << "\n========================================";
    cout << "\n-< Reign Of Terror >- ACTIVATED!\n";

    int damagedFightersCount = 0;

    Hero *enemyHero = opponentPlayer->getHero();

    if (enemyHero != nullptr && enemyHero->isAlive())
    {
        enemyHero->takeDamage(2);
        damagedFightersCount++;

        cout << "\n[-] " << enemyHero->getName() << " took 2 damage.\n";
    }

    for (Sidekick *sidekick : opponentPlayer->getSideKicks())
    {
        if (sidekick == nullptr)
            continue;
        if (!sidekick->isAlive())
            continue;

        sidekick->takeDamage(2);
        damagedFightersCount++;

        cout << "\n[-] " << sidekick->getName() << " took 2 damage.\n";
    }

    if (damagedFightersCount == 0)
    {
        cout << "\n[!] No opposing fighters are alive to damage.\n";
    }
    else
    {
        cout << "\n[+] Reign Of Terror dealt 2 damage to "
             << damagedFightersCount << " opposing fighter(s).\n";
    }

    cout << "\n========================================\n";
}

string ReignOfTerror::getDescription() const
{
    return "> If Invisible Man is on a Fog token, deal 2 damage to every opposing fighter.";
}

Effect *ReignOfTerror::clone() const
{
    return new ReignOfTerror(*this);
}