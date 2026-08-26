#include "effect/InvisibleManCards/CodedNotes.h"
#include "game/Game.h"
#include "player/Player.h"
#include "card/Hand.h"
#include "card/Deck.h"
#include "card/Card.h"
#include <stdexcept>
using namespace std;

void CodedNotes::apply(Game &game, Fighter &fighter, Fighter &target,
                       const Card &self, Card *opponentCard, bool didUserWin,
                       const EffectChoice &choice)
{
    Player *player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Coded Notes can only be used by Invisible Man!\n");
    }

    Hand &hand = player->getHand();
    Deck &deck = player->getDeck();

    if (choice.selectedCardIndices.size() != 2)
    {
        throw runtime_error("\n[!] ERROR : Exactly 2 cards must be selected!\n");
    }

    int idx1 = choice.selectedCardIndices[0];
    int idx2 = choice.selectedCardIndices[1];

    if (idx1 < 0 || idx1 >= hand.getSize() || idx2 < 0 || idx2 >= hand.getSize() || idx1 == idx2)
    {
        throw out_of_range("\n[!] ERROR : Invalid card selection!\n");
    }

    if (choice.selectedOrder != 1 && choice.selectedOrder != 2)
    {
        throw runtime_error("\n[!] ERROR : Invalid order selection!\n");
    }

    Card *firstCard;
    Card *secondCard;

    if (idx1 > idx2)
    {
        firstCard = hand.removeCard(idx1);
        secondCard = hand.removeCard(idx2);
    }
    else
    {
        secondCard = hand.removeCard(idx2);
        firstCard = hand.removeCard(idx1);
    }

    if (choice.selectedOrder == 1)
    {
        deck.addToTop(secondCard);
        deck.addToTop(firstCard);
    }
    else
    {
        deck.addToTop(firstCard);
        deck.addToTop(secondCard);
    }
}

EffectInputKind CodedNotes::getInputKind() const
{
    return EffectInputKind::ChooseTwoCardsAndOrder;
}

string CodedNotes::getDescription() const
{
    return "> Draw 3 cards. Then place any 2 cards from your hand on top of your deck in any order.";
}

Effect *CodedNotes::clone() const
{
    return new CodedNotes(*this);
}