#include "effect/DraculaCards/RaveningSeduction.h"
#include "game/Game.h"
#include "game/CombatSystem.h"
#include "board/Board.h"
#include "board/Space.h"
#include "fighter/Fighter.h"
#include "fighter/Sisters.h"
#include "player/Player.h"
#include "fighter/Hero.h"
#include "fighter/Sidekick.h"
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

void RaveningSeduction::apply(Game &game,
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

    if (!fighter.isSister())
    {
        throw runtime_error("\n[!] ERROR : Ravening Seduction can only be used by Sisters!\n");
    }

    if (choice.selectedFighter == nullptr)
    {
        throw runtime_error("\n[!] ERROR : No fighter selected!\n");
    }

    if (choice.selectedSpace == nullptr)
    {
        throw runtime_error("\n[!] ERROR : No destination selected!\n");
    }

    Fighter *selected = choice.selectedFighter;
    Space *destination = choice.selectedSpace;


    if (!selected->isAlive())
    {
        throw runtime_error("\n[!] ERROR : Selected fighter is NOT alive!\n");
    }

    if (destination->isOccupied())
    {
        throw runtime_error("\n[!] ERROR : Selected destination is OCCUPIED!\n");
    }

    game.getBoard().moveFighter(selected, destination);

    int damage = 0;

    for (Space *neighbor : destination->getNeighbors())
    {
        if (neighbor == nullptr)
            continue;

        Fighter *adjacent = neighbor->getFighter();

        if (adjacent == nullptr)
            continue;

        if (adjacent->isSister() && adjacent->isAlive())
        {
            damage++;
        }
    }

    if (damage > 0)
    {
        selected->takeDamage(damage);
    }
}

EffectInputKind RaveningSeduction::getInputKind() const
{
    return EffectInputKind::ChooseFighterAndReachableSpace;
}

string RaveningSeduction::getDescription() const
{
    return "> Move one of your fighters up to 2 homes.\nThen deal 1 damage to that fighter for each adjacent Sister.";
}

Effect *RaveningSeduction::clone() const
{
    return new RaveningSeduction(*this);
}