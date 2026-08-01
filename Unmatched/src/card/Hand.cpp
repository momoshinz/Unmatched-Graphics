#include "card/Hand.h"
#include "card/Card.h"

#include <iostream>
#include <stdexcept>

using namespace std;

Hand::Hand()
{
}

void Hand::addCard(Card *card)
{
    if (card == nullptr)
    {
        throw invalid_argument("\n[!] ERROR : Card cannot be NULL!\n");
    }

    cards.push_back(card);
}

Card *Hand::removeCard(int index)
{
    if (index < 0 || index >= cards.size())
    {
        throw out_of_range("\n[!] ERROR : NO card FOUND!\n");
    }

    Card *selectedCard = cards[index];

    cards.erase(cards.begin() + index);

    return selectedCard;
}

Card *Hand::getCard(int index) const
{
    if (index < 0 || index >= cards.size())
    {
        throw out_of_range("\n[!] ERROR : NO card FOUND!\n");
    }

    return cards[index];
}

bool Hand::isEmpty() const
{
    return cards.empty();
}

int Hand::getSize() const
{
    return cards.size();
}

const vector<Card *> &Hand::getCards() const
{
    return cards;
}

void Hand::display() const
{
    if (cards.empty())
    {
        cout << "Hand is EMPTY!\n";
        return;
    }

    for (int i = 0; i < cards.size(); i++)
    {
        cout << "\nCard {" << i + 1 << "}\n\n";
        cards[i]->display();
        cout << endl;
    }
}