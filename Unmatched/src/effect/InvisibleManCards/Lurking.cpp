#include "effect/InvisibleManCards/Lurking.h"
#include "game/Game.h"
#include "fighter/InvisibleMan.h"
#include "fighter/Fog.h"
#include "player/Player.h"
#include "board/Board.h"
#include "board/Space.h"
#include <iostream>
#include <stdexcept>

using namespace std;

void Lurking::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    Player* player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

     if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Coded Notes can only be used by Invisible Man!\n");
    }
    Board &board = game.getBoard();

    cout << "\n========================================\n";
    cout << "-< Lurking >- ACTIVATED!\n";

    player->drawCardToHand();
    cout << "\n[+] Drew one card.\n";

    cout << "\n> Choose one effect:\n";
    cout << "1. Move Invisible Man to a Fog token.\n";
    cout << "2. Move one Fog token up to 3 spaces.\n";
    cout << "~~> ";

    int choice;
    cin >> choice;
    if (choice == 1)
    {
        vector<Space *> fogSpaces;

        for (Space *space : board.getSpaces())
        {
            if (space->hasFogToken())
            {
                fogSpaces.push_back(space);
            }
        }

        if (fogSpaces.empty())
        {
            cout << "\n[!]ERROR : No Fog tokens on the board.\n";
            return;
        }

        cout << "\n> Choose destination:\n";

        for (int i = 0; i < fogSpaces.size(); i++)
        {
            cout << i + 1 << ". Home " << fogSpaces[i]->getId() << endl;
        }

        int moveChoice;
        cin >> moveChoice;

        if (moveChoice < 1 || moveChoice > fogSpaces.size())
        {
            throw runtime_error("\n[!] Invalid destination!\n");
        }

        board.moveFighter(&fighter, fogSpaces[moveChoice - 1]);

        cout << "\n[+] Invisible Man moved successfully.\n";
    }

    else if (choice == 2)
    {
        vector<Fog *> fogs = player->getFogs();

        if (fogs.empty())
        {
            cout << "\n[!]ERROR : No Fog tokens.\n";
            return;
        }
        cout << "\n> Choose Fog token:\n";

        for (int i = 0; i < fogs.size(); i++)
        {
            cout << i + 1 << ". Fog " << fogs[i]->getID();

            if (fogs[i]->getPosition() != nullptr)
            {
                cout << " (Space " << fogs[i]->getPosition()->getId() << ")";
            }
            cout << endl;
        }

        int fogChoice;
        cin >> fogChoice;

        if (fogChoice < 1 || fogChoice > fogs.size())
        {
            throw runtime_error("\n[!] Invalid Fog!\n");
        }
        Fog *fog = fogs[fogChoice - 1];

        vector<Space *> moves = board.getAvailableFogMoves(fog, 3);

        if (moves.empty())
        {
            cout << "\n[!]ERROR : No available destination.\n";
            return;
        }

        cout << "\n> Choose destination:\n";

        for (int i = 0; i < moves.size(); i++)
        {
            cout << i + 1 << ". Home " << moves[i]->getId() << endl;
        }

        int moveChoice;
        cin >> moveChoice;
        if (moveChoice < 1 || moveChoice > moves.size())
        {
            throw runtime_error("\n[!] Invalid destination!\n");
        }

        fog->getPosition()->setFogToken(false);
        fog->setPosition(moves[moveChoice - 1]);
        moves[moveChoice - 1]->setFogToken(true);

        cout << "\n[+] Fog moved successfully.\n";
        cout << "\n========================================\n";
    }
    else
    {
        throw runtime_error("\n[!] Invalid choice!\n");
    }
}

string Lurking::getDescription() const
{
    return "> Draw 1 card. Choose one effect: Move Invisible Man to a home with a Fog token, or move one Fog token up to 3 homes.";
}

Effect *Lurking::clone() const
{
    return new Lurking(*this);
}