#include "effect/DraculaCards/ThirstForSustenance.h"
#include "game/Game.h"
#include "board/Board.h"
#include "board/Space.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "fighter/Hero.h"
#include "fighter/Sisters.h"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

void ThirstForSustenance::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    if(!didUserWin)
    {
        cout << "\n[!] ERROR : You didn't win in combat!\n";
        return;
    }
    Player *player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }
    if(!fighter.isSister())
    {
        throw runtime_error("\n[!] ERROR : Thirst For Sustenance can only be used by Sisters!\n");
    }

    Hero* dracula = player->getHero();
    if(dracula == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Dracula not found!\n");
    }

    Board &board = game.getBoard();
    Space* enemySpace = target.getPosition();
    if(enemySpace == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Target has NO position!\n");
    }
    
    vector<Space*> availableSpaces;
    for(auto neighbor : enemySpace->getNeighbors())
    {
        if(!neighbor->isOccupied())
        {
            availableSpaces.push_back(neighbor);
        }
    }
    if(availableSpaces.empty())
    {
        cerr << "\n[!] ERROR : There is no empty adjacent home!\n";
        return;
    }

    cout << "\n========================================\n";
    cout << "-< Thirst For Sustenance >- ACTIVATED!\n";

    cout << "\n========================================\n";
    cout << "[?] Choose where to move Dracula :\n";
    cout << "========================================\n";

    for(int i=0 ; i<availableSpaces.size() ; i++)
    {
        cout << i+1 << ". Home " << availableSpaces[i]->getId() << endl;
    }
    int choice;
    cout << "> Enter your choice :";
    cin >> choice;

    if(choice < 1 || choice > availableSpaces.size())
    {
        throw runtime_error("\n[!] ERROR : Invalid choice! :(\n");
    }

    board.moveFighter(dracula, availableSpaces[choice-1]);
    cout << "\n[+] Dracula moved next to " << target.getName() << ".\n";
    cout << "========================================\n";
}

string ThirstForSustenance::getDescription() const
{
    return "> If you won the combat, move Dracula to any empty adjacent home next to the opposing fighter.";
}

Effect *ThirstForSustenance::clone() const
{
    return new ThirstForSustenance(*this);
}