#include "effect/DraculaCards/Ambush.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "card/Card.h"
#include "card/Hand.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdlib>

using namespace std;

void Ambush::apply(Game &game,
                   Fighter &fighter,
                   Fighter &target,
                   const Card &self,
                   Card *opponentCard,
                   bool didUserWin,
                   const EffectChoice &choice)
{
    Player *opponent = target.getOwner();

    if (opponent == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Opponent has NO owner!\n");
    }

    const vector<Card *> &cards = opponent->getHand().getCards();

    if (cards.empty())
    {
        throw runtime_error("\n[!] ERROR : Opponent has NO card to discard!\n");
    }

    int randomIndex = rand() % static_cast<int>(cards.size());

    Card *selectedCard = cards[randomIndex];

    if (selectedCard == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Selected opponent card is NULL!\n");
    }

    int boostValue = selectedCard->getBoost();

    Card *removedCard = opponent->getHand().removeCard(randomIndex);

    if (removedCard == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Could not remove opponent card!\n");
    }

    opponent->getDiscardPile().addCard(removedCard);

    fighter.addTempAttackBoost(boostValue);
}

EffectInputKind Ambush::getInputKind() const
{
    return EffectInputKind::None;
}

string Ambush::getDescription() const
{
    return
        "> Your opponent discards a random card.\nAdd its BOOST value to your attack.";
}

Effect *Ambush::clone() const
{
    return new Ambush(*this);
}