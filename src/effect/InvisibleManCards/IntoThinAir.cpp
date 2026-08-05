#include "effect/InvisibleManCards/IntoThinAir.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "fighter/Fog.h"
#include "board/Board.h"
#include "board/Space.h"
#include "ui/TerminalUI.h"
#include <iostream>
#include <stdexcept>
using namespace std;

void IntoThinAir::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
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

    cout << "\n========================================\n";
    cout << "-< Into Thin Air >- ACTIVATED!\n";

    Board &board = game.getBoard();
    vector<Space*> moves = board.getAvailableMoves(&fighter, 1);;
    if(!moves.empty())
    {
        cout << "\n> Choose a space for Invisible Man:\n";
        for(int i=0 ; i<moves.size() ; i++)
        {
            cout << i+1 << ". Home " << moves[i]->getId() << endl;
        }
        int choice;
        while(true)
        {
            cout << "~~> ";
            cin >> choice;
            if(cin.fail())
            {
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }
            if(choice >= 1 && choice <= moves.size())
            {
                break;
            }
            cout << "\n[!] ERROR : Invalid choice.\n";
        }
        board.moveFighter(&fighter, moves[choice-1]);
        cout << "\n[+] Invisible Man moves.\n";
    }
    vector<Fog *> fogs = player->getFogs();
    if(fogs.empty())
    {
        return;
    }
    cout << "\n> Choose a Fog token: \n";
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
    while(true)
    {
        cout << "~~> ";
        cin >> fogChoice;
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        if(fogChoice >= 1 && fogChoice <= fogs.size())
        {
            break;
        }
        cout << "\n[!] ERROR : Invalid choice.\n";
    }
    Fog* fog = fogs[fogChoice-1];
    vector<Space*> fogMoves = board.getAvailableFogMoves(fog, 3);
    if(fogMoves.empty())
    {
        cout << "\n[!] ERROR : No available destination for this Fog!\n";
        return;
    }
    cout<<"\nChoose destination for Fog:\n";

    for(size_t i=0;i<fogMoves.size();i++)
    {
        cout<<i+1 <<". Home " <<fogMoves[i]->getId() <<endl;
    }
    int dest;
    while(true)
    {
        cout<<"~~> ";
        cin>>dest;

        if(cin.fail())
        {
            cin.clear();
            cin.ignore(1000,'\n');
            continue;
        }
        if(dest>=1 && dest<=fogMoves.size())
        {
            break;
        }
        cout << "\n[!] ERROR : Invalid choice.\n";
    }
    if(fog->getPosition() != nullptr)
    {
        fog->getPosition()->setFogToken(false);
    }
    fog->setPosition(fogMoves[dest-1]);
    fogMoves[dest-1]->setFogToken(true);
    cout << "\n[+] Fog moved successfully.\n";
    cout << "\n========================================\n";
}

string IntoThinAir::getDescription() const
{
    return "> Move Invisible Man up to 1 Home. Then move one Fog token up to 3 Homes.";
}

Effect *IntoThinAir::clone() const
{
    return new IntoThinAir(*this);
}