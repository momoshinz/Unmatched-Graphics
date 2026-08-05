#include "effect/DraculaCards/Beastform.h"
#include "game/Game.h"
#include "player/Player.h"
#include "card/Card.h"
#include "fighter/Fighter.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
using namespace std;

void BeastForm::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    Player* player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Beastform can only be used by Dracula!\n");
    }

    Hand &hand = player->getHand();
    if(hand.isEmpty())
    {
        cout << "\n[!] ERROR : Your hand is EMPTY. NO card to discard!\n";
        return;
    }
    cout << "\n========================================";
    cout << "\n-< BeastForm >- ACTIVATED!\n";

    cout << "\nYour hand :\n";
    hand.display();

    cout << "\n========================================";
    cout << "\n[o] Choose a card to discard :\n";
    cout << "========================================\n";
    for(int i=0 ; i<hand.getSize() ; i++)
    {
        cout << "> " << i+1 << ". " << hand.getCard(i)->getName() << endl;
    }
    cout << "\n> Enter number { 0 = finish } :\n";
    
    vector<int> selectedCards;
    vector<bool> isSelected(hand.getSize(), false);
    while(true)
    {
        cout << "~~> ";
        int choice;
        cin >> choice;
        if(choice == 0)
        {
            break;
        }
        if(choice < 1 || choice > hand.getSize())
        {
            cout << "\n[!] ERROR : Invalid number! try again.\n";
            cout << "\n> Enter number { 0 = finish } :\n";
            continue;
        }
        if(isSelected[choice-1])
        {
            cout << "\n[!] Card already selected. choose another one.\n";
            cout << "\n> Enter number { 0 = finish } :\n";
            continue;;
        }
        isSelected[choice-1] = true;
        selectedCards.push_back(choice-1);
        cout << "\n[+] Selected : " << hand.getCard(choice-1)->getName() << endl;
        cout << "\n> Enter number { 0 = finish } :\n";
    }
    if(selectedCards.empty())
    {
        cout << "\n[!] No cards discarded.\n";
        return;
    }

    cout << "\n========================================";
    cout << "\n[o] Selected cards to discard :\n";
    cout << "========================================\n";
    for(int index : selectedCards)
    {
        cout << "\n[-] " << hand.getCard(index)->getName() << endl;
    }
    
    sort(selectedCards.begin(), selectedCards.end(), greater<int>());
    int discarded = 0;
    for(int index : selectedCards)
    {
        Card* discardedOnes = hand.removeCard(index);
        if(discardedOnes != nullptr)
        {
            player->getDiscardPile().addCard(discardedOnes);
            discarded++;
        }
    }

    fighter.addTempAttackBoost(discarded);
    cout << "\n[+] " << discarded << " card(s) discarded.\n";
    cout << "\n[+] Attack value increased by " << discarded << ".\n";
    cout << "========================================\n";
}

string BeastForm::getDescription() const
{
    return "> You may discard any number of cards from your hand.This card's attack value is +1 for each card you discard.";
}

Effect* BeastForm::clone() const
{
    return new BeastForm(*this);
}