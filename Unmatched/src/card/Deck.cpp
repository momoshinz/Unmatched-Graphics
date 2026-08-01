#include "card/Deck.h"
#include "card/Card.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <stdexcept>

using namespace std;

Deck::Deck()
{
}

Deck::~Deck()
{
    for (Card *card : cards)
    {
        delete card;
    }

    cards.clear();
}

void Deck::addCard(Card *card)
{
    if (card == nullptr)
    {
        throw invalid_argument("\n[!] ERROR : Card cannot be NULL!\n");
    }

    cards.push_back(card);
}

Card *Deck::drawCard()
{
    if (cards.empty())
    {
        return nullptr;
    }

    Card *topCard = cards.back();

    cards.pop_back();

    return topCard;
}

void Deck::shuffle()
{
    random_device rd;
    mt19937 generator(rd());

    std::shuffle(cards.begin(), cards.end(), generator);
}

int Deck::getSize() const
{
    return cards.size();
}

bool Deck::isEmpty() const
{
    return cards.empty();
}

const vector<Card *> &Deck::getCards() const
{
    return cards;
}

void Deck::addCopies(const Card &card, int count)
{
    for (int i = 1; i <= count; i++)
    {
        addCard(new Card(card));
    }
}

void Deck::DraculaDeck(Deck &deck)
{
    deck.addCopies(Card::createFeedingFrenzy(), 2);
    deck.addCopies(Card::createMistForm(), 2);
    deck.addCopies(Card::createAmbush(), 2);
    deck.addCopies(Card::createBaptismOfBlood(), 2);
    deck.addCopies(Card::createBeastform(), 2);
    deck.addCopies(Card::createDash(), 3);
    deck.addCopies(Card::createExploit(), 3);
    deck.addCopies(Card::createLookIntoMyEyes(), 3);
    deck.addCopies(Card::createPreyUpon(), 2);
    deck.addCopies(Card::createRaveningSeduction(), 3);
    deck.addCopies(Card::createThirstForSustenance(), 3);
    deck.addCopies(Card::createFeintDracula(), 3);

    deck.shuffle();
}

void Deck::SherlockDeck(Deck &deck)
{
    deck.addCopies(Card::createAdministerAid(), 2);
    deck.addCopies(Card::createCounterPunch(), 3);
    deck.addCopies(Card::createDeduceStrategy(), 3);
    deck.addCopies(Card::createEducationNeverEnds(), 2);
    deck.addCopies(Card::createEliminateTheImpossible(), 2);
    deck.addCopies(Card::createFeintSherlock(), 3);
    deck.addCopies(Card::createFixedPoint(), 2);
    deck.addCopies(Card::createMasterOfDisguise(), 2);
    deck.addCopies(Card::createTheGameIsAfoot(), 2);
    deck.addCopies(Card::createServiceRevolver(), 2);
    deck.addCopies(Card::createStudyMethods(), 2);

    deck.shuffle();
}