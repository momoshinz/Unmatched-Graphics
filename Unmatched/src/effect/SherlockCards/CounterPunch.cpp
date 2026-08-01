#include "effect/SherlockCards/CounterPunch.h"
#include "game/Game.h"
#include "fighter/Fighter.h"
#include "board/Board.h"
#include "card/Card.h"
#include <iostream>
#include <stdexcept>

using namespace std;

void CounterPunch::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{

    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Counter Punch can only be used by Sherlock Holmes!\n");
    }

    if (fighter.getPosition() == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Sherlock has NO position on the map!\n");
    }

    if (target.getPosition() == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Target has NO position on the map!\n");
    }

    if (fighter.isAdjacent(&target, game.getBoard()))
    {
        target.takeDamage(2);

        cout << "\n========================================";
        cout << "\n-< counter punch PUNCH >- ACTIVATED!\n";
        cout << "[-] " << target.getName()
             << " takes 2 damage.\n";
        cout << "========================================\n";
    }
    else
    {
        cout << "\n[!] Counter Punch CNCELED.\n";
        cout << fighter.getName() << " is not adjacent to " << target.getName() << "!\n";
    }
}

string CounterPunch::getDescription() const
{
    return "> If Sherlock Holmes is adjacent to the opposing fighter, deal 2 damage to that fighter.";
}

Effect *CounterPunch::clone() const
{
    return new CounterPunch(*this);
}