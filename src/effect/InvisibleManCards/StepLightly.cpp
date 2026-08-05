#include "effect/InvisibleManCards/StepLightly.h"
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

void StepLightly::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    Player* player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    Board &board = game.getBoard();
    vector<Fighter *> adjacentEnemies;

    for (Space* space : board.getAdjacentSpaces(fighter.getPosition()))
    {
        Fighter* enemy = space->getFighter();

        if (enemy != nullptr && enemy->getOwner() != player && enemy->isAlive())
        {
            adjacentEnemies.push_back(enemy);
        }
    }

    if (adjacentEnemies.empty())
    {
        cout << "\n[!] ERROR : No adjacent enemy.\n";
    }
    else
    {
        cout << "\n========================================\n";
        cout << "-< Step Lightly >- ACTIVATED!\n";

        cout << "\n> Choose enemy:\n";

        for (int i = 0; i < adjacentEnemies.size(); i++)
        {
            cout << i+1 << ". " << adjacentEnemies[i]->getName() << endl;
        }

        int choice;
        cin >> choice;

        if (choice < 1 || choice > adjacentEnemies.size())
        {
            throw runtime_error("\n[!] ERROR : Invalid fighter!\n");
        }

        int damage = 1;
        if (fighter.getPosition()->hasFogToken())
        {
            damage = 3;
        }
        adjacentEnemies[choice - 1]->takeDamage(damage);
        cout << "\n[+] " << adjacentEnemies[choice - 1]->getName() << " took " << damage << " damage.\n";
    }

    Player *opponent = nullptr;
    for (Player *p : game.getPlayers())
    {
        if (p != player)
        {
            opponent = p;
            break;
        }
    }

    if (opponent == nullptr)
    {
        return;
    }

    vector<Fog *> fogs = player->getFogs();
    if (fogs.empty())
    {
        return;
    }

    cout << "\n> Opponent chooses a Fog token to move.\n";

    for (int i = 0; i < fogs.size(); i++)
    {
        cout << i + 1 << ". Fog " << fogs[i]->getID();
        if (fogs[i]->getPosition() != nullptr)
        {
            cout << " (Home " << fogs[i]->getPosition()->getId() << ")";
        }
        cout << endl;
    }

    int fogChoice;
    cin >> fogChoice;

    if (fogChoice < 1 || fogChoice > fogs.size())
    {
        throw runtime_error("\n[!] ERROR : Invalid Fog!\n");
    }

    Fog *fog = fogs[fogChoice - 1];
    vector<Space *> moves = board.getAvailableFogMoves(fog, 2);
    if (moves.empty())
    {
        cout << "\n[!] ERROR : No available destination.\n";
        return;
    }

    cout << "\n> Choose a destination:\n";
    for (size_t i = 0; i < moves.size(); i++)
    {
        cout << i + 1 << ". Home " << moves[i]->getId() << endl;
    }

    int moveChoice;
    cin >> moveChoice;
    if (moveChoice < 1 || moveChoice > moves.size())
    {
        throw runtime_error("\n[!] ERROR : Invalid destination!\n");
    }
    if (fog->getPosition() != nullptr)
    {
        fog->getPosition()->setFogToken(false);
    }

    fog->setPosition(moves[moveChoice - 1]);
    moves[moveChoice - 1]->setFogToken(true);
    cout << "\n[+] Fog moved successfully.\n";
    cout << "\n========================================\n";
}

string StepLightly::getDescription() const
{
    return "> Deal 1 damage to an adjacent fighter (3 instead if Invisible Man is on a Fog). Then your opponent moves one Fog token up to 2 homes.";
}

Effect *StepLightly::clone() const
{
    return new StepLightly(*this);
}