#include "effect/SherlockCards/EliminateTheImpossible.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "card/Card.h"
#include "card/Hand.h"
#include <stdexcept>

using namespace std;

void EliminateTheImpossible::apply(Game &game, Fighter &fighter, Fighter &target,
                                   const Card &self, Card *opponentCard, bool didUserWin,
                                   const EffectChoice &choice)
{
    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Eliminate The Impossible can only be used by Sherlock Holmes!\n");
    }

    Player *currentPlayer = fighter.getOwner();
    if (currentPlayer == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    Player *opponent = game.selectOpponent(*currentPlayer);
    if (opponent == nullptr)
    {
        throw runtime_error("\n[!] ERROR : NO opponent selected!\n");
    }

    Hand &opponentHand = opponent->getHand();
    if (opponentHand.isEmpty())
    {
        return;
    }

    if (choice.selectedCardIndex < 0 || choice.selectedCardIndex >= opponentHand.getSize())
    {
        throw out_of_range("\n[!] ERROR : Invalid card selection!\n");
    }

    Card *burnedCard = opponentHand.removeCard(choice.selectedCardIndex);
    delete burnedCard;
}

EffectInputKind EliminateTheImpossible::getInputKind() const
{
    return EffectInputKind::ChooseOpponentCardToBurn;
}

string EliminateTheImpossible::getDescription() const
{
    return "> Look at your opponent's hand, choose 1 card, and burn it.";
}

Effect *EliminateTheImpossible::clone() const
{
    return new EliminateTheImpossible(*this);
}