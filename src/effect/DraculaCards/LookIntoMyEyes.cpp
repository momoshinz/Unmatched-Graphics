#include "effect/DraculaCards/LookIntoMyEyes.h"
#include "game/Game.h"
#include "fighter/Fighter.h"
#include "player/Player.h"
#include "card/Card.h"
#include <iostream>
#include <stdexcept>

using namespace std;

void LookIntoMyEyes::apply(Game &game,
                           Fighter &fighter,
                           Fighter &target,
                           const Card &self,
                           Card *opponentCard,
                           bool didUserWin,
                           const EffectChoice &choice)
{
    Player *player = fighter.getOwner();

    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Look Into My Eyes can only be used by Dracula!\n");
    }

    if (opponentCard == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Opponent has no card!\n");
    }

    cout << "\n========================================\n";
    cout << "-< Look Into My Eyes >- ACTIVATED!\n";

    int opponentBoost = opponentCard->getBoost();

    fighter.enableOpponentBoostValue();
    fighter.setOpponentBoostValue(opponentBoost);

    cout << "[+] Opponent's attack card boost: " << opponentBoost << "\n";

    cout << "[+] Adding " << opponentBoost << " to defense value!\n";

    cout << "========================================\n";
}

EffectInputKind LookIntoMyEyes::getInputKind() const
{
    return EffectInputKind::None;
}

string LookIntoMyEyes::getDescription() const
{
    return "> Add the BOOST value from your opponent's attack card to the defense value of this card.";
}

Effect *LookIntoMyEyes::clone() const
{
    return new LookIntoMyEyes(*this);
}