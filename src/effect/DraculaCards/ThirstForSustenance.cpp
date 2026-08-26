#include "effect/DraculaCards/ThirstForSustenance.h"
#include "game/Game.h"
#include "board/Board.h"
#include "board/Space.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "fighter/Hero.h"
#include "fighter/Sisters.h"
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

void ThirstForSustenance::apply(
    Game &game,
    Fighter &fighter,
    Fighter &target,
    const Card &self,
    Card *opponentCard,
    bool didUserWin,
    const EffectChoice &choice)
{
    if (!didUserWin)
    {
        cout << "\n[!] ERROR : You didn't win in combat!\n";
        return;
    }

    Player *player = fighter.getOwner();

    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    if (!fighter.isSister())
    {
        throw runtime_error("\n[!] ERROR : Thirst For Sustenance can only be used by Sisters!\n");
    }

    Hero *dracula = player->getHero();

    if (dracula == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Dracula not found!\n");
    }

    Space *enemySpace = target.getPosition();

    if (enemySpace == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Target has NO position!\n");
    }

    Space *chosenSpace = choice.selectedSpace;

    if (chosenSpace == nullptr)
    {
        throw runtime_error("\n[!] ERROR : No destination space was selected!\n");
    }

    bool validDestination = false;

    for (Space *neighbor : enemySpace->getNeighbors())
    {
        if (neighbor == chosenSpace)
        {
            validDestination = true;
            break;
        }
    }

    if (!validDestination)
    {
        throw runtime_error("\n[!] ERROR : Selected destination is NOT adjacent to target!\n");
    }

    if (chosenSpace->isOccupied())
    {
        throw runtime_error("\n[!] ERROR : Selected destination is OCCUPIED!\n");
    }

    game.getBoard().moveFighter(dracula, chosenSpace);
}

string ThirstForSustenance::getDescription() const
{
    return "> If you won the combat, move Dracula to any empty adjacent home next to the opposing fighter.";
}

EffectInputKind ThirstForSustenance::getInputKind() const
{
    return EffectInputKind::ChooseTargetAdjacentEmptySpace;
}

Effect *ThirstForSustenance::clone() const
{
    return new ThirstForSustenance(*this);
}