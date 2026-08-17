#include "effect/InvisibleManCards/EmergeFromMist.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "fighter/Hero.h"
#include "board/Space.h"
#include <iostream>
#include <stdexcept>
using namespace std;

void EmergeFromMist::apply(Game &game, Fighter &fighter, Fighter &target,
                           const Card &self, Card *opponentCard, bool didUserWin,
                           const EffectChoice &choice)
{
    Player *player = fighter.getOwner();

    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Emerge From Mist can only be used by Invisible Man!\n");
    }

    Space *mySpace = fighter.getPosition();

    if (mySpace == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has no position!\n");
    }

    cout << "\n========================================";
    cout << "\n-< Emerge From Mist >- ACTIVATED!\n";

    if (!fighter.hasStartedTurnOnFog())
    {
        cout << "\n[!] Invisible Man did NOT start this turn on a Fog token.\n";
        cout << "[!] Card keeps its normal attack value.\n";
        cout << "========================================\n";
        return;
    }

    fighter.addTempAttackBoost(2);

    cout << "\n[+] Invisible Man started this turn on a Fog token.\n";
    cout << "[+] Card attack increased from '3' to '5'.\n";
    cout << "========================================\n";
}

string EmergeFromMist::getDescription() const
{
    return "> If Invisible Man started this turn on a Fog token, this card's attack value becomes 5.";
}

Effect *EmergeFromMist::clone() const
{
    return new EmergeFromMist(*this);
}