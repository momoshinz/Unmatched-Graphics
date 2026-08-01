#ifndef DISCARDPILE_H
#define DISCARDPILE_H
#include <vector>

using namespace std;

class Card;
class DiscardPile
{
    private:
        vector<Card *> cards;

    public:
        DiscardPile();
        ~DiscardPile() = default;

        void addCard(Card *card);

        int getSize() const;
        bool isEmpty() const;

        const vector<Card *> &getCards() const;
};

#endif