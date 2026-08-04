#include "effect/InvisibleManCards/SlipAway.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "fighter/Fog.h"
#include "fighter/InvisibleMan.h"
#include "board/Board.h"
#include "board/Space.h"
#include <iostream>
#include <stdexcept>
using namespace std;

void SlipAway::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    Player* player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    Board &board = game.getBoard();
    vector<Fog*> fogs = player->getFogs();
    if(fogs.empty())
    {
        cout << "\n[!] ERROR : No Fog tokens available.\n";
        return;
    }
    cout << "\n========================================\n";
    cout << "-< Slip Away >- ACTIVATED!\n";

    cout << "\n> Choose a Fog token:\n";
    for(int i=0 ; i<fogs.size() ; i++)
    {
        cout << i+1 << ". Fog " << fogs[i]->getID();
        if(fogs[i]->getPosition() != nullptr)
        {
            cout << " (Home " << fogs[i]->getPosition()->getId() << ")";
        }
        cout << endl;
    }
    int fogChoice;
    cin >> fogChoice;

    if (fogChoice < 1 || fogChoice > fogs.size())
    {
        throw runtime_error("\n[!] ERROR : Invalid Fog token!\n");
    }

    Fog *fog = fogs[fogChoice - 1];

    vector<Space *> availableSpaces;

    for (Space *space : board.getSpaces())
    {
        if (!space->isOccupied())
        {
            availableSpaces.push_back(space);
        }
    }

    if (availableSpaces.empty())
    {
        cout << "\n[!] No available spaces.\n";
        return;
    }

    cout << "\nChoose destination:\n";

    for (int i = 0; i < availableSpaces.size(); i++)
    {
        cout << i + 1 << ". Home " << availableSpaces[i]->getId() << endl;
    }

    int choice;
    cin >> choice;

    if (choice < 1 || choice > availableSpaces.size())
    {
        throw runtime_error("\n[!] ERROR : Invalid destination!\n");
    }
    Space *destination = availableSpaces[choice - 1];

    if (fog->getPosition() != nullptr)
    {
        fog->getPosition()->setFogToken(false);
    }

    fog->setPosition(destination);
    destination->setFogToken(true);

    board.moveFighter(&fighter, destination);

    cout << "\n[+] Fog token moved.\n";
    cout << "[+] Invisible Man moved to the same home.\n";
    cout << "\n========================================\n";
}

string SlipAway::getDescription() const
{
    return "> Move one Fog token to an unoccupied home, then move Invisible Man to that home.";
}

Effect *SlipAway::clone() const
{
    return new SlipAway(*this);
}