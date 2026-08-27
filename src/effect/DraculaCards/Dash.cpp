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
    if (choice.selectedSpace == nullptr)
    {
        throw runtime_error("\n[!] ERROR : No destination selected for Dash!\n");
    }

    Space *currentPos = fighter.getPosition();

    if (currentPos == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO position on the map!\n");
    }

    Space *destination = choice.selectedSpace;

    if (!game.getBoard().moveFighter(&fighter, destination))
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