#include "fighter/InvisibleMan.h"
#include "game/Game.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;

InvisibleMan::InvisibleMan()
    : Hero("Invisible Man", 15, 2, AttackType::Melee,
           "> Sherlock Holmes and Dr. Watson's abilities can never be disabled.")
{
}

void InvisibleMan::useAbility(Game &game, Player &player)
{
}


