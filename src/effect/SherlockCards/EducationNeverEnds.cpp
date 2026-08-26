#include "effect/SherlockCards/EducationNeverEnds.h"
#include "game/Game.h"
#include "fighter/Fighter.h"
#include "fighter/Hero.h"
#include "card/Card.h"
#include <stdexcept>

using namespace std;

void EducationNeverEnds::apply(Game &game, Fighter &fighter, Fighter &target,
                               const Card &self, Card *opponentCard, bool didUserWin,
                               const EffectChoice &choice)
{
    Player *player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }
}

string EducationNeverEnds::getDescription() const
{
    return "> If you win the combat, your opponent draws 1 card, but if you lose the combat, you draw 2 cards.";
}

Effect *EducationNeverEnds::clone() const
{
    return new EducationNeverEnds(*this);
}