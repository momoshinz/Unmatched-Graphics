#include "effect/SherlockCards/FixedPoint.h"
#include "game/Game.h"
#include "card/Card.h"
#include "fighter/Fighter.h"
#include "fighter/Sherlock.h"
#include "player/Player.h"
#include "board/Board.h"
#include "board/Space.h"
#include <iostream>

using namespace std;

void FixedPoint::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool attackerWon)
{
    if (fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Fixed Point In A Changing Age can only be used by Dr. Watson!\n");
    }

    Player *player = fighter.getOwner();

    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    Watson *watson = player->getWatson();

    if (watson == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Watson NOT found!\n");
    }

    Hero *sherlock = player->getHero();

    if (sherlock == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Sherlock NOT found!\n");
    }

    if (watson->getPosition() == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Watson has NO position on map!\n");
    }

    if (sherlock->getPosition() == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Sherlock has NO position on map!\n");
    }

    if (watson->isAdjacent(sherlock, game.getBoard()))
    {
        watson->heal(1);
        sherlock->heal(1);

        cout << "\n========================================\n";
        cout << "-< Fixed Point >- ACTIVATED!\n";

        cout << "[+] Watson and Sherlock each RECOVER +1 health!\n";
        cout << "========================================\n";
    }
    else
    {
        cout << "\n[!] Watson is NOT adjacent to Sherlock. Effect CANCELED.\n";
    }
}

string FixedPoint::getDescription() const
{
    return "> If Dr. Watson is adjacent to Holmes, add 1 health to both.";
}

Effect *FixedPoint::clone() const
{
    return new FixedPoint(*this);
}