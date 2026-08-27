#pragma once
#include <vector>

using namespace std;

class Card;
class Deck
{
    private:
        vector<Card *> cards;

    public:
        Deck();
        ~Deck();

        void shuffle();
        Card *drawCard();
        void addCopies(const Card &card, int count);

        void addCard(Card *card);

        int getSize() const;
        bool isEmpty() const;

        const vector<Card *> &getCards() const;

        static void DraculaDeck(Deck& deck);
        static void SherlockDeck(Deck& deck);
        static void InvisibleManDeck(Deck& deck);

        void addToTop(Card* card);
};