#include "effect/DraculaCards/RaveningSeduction.h"
#include "game/Game.h"
#include "board/Board.h"
#include "board/Space.h"
#include "fighter/Fighter.h"
#include "fighter/Sisters.h"
#include "player/Player.h"
#include "fighter/Hero.h"
#include "fighter/Sidekick.h"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

void RaveningSeduction::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    Player *player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    if (!fighter.isSister())
    {
        throw runtime_error("\n[!] ERROR : Ravening Seduction can only be used by Sisters!\n");
    }

    cout << "\n========================================\n";
    cout << "-< Ravening Seduction >- ACTIVATED!\n";

    Board &board = game.getBoard();
    vector<Fighter *> fighters;
    cout << "[?] Choose a Fighter to move :\n";
    cout << "========================================\n";

    Hero *hero = player->getHero();
    if (hero != nullptr && hero->isAlive())
    {
        fighters.push_back(hero);
        cout << fighters.size() << ". " << hero->getName() << endl;
    }
    for (auto sidekick : player->getSideKicks())
    {
        if (sidekick->isAlive())
        {
            fighters.push_back(sidekick);
            cout << fighters.size() << ". " << sidekick->getName() << endl;
        }
    }
    if (fighters.empty())
    {
        cout << "\n[!] ERROR : No fighter available!\n";
        return;
    }
    int choice;
    cout << "\n~~> ";
    cin >> choice;

    if (choice < 1 || choice > fighters.size())
    {
        throw runtime_error("\n[!] ERROR : Invalid fighter!\n");
    }
    Fighter *selected = fighters[choice - 1];

    vector<Space *> homes = board.getAvailableMoves(selected, 2);
    if (homes.empty())
    {
        cerr << "\n[!] ERROR : No available movement!\n";
        return;
    }

    cout << "\n[o] Available Homes :";
    cout << "\n==============================\n";
    for (int i = 0; i < homes.size(); i++)
    {
        cout << i + 1 << ". Home " << homes[i]->getId() << endl;
    }
    int moveChoice;
    cout << "\n> Choose destination : ";
    cin >> moveChoice;
    if (moveChoice < 1 || moveChoice > homes.size())
    {
        throw runtime_error("\n[!] ERROR : Invalid destination!\n");
    }
    board.moveFighter(selected, homes[moveChoice - 1]);
    
    Space *currentSpace = homes[moveChoice - 1];

    int damage = 0;
    for (auto neighbor : currentSpace->getNeighbors())
    {
        Fighter *adjacent = neighbor->getFighter();
        if (adjacent == nullptr)
        {
            continue;
        }
        if (adjacent->isSister() && adjacent->isAlive())
        {
            damage++;
        }
    }
    if (damage > 0)
    {
        selected->takeDamage(damage);
        cout << "\n[-] " << selected->getName() << " took " << damage << " damage.\n";
        cout << "\n========================================\n";
    }
    else
    {
        cout << "\n[!] NO adjacent sister, NO damage.\n";
    }
}

string RaveningSeduction::getDescription() const
{
    return "> Move one of your fighters up to 2 homes.Then deal 1 damage to that fighter for each adjacent Sister.";
}

Effect *RaveningSeduction::clone() const
{
    return new RaveningSeduction(*this);
}