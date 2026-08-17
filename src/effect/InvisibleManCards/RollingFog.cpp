#include "effect/InvisibleManCards/RollingFog.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "board/Board.h"
#include "board/Space.h"
#include <iostream>
#include <stdexcept>
using namespace std;

void RollingFog::apply(Game &game, Fighter &fighter, Fighter &target,
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
        throw runtime_error("\n[!] ERROR : Rolling Fog can only be used by Invisible Man!\n");
    }

    cout << "\n========================================";
    cout << "\n-< Rolling Fog >- ACTIVATED!\n";

    if (choice.selectedSpace == nullptr || choice.secondSpace == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Invalid Fog move selection!\n");
    }

    if (!choice.selectedSpace->hasFogToken())
    {
        throw runtime_error("\n[!] ERROR : Selected source has NO Fog token!\n");
    }

    if (choice.secondSpace->hasFogToken())
    {
        throw runtime_error("\n[!] ERROR : Destination already has a Fog token!\n");
    }

    choice.selectedSpace->setFogToken(false);
    choice.secondSpace->setFogToken(true);

    cout << "\n[+] Fog token moved from Home "
         << choice.selectedSpace->getId()
         << " to Home "
         << choice.secondSpace->getId()
         << ".\n";

    game.getTurnManager().addAction();

    cout << "[+] Invisible Man gains 1 action.\n";

    cout << "========================================\n";
}

EffectInputKind RollingFog::getInputKind() const
{
    return EffectInputKind::ChooseFogSourceAndDestination;
}

string RollingFog::getDescription() const
{
    return "> Move 1 Fog token to another home and gain 1 action.";
}

Effect *RollingFog::clone() const
{
    return new RollingFog(*this);
}