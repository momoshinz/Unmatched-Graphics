#include "ui/TerminalUI.h"
#include "game/Game.h"
#include "board/Board.h"
#include "fighter/Hero.h"
#include "fighter/Dracula.h"
#include "fighter/Sherlock.h"
#include "fighter/Sidekick.h"
#include "fighter/Sisters.h"
#include "fighter/InvisibleMan.h"
#include "fighter/Fog.h"
#include "player/Player.h"
#include <iomanip>
#include <limits>
#include <string>

using namespace std;

TerminalUI::TerminalUI()
{
}

string TerminalUI::getAttackTypeString(Fighter *fighter) const
{
    if (fighter == nullptr)
        return "?";

    switch (fighter->getAttackType())
    {
    case AttackType::Melee:
        return "Melee";
    case AttackType::Ranged:
        return "Ranged";
    default:
        return "?";
    }
}