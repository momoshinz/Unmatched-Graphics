#include "effect/SherlockCards/FeintSherlock.h"
#include "fighter/Fighter.h"
#include "game/Game.h"
#include "player/Player.h"
#include "card/Card.h"
#include <iostream>
#include <stdexcept>
using namespace std;

void FeintSherlock::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    Player *player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    if (opponentCard == nullptr)
    {
        cerr << "\n[!] ERROR : Opponent has NO card to cancel.\n";
        return;
    }

    if (!opponentCard->hasEffect())
    {
        cerr << "\n[!] ERROR : Opponent's card has NO effect to cancel.\n";
        return;
    }

    opponentCard->cancelEffects();
    cout << "========================================";
    cout << "\n-< feint >- ACTIVATED!\n";
    cout << "\n[+] Effects of " << opponentCard->getName() << " have been CANCELED!\n";
    cout << "========================================\n";
}

string FeintSherlock::getDescription() const
{
    return "> Cancel all effects on your opponent's card.";
}

Effect *FeintSherlock::clone() const
{
    return new FeintSherlock(*this);
}