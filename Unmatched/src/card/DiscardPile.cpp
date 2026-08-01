#include "card/DiscardPile.h"
#include "card/Card.h"
#include <iostream>
#include <stdexcept>
using namespace std;

DiscardPile::DiscardPile()
{
}

void DiscardPile::addCard(Card *card)
{
    if (card == nullptr)
    {
        throw invalid_argument("\n[!] ERROR : Cannot add NULL card to discard pile!\n");
    }
    cards.push_back(card);
}

int DiscardPile::getSize() const
{
    return cards.size();
}

bool DiscardPile::isEmpty() const
{
    return cards.empty();
}

const vector<Card *> &DiscardPile::getCards() const
{
    return cards;
}
