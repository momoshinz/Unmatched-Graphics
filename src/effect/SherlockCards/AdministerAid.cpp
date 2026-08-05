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

void AdministerAid::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
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

    Space *heroSpace = sherlock->getPosition();

    if (heroSpace == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Sherlock is NOT on the map!\n");
    }

    vector<Space *> availableSpaces;

    for (Space *neighbor : heroSpace->getNeighbors())
    {
        if (!neighbor->isOccupied())
        {
            availableSpaces.push_back(neighbor);
        }
    }

    if (availableSpaces.empty())
    {
        throw runtime_error("\n[!] ERROR : There is NO empty adjacent home to Sherlock!\n");
    }

    cout << "\n========================================";
    cout << "\n-< Administer Aid >- ACTIVATED!\n";

    cout << "\nAvailable destinations for Dr. Watson :\n";

    for (size_t i = 0; i < availableSpaces.size(); i++)
    {
        cout << i + 1 << ". Home " << availableSpaces[i]->getId() << endl;
    }

    int choice;
    cout << "\n> Choose destination : ";
    cin >> choice;

    if (choice < 1 || choice > static_cast<int>(availableSpaces.size()))
    {
        throw out_of_range("\n[!] ERROR : Invalid home selection! :(\n");
    }

    watson->moveTo(availableSpaces[choice - 1]);

    cout << "\n[o] Dr. Watson moved to Home " << availableSpaces[choice - 1]->getId() << endl;

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

string AdministerAid::getDescription() const
{
    return "> Move Dr. Watson to an adjacent home to Sherlock, heal Sherlock by 1, then draw 1 card.";
}

Effect *AdministerAid::clone() const
{
    return new AdministerAid(*this);
}