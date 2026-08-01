#include "fighter/DrWatson.h"
#include <iostream>
#include <iomanip>

using namespace std;

Watson::Watson()
    : Sidekick("Dr. Watson", 8, 2, AttackType::Ranged)
{
}

bool Watson::isAbilityImmune() const
{
    return true;
}