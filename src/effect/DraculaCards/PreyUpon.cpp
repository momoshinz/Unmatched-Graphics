#include "effect/DraculaCards/PreyUpon.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "fighter/Hero.h"
#include "fighter/Sidekick.h"
#include "board/Space.h"
#include "board/Board.h"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

void PreyUpon::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    Player *player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }
    Player* opponentPlayer = target.getOwner();
    if(opponentPlayer == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Target has NO owner!\n");
        return;
    }

    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Prey Upon can only be used by Dracula!\n");
    }

    vector<Fighter*> adjacentOpponents;
    Hero* opponentHero = opponentPlayer->getHero();
    if(opponentHero != nullptr && opponentHero->isAlive())
    {
        if(fighter.isAdjacent(opponentHero, game.getBoard()))
        {
            adjacentOpponents.push_back(opponentHero);
        }
    }
    for(Sidekick* sidekick : opponentPlayer->getSideKicks())
    {
        if(sidekick != nullptr && sidekick->isAlive())
        {
            if(fighter.isAdjacent(sidekick, game.getBoard()))
            {
                adjacentOpponents.push_back(sidekick);
            }
        }
    }

    if(adjacentOpponents.empty())
    {
        cerr << "\n[!] ERROR : No adjacent opponent found!\n";
        return;
    }
    cout << "\n========================================\n";
    cout << "-< Prey Upon >- ACTIVATED!\n";

    int totalDamage = 0;
    cout << "\n[o] Affected fighters :\n";
    for(auto opponent : adjacentOpponents)
    {
        opponent->takeDamage(1);
        totalDamage++;
        cout << "> " << opponent->getName() << " took 1 Damage!\n";
    }

    fighter.heal(totalDamage);
    cout << "[-] Total damage amount : " << totalDamage << "\n";
    cout << "[+] " << fighter.getName() << " recovered " << totalDamage << " health!\n";
    cout << "========================================\n";
}

string PreyUpon::getDescription() const
{
    return "> Deal 1 damage to all opposing fighters adjacent to Dracula. Dracula recovers 1 health for each damage dealt.";
}

Effect *PreyUpon::clone() const
{
    return new PreyUpon(*this);
}