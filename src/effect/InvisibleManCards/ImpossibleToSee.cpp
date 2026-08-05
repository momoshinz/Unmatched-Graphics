#include "effect/InvisibleManCards/ImpossibleToSee.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "fighter/Hero.h"
#include "fighter/Sidekick.h"
#include "board/Space.h"
#include "card/Card.h"
#include <iostream>
#include <stdexcept>
using namespace std;

void ImpossibleToSee::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    Player *player = fighter.getOwner();

    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Impossible To See can only be used by Invisible Man!\n");
    }

    if (opponentCard == nullptr)
    {
        cout << "\n[!] Opponent has NO combat card.\n";
        return;
    }

    cout << "\n========================================";
    cout << "\n-< Impossible To See >- ACTIVATED!\n";

    opponentCard->setTemporaryCombatValue(0);

    cout << "\n[+] Opponent combat value becomes 0.\n";
    cout << "[!] Card effects are NOT canceled.\n";

    cout << "\n========================================\n";
}

string ImpossibleToSee::getDescription() const
{
    return "> The value of your opponents's attack or defense is 0 and cannot be changed by card effects.(other card effects still happen)";
}

Effect* ImpossibleToSee::clone() const
{
    return new ImpossibleToSee(*this);
}