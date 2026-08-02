#include "effect/InvisibleManCards/CodedNotes.h"
#include "game/Game.h"
#include "player/Player.h"
#include "card/Hand.h"
#include "card/Deck.h"
#include "card/Card.h"
#include <iostream>
#include <stdexcept>
using namespace std;

void CodedNotes::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    Player* player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

     if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Coded Notes can only be used by Invisible Man!\n");
    }

    cout << "\n========================================\n";
    cout << "-< Look Into My Eyes >- ACTIVATED!\n";

    for(int i=0 ; i<3 ; i++)
    {
        player->drawCardToHand();
    }
    cout << "\n[+] Invisible Man drew 3 cards.\n";

    Hand &hand = player->getHand();
    Deck &deck = player->getDeck();

    hand.display();
    Card* firstCard = nullptr;
    Card* secondCard = nullptr;

    int choice;
    while(true)
    {
        cout << "\n> Choose the first card to place on top of your deck: ";
        cin >> choice;
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        if(choice >=1 && choice <= hand.getSize())
        {
            break;
        }
        cout << "\n[!] ERROR : Invalid choice!\n";
    }
    firstCard = hand.removeCard(choice-1);

    hand.display();
    while(true)
    {
        cout << "\n> Choose the second card to place on top of your deck: ";
        cin >> choice;
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        if(choice >=1 && choice <= hand.getSize())
        {
            break;
        }
        cout << "\n[!] ERROR : Invalid choice!\n";
    }
    secondCard = hand.removeCard(choice-1);

    cout << "\n> Which card should be TOP of the deck?\n";
    cout << "\n1. " << firstCard->getName() << endl;
    cout << "\n2. " << secondCard->getName() << endl;
    
    int order;
    while(true)
    {
        cout << "~~>";
        cin >> order;
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        if(order == 1 || order == 2)
        {
            break;
        }
        cout << "\n[!] ERROR : Invalid choice!\n";
    }
    if(order == 1)
    {
        deck.addToTop(secondCard);
        deck.addToTop(firstCard);
    }
    else
    {
        deck.addToTop(firstCard);
        deck.addToTop(secondCard);
    }
    cout << "\n[+] Cards placed on top of the deck successfully.\n";
    cout << "\n========================================\n";
}

string CodedNotes::getDescription() const
{
    return "> Draw 3 cards. Then place any 2 cards from your hand on top of your deck in any order.";
}

Effect *CodedNotes::clone() const
{
    return new CodedNotes(*this);
}