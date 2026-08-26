#include "effect/SherlockCards/DeduceStrategy.h"
#include "game/Game.h"
#include "fighter/Fighter.h"
#include "board/Board.h"
#include "card/Card.h"
#include <stdexcept>

using namespace std;

void DeduceStrategy::apply(Game &game, Fighter &fighter, Fighter &target,
                           const Card &self, Card *opponentCard, bool didUserWin,
                           const EffectChoice &choice)
{
    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Deduce Strategy can only be used by Sherlock Holmes!\n");
    }

    fighter.enableOpponentBoostValue();
}

string DeduceStrategy::getDescription() const
{
    return "> Opponent printed value will become its Boost value.";
}

Effect *DeduceStrategy::clone() const
{
    return new DeduceStrategy(*this);
}