#include "effect/SherlockCards/TheGameIsAfoot.h"
#include "game/Game.h"
#include "player/Player.h"
#include "board/Space.h"
#include "board/Board.h"
#include "fighter/Fighter.h"
#include <iostream>
#include <vector>
#include <queue>
#include <stdexcept>
using namespace std;

void TheGameIsAfoot::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : The Game Is Afoot can only be used by Sherlock Holmes!\n");
    }

    Player *player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Sherlock has NO owner!\n");
    }

    Space *currentPos = fighter.getPosition();
    if (currentPos == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Sherlock has NO position on the map!\n");
    }

    vector<Space *> reachableSpaces = game.getBoard().getAvailableMoves(&fighter, 3);
    if (reachableSpaces.empty())
    {
        cerr << "\n[!] NO reachable empty home.\n";
        return;
    }
    cout << "\n==============================================";
    cout << "\n[o] Choose a destination { up to 3 homes away } :\n";
    cout << "===============================================\n";

    for (int i = 0; i < reachableSpaces.size(); i++)
    {
        cout << "\n> " << i + 1 << ". home" << reachableSpaces[i]->getId() << endl;
    }

    cout << "\n> Enter your choice (1 to " << reachableSpaces.size() << ") : ";
    int choice;
    cin >> choice;
    if (choice < 1 || choice > reachableSpaces.size())
    {
        throw out_of_range("\n[!] ERROR : Invalid choice!\n");
    }

    Space *destination = reachableSpaces[choice - 1];
    if (game.getBoard().moveFighter(&fighter, destination))
    {
        cout << "\n========================================\n";
        cout << "-< The Game Is Afoot >- ACTIVATED!\n";
        cout << "\n[+] " << fighter.getName() << " moved from home "
             << currentPos->getId() << " to home " << destination->getId() << "!\n";
        cout << "========================================\n";
    }
    else
    {
        cout << "\n[!] ERROR : FAILED to move Sherlock Holmes!\n";
    }
}

string TheGameIsAfoot::getDescription() const
{
    return "> move your fighter up to 3 homes.";
}

Effect *TheGameIsAfoot::clone() const
{
    return new TheGameIsAfoot(*this);
}