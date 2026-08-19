#include "effect/SherlockCards/StudyMethods.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "card/Hand.h"
#include <iostream>
#include <stdexcept>

using namespace std;

void StudyMethods::apply(Game &game, Fighter &fighter, Fighter &target,
                         const Card &self, Card *opponentCard, bool attackerWon,
                         const EffectChoice &choice)
{
    // اگه به اینجا رسیدیم یعنی shouldRequestInput از قبل تایید کرده
    // که attackerWon == true بوده، پس دیگه لازم نیست دوباره چک کنیم.
    // خود نمایش دست حریف رو EffectUI قبل از این apply انجام داده،
    // پس اینجا کار خاصی لازم نیست انجام بشه.

    cout << "\n========================================\n";
    cout << "-< Study Methods >- ACTIVATED!\n";
    cout << "[+] You looked at your opponent's hand.\n";
    cout << "========================================\n";
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