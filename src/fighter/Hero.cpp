#include "fighter/Hero.h"

Hero::Hero(const string &name,
           int maxHealth,
           int movement,
           AttackType attackType,
           const string &abilityDescription)
    : Fighter(name, maxHealth, movement, attackType),
      abilityDescription(abilityDescription)
{
}

const string &Hero::getAbilityDescription() const
{
    return abilityDescription;
}

void Hero::useAbility(Game& game, Player &player)
{
}

bool Hero::isAbilityImmune() const
{
    return false;
}

bool Hero::isHero() const
{
    return true;
}