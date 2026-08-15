#include "fighter/InvisibleMan.h"
#include "game/Game.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;

InvisibleMan::InvisibleMan() : Hero("INVISIBLE MAN", 15, 2, AttackType::Melee, "> If Invisible Man is on a Fog while defending, "
                                                                               "his defense value gains +1. "
                                                                               "He can also move directly between Fogs.")
{
}

bool InvisibleMan::isOnFog() const
{
    if (getPosition() == nullptr)
        return false;

    return getPosition()->hasFogToken();
}

bool InvisibleMan::canTeleport() const
{
    return isOnFog();
}

void InvisibleMan::useAbility(Game &game, Player &player)
{
}
