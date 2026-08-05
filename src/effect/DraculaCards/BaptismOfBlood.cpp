#include "effect/DraculaCards/BaptismOfBlood.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Sisters.h"
#include "fighter/Sidekick.h"
#include <iostream>
using namespace std;

void BaptismOfBlood::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    Player* player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Baptism Of Blood can only be used by Dracula!\n");
    }

    cout << "\n========================================\n";
    cout << "-< Baptism Of Blood >- ACTIVATED!\n";

    fighter.heal(2);
    cout << "[+] " << fighter.getName() << " recovered 2 health!" << endl;

    Sisters* defeatedSister = nullptr;
    for(auto sidekick : player->getSideKicks())
    {
        if(sidekick->isSister() && !sidekick->isAlive())
        {
            defeatedSister = static_cast<Sisters*>(sidekick);
            break;
        }
    }
    if(defeatedSister != nullptr)
    {
        vector<ZoneType> draculaZones;
        for(auto space : game.getBoard().getSpaces())
        {
            if(space->getFighter() == &fighter)
            {
                draculaZones = space->getZones();
                break;
            }
        }
        if(draculaZones.empty())
        {
            cerr << "\n[!] ERROR : Dracula is not in any ZONE!\n";
            return;
        }
        Space* targetSpace = nullptr;
        for(auto space : game.getBoard().getSpaces())
        {
            if(space->isOccupied())
            {
                continue;
            }
            const vector<ZoneType> &zones = space->getZones();
            for(int i=0 ; i<zones.size() ; i++)
            {
                for(int j=0 ; j<draculaZones.size() ; j++)
                {
                    if(zones[i] == draculaZones[j])
                    {
                        targetSpace = space;
                        break;
                    }
                }
                if(targetSpace != nullptr)
                {
                    break;
                }
            }
            if(targetSpace != nullptr)
            {
                break;
            }
        }
        if(targetSpace != nullptr)
        {
            defeatedSister->heal(defeatedSister->getMaxHealth());
            defeatedSister->setPosition(targetSpace);
            targetSpace->setFighter(defeatedSister);
            cout << "[+] " << defeatedSister->getName() << " has been revived in Dracula's zone.\n";
            cout << "========================================\n";
        }
        else
        {
            cout << "\n[!] ERROR : No empty home in Dracula's zone to revive Sister!\n";
            cout << "========================================\n";
        }
    }
    else
    {
        cout << "\n[!] ERROR : No defeated Sister to revive.\n";
        cout << "========================================\n";
    }
}

string BaptismOfBlood::getDescription() const
{
    return "> Recover 2 HEALTH. Retrun a defeated Sister(if any) to space in Dracula's zone.";
}

Effect* BaptismOfBlood::clone() const
{
    return new BaptismOfBlood(*this);
}