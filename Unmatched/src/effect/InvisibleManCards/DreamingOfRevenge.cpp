#include "effect/InvisibleManCards/DreamingOfRevenge.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "fighter/Hero.h"
#include "fighter/Sidekick.h"
#include "board/Space.h"
#include <iostream>
#include <stdexcept>
using namespace std;

void DreamingOfRevenge::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    Player* player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

     if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Dreaming Of Revenge can only be used by Invisible Man!\n");
    }

    Player* opponentPlayer = target.getOwner();
    if(opponentPlayer == nullptr)
    {
        cerr << "\n[!] ERROR : Target has NO owner!\n";
        return;
    }

    Space* mySpace = fighter.getPosition();
    if(mySpace == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has no position!");
    }
    if(!mySpace->hasFogToken())
    {
        cout << "\n[!] ERROR : Invisible Man is not on a Fog token. card has no effect!\n";
        return;
    }
    cout << "\n========================================";
    cout << "\n-< Dreaming Of Revenge >- ACTIVATED!\n";


    int damaged = 0;
    Hero* enemyHero = opponentPlayer->getHero();
    if(enemyHero != nullptr && enemyHero->isAlive() && enemyHero->getPosition() != nullptr && enemyHero->getPosition()->hasFogToken())
    {
        enemyHero->takeDamage(1);
        damaged++;
        cout << "\n[-] " << enemyHero->getName() << " took 1 damage.\n";
    }
    for(Sidekick* sidekick : opponentPlayer->getSideKicks())
    {
        if(sidekick == nullptr)
        {
            continue;
        }
        if(!sidekick->isAlive())
        {
            continue;
        }
        if(sidekick->getPosition() == nullptr)
        {
            continue;
        }
        if(!sidekick->getPosition()->hasFogToken())
        {
            continue;
        }
        sidekick->takeDamage(1);
        damaged++;
        cout << "\n[-] " << sidekick->getName() << " took 1 damage.\n";
    }
    if(damaged == 0)
    {
        cout << "\n[!] ERROR : No opposing fighter was on a Fog token!\n";
    }
    else
    {
        cout << "\n[+] Dreaming Of Revenge dealt damage to " << damaged << " fighter(s).\n";
        cout << "\n========================================";
    }
}

string DreamingOfRevenge::getDescription() const
{
    return "> If invisible Man is on a Fog token, deal 1 damage to every opposing fighter on a Fog token.";
}

Effect *DreamingOfRevenge::clone() const
{
    return new DreamingOfRevenge(*this);
}