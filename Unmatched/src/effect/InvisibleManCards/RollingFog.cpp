#include "effect/InvisibleManCards/RollingFog.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "board/Board.h"
#include "board/Space.h"
#include <iostream>
#include <stdexcept>

using namespace std;

void RollingFog::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    Player *player = fighter.getOwner();

    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Rolling Fog can only be used by Invisible Man!\n");
    }

    Board &board = game.getBoard();

    cout << "\n========================================";
    cout << "\n-< Rolling Fog >- ACTIVATED!\n";

    vector<Space *> fogSpaces;

    for (Space *space : board.getSpaces())
    {
        if (space != nullptr && space->hasFogToken())
        {
            fogSpaces.push_back(space);
        }
    }

    if (fogSpaces.empty())
    {
        cout << "\n[!] There is NO Fog token on the board!\n";
        cout << "    Rolling Fog has NO effect.\n";
        cout << "========================================\n";
        return;
    }

    cout << "\n========== Choose Fog Token ==========\n";

    for (size_t i = 0; i < fogSpaces.size(); i++)
    {
        cout << i + 1
             << ". Home " << fogSpaces[i]->getId()
             << endl;
    }

    int fogChoice;

    while (true)
    {
        cout << "\n> Choose a Fog token to move : ";
        cin >> fogChoice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n[!] ERROR : Invalid Choice!\n";
            continue;
        }

        if (fogChoice >= 1 && fogChoice <= static_cast<int>(fogSpaces.size()))
        {
            break;
        }

        cout << "\n[!] ERROR : Invalid choice!\n";
    }

    Space *source = fogSpaces[fogChoice - 1];

    cout << "\n========== Choose Destination ==========\n";

    vector<Space *> destinations;

    for (Space *space : board.getSpaces())
    {
        if (space == nullptr)
            continue;

        if (space == source)
            continue;

        if (space->hasFogToken())
            continue;

        destinations.push_back(space);
    }

    if (destinations.empty())
    {
        cout << "\n[!] There is NO valid destination for the Fog token!\n";
        cout << "========================================\n";
        return;
    }

    for (size_t i = 0; i < destinations.size(); i++)
    {
        cout << i + 1
             << ". Home " << destinations[i]->getId()
             << endl;
    }

    int destinationChoice;

    while (true)
    {
        cout << "\n> Choose destination home : ";
        cin >> destinationChoice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n[!] ERROR : Invalid Choice!\n";
            continue;
        }

        if (destinationChoice >= 1 && destinationChoice <= static_cast<int>(destinations.size()))
        {
            break;
        }

        cout << "\n[!] ERROR : Invalid choice!\n";
    }

    Space *destination = destinations[destinationChoice - 1];

    source->setFogToken(false);
    destination->setFogToken(true);

    cout << "\n[+] Fog token moved from Home "
         << source->getId()
         << " to Home "
         << destination->getId()
         << ".\n";

    game.getTurnManager().addAction();

    cout << "[+] Invisible Man gains 1 action.\n";

    cout << "========================================\n";
}

string RollingFog::getDescription() const
{
    return "> Move 1 Fog token to another home and gain 1 action.";
}

Effect *RollingFog::clone() const
{
    return new RollingFog(*this);
}