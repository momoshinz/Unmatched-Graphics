#include "effect/InvisibleManCards/SlipAway.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "fighter/Fog.h"
#include "board/Board.h"
#include "board/Space.h"
#include <iostream>
#include <stdexcept>
using namespace std;

void SlipAway::apply(Game &game, Fighter &fighter, Fighter &target,
                     const Card &self, Card *opponentCard, bool didUserWin,
                     const EffectChoice &choice)
{
    Player *player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    vector<Fog *> fogs = player->getFogs();
    if (fogs.empty())
    {
        cout << "\n[!] ERROR : No Fog tokens available.\n";
        return;
    }

    cout << "\n========================================\n";
    cout << "-< Slip Away >- ACTIVATED!\n";

    if (choice.selectedFogId < 0 || choice.selectedFogId >= static_cast<int>(fogs.size()))
    {
        throw runtime_error("\n[!] ERROR : Invalid Fog token selection!\n");
    }

    Fog *fog = fogs[choice.selectedFogId];

    if (choice.selectedSpace == nullptr)
    {
        throw runtime_error("\n[!] ERROR : No destination selected!\n");
    }

    Space *destination = choice.selectedSpace;

    if (fog->getPosition() != nullptr)
    {
        fog->getPosition()->setFogToken(false);
    }

    fog->setPosition(destination);
    destination->setFogToken(true);

    game.getBoard().moveFighter(&fighter, destination);

    cout << "\n[+] Fog token moved.\n";
    cout << "[+] Invisible Man moved to the same home.\n";
    cout << "\n========================================\n";
}

EffectInputKind SlipAway::getInputKind() const
{
    return EffectInputKind::ChooseFogAndDestination;
}

string SlipAway::getDescription() const
{
    return "> Move one Fog token to an unoccupied home, then move Invisible Man to that home.";
}

Effect *SlipAway::clone() const
{
    return new SlipAway(*this);
}