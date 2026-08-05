#include "effect/DraculaCards/FeedingFrenzy.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "fighter/Sisters.h"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

void FeedingFrenzy::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    Player *player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }
    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Feeding Frenzy can only be used by Dracula!\n");
    }

    Space* targetPos = target.getPosition();
    if(targetPos == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Target has NO position on the map!\n");
    }

    vector<ZoneType> targetZones = targetPos->getZones();

    int sisterCount = 0;
    for(Sisters* sister : player->getSisters())
    {
        if(!sister->isAlive())
        {
            continue;
        }
        Space* sisterPos = sister->getPosition();
        if(sisterPos == nullptr)
        {
            continue;
        }
        vector<ZoneType> sisterZones = sisterPos->getZones();

        bool sameZone = false;
        for(ZoneType Tzones : targetZones)
        {
            for(ZoneType Szones : sisterZones)
            {
                if(Tzones == Szones)
                {
                    cout << "\n========================================\n";
                    cout << "-< Feeding Frenzy >- ACTIVATED!\n";
                    sameZone = true;
                    break;
                }
            }
            if(sameZone)
            {
                break;
            }
        }
        if(sameZone)
        {
            sisterCount++;
        }
    }
    fighter.addTempAttackBoost(sisterCount);
    cout << "[+] " << sisterCount << " Sister(s) in the same zone as the target!\n";
    cout << "[+] Attack value increased by " << sisterCount << ".\n";
    cout << "========================================\n";
}

string FeedingFrenzy::getDescription() const
{
    return "> This card's ATTACK value is +1 for each Sister in the same zone as opposing fighter.";
}

Effect* FeedingFrenzy::clone() const
{
    return new FeedingFrenzy(*this);
}