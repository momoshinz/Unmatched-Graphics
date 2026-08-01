#include "effect/DraculaCards/Ambush.h"
#include "game/Game.h"
#include "fighter/Fighter.h"
#include "player/Player.h"
#include "card/Card.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

void Ambush::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    Player* opponentPlayer = target.getOwner();
    if(opponentPlayer == nullptr)
    {
        cerr << "\n[!] ERROR : Target has NO owner!\n";
        return;
    }

    Hand &opponentHand = opponentPlayer->getHand();
    if(opponentHand.isEmpty())
    {
        cerr << "\n[!] ERROR : Opponent player has NO cards in hand!\nAmbush effect is CANCELED.\n";
        return;
    }

    cout << "\n========================================";
    cout << "\n-< Ambush >- ACTIVATED!\n";

    srand(time(0));
    int randomCard = rand() % opponentHand.getSize();
    Card* discardedCard = opponentHand.removeCard(randomCard);

    opponentPlayer->getDiscardPile().addCard((discardedCard));
    cout << endl;
    cout << "[o] " << target.getName() << " discarded : " << discardedCard->getName() << endl;

    int boostValue = discardedCard->getBoost();
    fighter.addTempAttackBoost(boostValue);
    cout << "[+] Ambush gains +" << boostValue << " attack from discarded card's boost!\n";
    cout << "========================================\n";
}

string Ambush::getDescription() const
{
    return "\n> Your opponent discards 1 random card.add it's BOOST value to this card's ATTACK POWER!";
}

Effect* Ambush::clone() const
{
    return new Ambush(*this);
}