#include "effect/SherlockCards/AdministerAid.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Sherlock.h"
#include "fighter/DrWatson.h"
#include "board/Space.h"
#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

void AdministerAid::apply(Game &game, Fighter &fighter, Fighter &target,
                          const Card &self, Card *opponentCard, bool didUserWin,
                          const EffectChoice &choice)
{
    if (fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Administer Aid can only be used by Dr. Watson!\n");
    }

    Player *player = fighter.getOwner();

    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    Watson *watson = player->getWatson();

    if (watson == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Dr. Watson NOT found!\n");
    }

    Hero *sherlock = player->getHero();

    if (sherlock == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Sherlock NOT found!\n");
    }

    if (choice.selectedSpace == nullptr)
    {
        throw runtime_error("\n[!] ERROR : No destination selected for Dr. Watson!\n");
    }

    if (choice.selectedSpace->isOccupied())
    {
        throw runtime_error("\n[!] ERROR : Selected home is already OCCUPIED!\n");
    }

    cout << "\n========================================";
    cout << "\n-< Administer Aid >- ACTIVATED!\n";

    watson->moveTo(choice.selectedSpace);

    cout << "\n[o] Dr. Watson moved to Home " << choice.selectedSpace->getId() << endl;

    sherlock->heal(1);

    cout << "[+] " << sherlock->getName() << " healed 1 health.\n";

    if (player->drawCardToHand())
    {
        cout << "[+] Drew 1 card successfully.\n";
    }
    else
    {
        cout << "[!] Deck is empty. No card drawn.\n";
    }
    cout << "========================================\n";
}

EffectInputKind AdministerAid::getInputKind() const
{
    return EffectInputKind::ChooseAdjacentEmptySpace;
}

string AdministerAid::getDescription() const
{
    return "> Move Dr. Watson to an adjacent home to Sherlock, heal Sherlock by 1, then draw 1 card.";
}

Effect *AdministerAid::clone() const
{
    return new AdministerAid(*this);
}