#include "effect/SherlockCards/EliminateTheImpossible.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "card/Card.h"
#include <iostream>
#include <stdexcept>

using namespace std;

void EliminateTheImpossible::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Eliminate The Impossible can only be used by Sherlock Holmes!\n");
    }

    Player *currentPlayer = fighter.getOwner();

    if (currentPlayer == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    Player *opponent = game.selectOpponent(*currentPlayer);

    if (opponent == nullptr)
    {
        throw runtime_error("\n[!] ERROR : NO opponent selected!\n");
    }

    Hand &opponentHand = opponent->getHand();

    if (opponentHand.isEmpty())
    {
        cout << "\n[!] Opponent has NO cards in hand.\n";
        return;
    }

    cout << "\n========================================\n";
    cout << "-< Eliminate The Impossible >- ACTIVATED!\n";

    cout << "\nOpponent's hand :\n";
    opponentHand.display();

    cout << "\nChoose a card to burn : ";

    int choice;
    cin >> choice;

    if (choice < 1 || choice > opponentHand.getSize())
    {
        throw out_of_range("\n[!] ERROR : Invalid card!\n");
    }

    Card *burnedCard = opponentHand.removeCard(choice - 1);

    cout << "\n[-] " << burnedCard->getName() << " GOT BURNED!\n";

    delete burnedCard;

    cout << "[+] Card removed successfully.\n";
    cout << "========================================\n";
}

string EliminateTheImpossible::getDescription() const
{
    return "> Look at your opponent's hand, choose 1 card, and burn it.";
}

Effect *EliminateTheImpossible::clone() const
{
    return new EliminateTheImpossible(*this);
}