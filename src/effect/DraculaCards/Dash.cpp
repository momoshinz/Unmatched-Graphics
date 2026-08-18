#include "effect/DraculaCards/Dash.h"
#include "game/Game.h"
#include "player/Player.h"
#include "board/Space.h"
#include "board/Board.h"
#include "fighter/Fighter.h"
#include <iostream>
#include <stdexcept>

using namespace std;

void Dash::apply(Game &game,
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

    Space *currentPos = fighter.getPosition();

    if (currentPos == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO position on the map!\n");
    }

    if (choice.selectedSpace == nullptr)
    {
        throw runtime_error("\n[!] ERROR : No destination selected for Dash!\n");
    }

    Space *destination = choice.selectedSpace;

    vector<Space *> reachableSpaces = game.getBoard().getAvailableMoves(&fighter, 3);

    bool validDestination = false;

    for (Space *space : reachableSpaces)
    {
        if (space == destination)
        {
            validDestination = true;
            break;
        }
    }

    if (!validDestination)
    {
        throw runtime_error("\n[!] ERROR : Selected destination is NOT reachable by Dash!\n");
    }

    cout << "\n========================================\n";
    cout << "-< Dash >- ACTIVATED!\n";

    if (game.getBoard().moveFighter(&fighter, destination))
    {
        cout << "\n[+] " << fighter.getName()
             << " moved from home "
             << currentPos->getId()
             << " to home "
             << destination->getId()
             << "!\n";

        cout << "========================================\n";
    }
    else
    {
        throw runtime_error("\n[!] ERROR : FAILED to move fighter!\n");
    }
}

EffectInputKind Dash::getInputKind() const
{
    return EffectInputKind::ChooseReachableSpace;
}

int Dash::getMoveRange() const
{
    return 3;
}

string Dash::getDescription() const
{
    return "> MOVE your fighter up to 3 homes.";
}

Effect *Dash::clone() const
{
    return new Dash(*this);
}