#include "effect/SherlockCards/StudyMethods.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "card/Hand.h"
#include <stdexcept>

using namespace std;

void StudyMethods::apply(Game &game, Fighter &fighter, Fighter &target,
                         const Card &self, Card *opponentCard, bool attackerWon,
                         const EffectChoice &choice)
{
    Player *player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }
}

EffectInputKind StudyMethods::getInputKind() const
{
    return EffectInputKind::ShowOpponentHand;
}

bool StudyMethods::shouldRequestInput(Game &game, Fighter &user, Fighter &target, bool didUserWin) const
{
    return didUserWin;
}

string StudyMethods::getDescription() const
{
    return "> If you win the combat, you may look at your opponent's hand.";
}

Effect *StudyMethods::clone() const
{
    return new StudyMethods(*this);
}