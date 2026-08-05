#include "effect/DraculaCards/FeintDracula.h"
#include "fighter/Fighter.h"
#include "game/Game.h"
#include "player/Player.h"
#include "card/Card.h"
#include <iostream>
#include <stdexcept>
using namespace std;

void FeintDracula::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    Player *player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    if(opponentCard == nullptr)
    {
        cerr << "\n[!] ERROR : Opponent has no card to cancel.\n";
        return;
    }

    if(!opponentCard->hasEffect())
    {
        cerr << "\n[!] ERROR : Opponent's card has no effect to cancel.\n";
        return;
    }
    cout << "\n========================================\n";
    cout << "-< Feint >- ACTIVATED!\n";
    opponentCard->cancelEffects();
    cout << "[+] Effects of " << opponentCard->getName() << " have been canceled!\n";
    cout << "========================================\n";
}

string FeintDracula::getDescription() const
{
    return "> Cancel all effects on your opponent's card.";
}

Effect *FeintDracula::clone() const
{
    return new FeintDracula(*this);
}