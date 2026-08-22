#include "effect/InvisibleManCards/RollingFog.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "fighter/Fog.h"
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

    vector<Fog *> fogs = player->getFogs();

    if (choice.selectedFogId < 0 || choice.selectedFogId >= static_cast<int>(fogs.size()))
    {
        throw runtime_error("\n[!] ERROR : Invalid Fog token selection!\n");
    }

    Fog *fog = fogs[choice.selectedFogId];

    if (choice.secondSpace == nullptr)
    {
        throw runtime_error("\n[!] ERROR : No destination selected!\n");
    }

    Space *destination = choice.secondSpace;

    if (destination->hasFogToken())
    {
        throw runtime_error("\n[!] ERROR : Destination already has a Fog token!\n");
    }

    if (fog->getPosition() != nullptr)
    {
        fog->getPosition()->setFogToken(false);
    }

    fog->setPosition(destination);
    destination->setFogToken(true);

    cout << "\n[+] Fog token moved to Home " << destination->getId() << ".\n";

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