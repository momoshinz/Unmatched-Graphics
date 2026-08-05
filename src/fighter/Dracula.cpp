#include "fighter/Dracula.h"
#include "game/Game.h"
#include "game/CombatSystem.h"
#include "player/Player.h"
#include "board/Space.h"
#include "board/Board.h"
#include <iostream>
#include <iomanip>
using namespace std;

Dracula::Dracula() : Hero("DRACULA", 13, 2, AttackType::Melee, 
"> At the start of your turn, you may deal 1 damage to a fighter adjacent to Dracula (even your sisters). if you do draw a card.")
{
}

void Dracula::useAbility(Game& game, Player& player)
{
    if (!isAlive())
    {
        throw runtime_error("\n[!] ERROR : Dracula is DEFEATED!\n");
    }

    cout << "\n[?] Use Dracula's ability? { 1 = Yes, 0 = No } : ";

    int answer;
    cin >> answer;

    while(answer != 1 && answer != 0)
    {
        cout << "try a valid number :) ~~> ";
        cin >> answer;
    }

    if (answer == 0)
    {
        return;
    }

    vector<Fighter*> adjacentFighters;

    Board& board = game.getBoard();

    for (Space* space : board.getAdjacentSpaces(getPosition()))
    {
        Fighter* fighter = space->getFighter();

        if (fighter != nullptr && fighter->isAlive())
        {
            adjacentFighters.push_back(fighter);
        }
    }

    if (adjacentFighters.empty())
    {
        cout << "\n[!] There is no adjacent fighter.\n";
        return;
    }

    cout << "\nChoose a fighter to damage :\n";

    for (size_t i = 0; i < adjacentFighters.size(); i++)
    {
        cout << i + 1 << ". " << adjacentFighters[i]->getName() << " { HP: " 
             << adjacentFighters[i]->getHealth() << " }\n";
    }
    cout << "~~> ";
    int choice;
    cin >> choice;

    if (choice < 1 || choice > adjacentFighters.size())
    {
        throw out_of_range("\n[!] ERROR : Invalid fighter!\n");
    }

    Fighter* target = adjacentFighters[choice - 1];

    cout << "\n[+] Dracula uses his SPECIAL ABILITY!\n";

    target->takeDamage(1);

    cout << "[-] " << target->getName() << " took 1 damage!\n";

    player.drawCardToHand();
}
