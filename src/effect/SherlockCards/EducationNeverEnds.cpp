#include "effect/SherlockCards/EducationNeverEnds.h"
#include "game/Game.h"
#include "fighter/Fighter.h"
#include "fighter/Hero.h"
#include "card/Card.h"
#include <iostream>

using namespace std;

void EducationNeverEnds::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    cout << "\n========================================\n";
    cout << "-< Education Never Ends >- ACTIVATED!\n";

    if (didUserWin)
    {
        bool ok = target.getOwner()->drawCardToHand();

        if (ok)
        {
            cout << "[+] " << target.getOwner()->getHero()->getName() << " drew 1 card.\n";
        }
    }
    else
    {
        int drawn = 0;

        if (fighter.getOwner()->drawCardToHand())
            drawn++;

        if (fighter.getOwner()->drawCardToHand())
            drawn++;

        cout << "[+] " << fighter.getName() << " drew " << drawn << " card" << (drawn == 1 ? "" : "s") << ".\n";
    }
    cout << "========================================\n";
}

string EducationNeverEnds::getDescription() const
{
    return "> If you win the combat, your opponent draws 1 card, but if you lose the combat, you draw 2 cards.";
}

Effect *EducationNeverEnds::clone() const
{
    return new EducationNeverEnds(*this);
}