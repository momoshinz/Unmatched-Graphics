#include "effect/DraculaCards/BaptismOfBlood.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Sisters.h"
#include "fighter/Sidekick.h"
#include "fighter/Hero.h"
#include "board/Board.h"
#include "board/Space.h"
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

void BaptismOfBlood::apply(Game &game,
                           Fighter &fighter,
                           Fighter &target,
                           const Card &self,
                           Card *opponentCard,
                           bool didUserWin,
                           const EffectChoice &choice)
{
    Player *player = fighter.getOwner();

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

    cout << "[+] " << fighter.getName() << " recovered 2 health!\n";

    if (choice.selectedFighter == nullptr)
    {
        cout << "[o] No defeated Sister available.\n";
        cout << "========================================\n";
        return;
    }

    Sisters *selectedSister = dynamic_cast<Sisters *>(choice.selectedFighter);

    if (selectedSister == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Selected fighter is not a Sister!\n");
    }

    if (selectedSister->getOwner() != player)
    {
        throw runtime_error("\n[!] ERROR : Selected Sister does not belong to Dracula!\n");
    }

    if (selectedSister->isAlive())
    {
        throw runtime_error("\n[!] ERROR : Selected Sister is not defeated!\n");
    }

    Space *selectedSpace = choice.selectedSpace;

    if (selectedSpace == nullptr)
    {
        throw runtime_error(
            "\n[!] ERROR : No destination selected for Sister!\n");
    }

    if (selectedSpace->isOccupied())
    {
        throw runtime_error("\n[!] ERROR : Selected home is already OCCUPIED!\n");
    }

    Space *draculaSpace = fighter.getPosition();

    if (draculaSpace == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Dracula has NO position!\n");
    }

    const vector<ZoneType> &draculaZones = draculaSpace->getZones();

    if (draculaZones.empty())
    {
        throw runtime_error("\n[!] ERROR : Dracula is not in any ZONE!\n");
    }

    const vector<ZoneType> &targetZones = selectedSpace->getZones();

    bool sameZone = false;

    for (ZoneType draculaZone : draculaZones)
    {
        for (ZoneType targetZone : targetZones)
        {
            if (draculaZone == targetZone)
            {
                sameZone = true;
                break;
            }
        }
        if (sameZone)
            break;
    }

    if (!sameZone)
    {
        throw runtime_error(
            "\n[!] ERROR : Selected home is NOT in Dracula's zone!\n");
    }

    selectedSister->heal(selectedSister->getMaxHealth());

    selectedSister->setPosition(selectedSpace);
    selectedSpace->setFighter(selectedSister);

    cout << "[+] "
         << selectedSister->getName()
         << " has been revived in Dracula's zone.\n";

    cout << "[o] Sister moved to Home "
         << selectedSpace->getId()
         << "\n";

    cout << "========================================\n";
}

EffectInputKind BaptismOfBlood::getInputKind() const
{
    return EffectInputKind::ChooseDefeatedSisterAndZoneSpace;
}
string BaptismOfBlood::getDescription() const
{
    return "> Recover 2 HEALTH. Return a defeated Sister (if any) to an empty space in Dracula's zone.";
}

Effect *BaptismOfBlood::clone() const
{
    return new BaptismOfBlood(*this);
}