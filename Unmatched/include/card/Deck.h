#ifndef DECK_H
#define DECK_H
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
};

#endif