#include "effect/SherlockCards/DeduceStrategy.h"
#include "game/Game.h"
#include "fighter/Fighter.h"
#include "board/Board.h"
#include "card/Card.h"
#include <iostream>

using namespace std;

void DeduceStrategy::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Deduce Strategy can only be used by Sherlock Holmes!\n");
    }

    fighter.enableOpponentBoostValue();

    cout << "\n========================================";
    cout << "\n-< Deduce Strategy >- ACTIVATED!\n";
    cout << "[+] Opponent's printed combat value is now replaced\n";
    cout << "    by its BOOST value.\n";
    cout << "========================================\n";
}

string DeduceStrategy::getDescription() const
{
    return "> Opponent printed value will become its Boost value.";
}

Effect *DeduceStrategy::clone() const
{
    return new DeduceStrategy(*this);
}