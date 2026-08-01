#include "effect/SherlockCards/StudyMethods.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "card/Hand.h"
#include <iostream>
#include <stdexcept>

using namespace std;

void StudyMethods::apply(Game &game, Fighter &fighter, Fighter &target,const Card &self, Card *opponentCard, bool attackerWon)
{
    Player *player = fighter.getOwner();

    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    if (!attackerWon)
    {
        cout << "\n[!] Fighter didn't win the combat therefore Study Methods is CANCELED.\n";
        return;
    }

    Player *opponent = target.getOwner();

    if (opponent == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Opponent has NO owner!\n");
    }

    cout << "\n========================================\n";
    cout << "-< Study Methods >- ACTIVATED!\n";

    cout << "\nOpponent's hand :\n";

    opponent->getHand().display();

    cout << "\n========================================\n";
}

string StudyMethods::getDescription() const
{
    return "> If you win the combat, you may look at your opponent's hand.";
}

Effect *StudyMethods::clone() const
{
    return new StudyMethods(*this);
}