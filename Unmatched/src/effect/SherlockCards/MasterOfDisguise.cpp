#include "effect/SherlockCards/MasterOfDisguise.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Hero.h"
#include "fighter/Fighter.h"
#include "board/Board.h"
#include <iostream>
#include <stdexcept>

using namespace std;

void MasterOfDisguise::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Master Of Disguise can only be used by Sherlock Holmes!\n");
    }

    Player *myPlayer = fighter.getOwner();

    if (myPlayer == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    Player *enemyPlayer = game.selectOpponent(*myPlayer);

    if (enemyPlayer == nullptr)
    {
        throw runtime_error("\n[!] ERROR : NO opponent selected!\n");
    }

    vector<Fighter *> enemyFighters;

    if (enemyPlayer->getHero()->isAlive())
    {
        enemyFighters.push_back(enemyPlayer->getHero());
    }

    for (Sidekick *sidekick : enemyPlayer->getSideKicks())
    {
        if (sidekick->isAlive())
        {
            enemyFighters.push_back(sidekick);
        }
    }

    cout << "\nChoose opponent :\n";

    for (int i = 0; i < enemyFighters.size(); i++)
    {
        cout << i + 1 << ". " << enemyFighters[i]->getName() << endl;
    }
    cout << "~~> ";
    int choice;
    cin >> choice;

    while (choice < 1 || choice > enemyFighters.size())
    {
        cout << "Invalid choice. Try again : ";
        cin >> choice;
    }

    Fighter *selectedEnemy = enemyFighters[choice - 1];

    Player *enemyOwner = selectedEnemy->getOwner();

    if (enemyOwner == nullptr)
    {
        throw runtime_error("Selected fighter has no owner! :<");
    }

    Hero *enemyHero = enemyOwner->getHero();

    game.getBoard().swapFighters(&fighter, enemyHero);

    enemyHero->takeDamage(1);

    cout << "\n========================================\n";

    cout << "-< Master Of Disguise >- ACTIVATED!\n";

    cout << fighter.getName() << " swapped places with " << enemyHero->getName() << ".\n";

    cout << enemyHero->getName() << " takes 1 damage!\n";

    cout << enemyHero->getName() << "'s Health : " << enemyHero->getHealth() << "/" << enemyHero->getMaxHealth() << endl;

    cout << "========================================\n";
}

string MasterOfDisguise::getDescription() const
{
    return "> Choose an opponent. Swap Holmes with that opponent's hero and deal 1 damage to that hero.";
}

Effect *MasterOfDisguise::clone() const
{
    return new MasterOfDisguise(*this);
}