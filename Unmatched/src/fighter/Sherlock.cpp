#include "fighter/Sherlock.h"
#include "game/Game.h"

#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;

Sherlock::Sherlock()
    : Hero("SHERLOCK HOLMES", 16, 2, AttackType::Melee,
           "> Sherlock Holmes and Dr. Watson's abilities can never be disabled.")
{
}

void Sherlock::useAbility(Game &game, Player &player)
{
}

bool Sherlock::isAbilityImmune() const
{
    return true;
}

