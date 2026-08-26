#include "fighter/Dracula.h"
#include "game/Game.h"
#include "game/CombatSystem.h"
#include "player/Player.h"
#include "board/Space.h"
#include "board/Board.h"
#include <iostream>
#include <iomanip>
using namespace std;

Dracula::Dracula() : Hero("DRACULA", 13, 2, AttackType::Melee, 
"> At the start of your turn, deal 1 damage to a fighter adjacent to Dracula (even sisters). then draw a card.")
{
}

void Dracula::useAbility(Game& game, Player& player)
{
}
