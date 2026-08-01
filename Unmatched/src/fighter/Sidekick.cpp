#include "fighter/Sidekick.h"
#include <iostream>
#include <iomanip>
using namespace std;

Sidekick::Sidekick(const string &name, int maxHealth, int movement, AttackType attackType)
    : Fighter(name, maxHealth, movement, attackType)
{
}

bool Sidekick::isSister() const
{
    return false;
}

bool Sidekick::isHero() const
{
    return false;
}

bool Sidekick::isAbilityImmune() const
{
    return false;
}
