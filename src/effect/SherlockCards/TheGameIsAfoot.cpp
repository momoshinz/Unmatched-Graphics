#include "effect/SherlockCards/TheGameIsAfoot.h"
#include "game/Game.h"
#include "player/Player.h"
#include "board/Space.h"
#include "board/Board.h"
#include "fighter/Fighter.h"
#include <stdexcept>
using namespace std;

void TheGameIsAfoot::apply(Game &game, Fighter &fighter, Fighter &target,
                           const Card &self, Card *opponentCard, bool didUserWin,
                           const EffectChoice &choice)
{
    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : The Game Is Afoot can only be used by Sherlock Holmes!\n");
    }

    Player *player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Sherlock has NO owner!\n");
    }

    Space *currentPos = fighter.getPosition();
    if (currentPos == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Sherlock has NO position on the map!\n");
    }

    if (choice.selectedSpace == nullptr)
    {
        throw runtime_error("\n[!] ERROR : No destination selected!\n");
    }

    Space *destination = choice.selectedSpace;

    if (!game.getBoard().moveFighter(&fighter, destination))
    {
        throw runtime_error("\n[!] ERROR : Could not move Sherlock to the selected home!\n");
    }
}

EffectInputKind TheGameIsAfoot::getInputKind() const
{
    return EffectInputKind::ChooseReachableSpace;
}

int TheGameIsAfoot::getMoveRange() const
{
    return 3;
}

string TheGameIsAfoot::getDescription() const
{
    return "> move your fighter up to 3 homes.";
}

Effect *TheGameIsAfoot::clone() const
{
    return new TheGameIsAfoot(*this);
}