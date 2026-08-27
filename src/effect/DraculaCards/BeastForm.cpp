#include "effect/DraculaCards/Beastform.h"
#include "game/Game.h"
#include "player/Player.h"
#include "card/Card.h"
#include "card/Hand.h"
#include "fighter/Fighter.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>

using namespace std;

void BeastForm::apply(Game &game,
                      Fighter &fighter,
                      Fighter &target,
                      const Card &self,
                      Card *opponentCard,
                      bool didUserWin,
                      const EffectChoice &choice)
{
    Player *player = fighter.getOwner();

    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Beastform can only be used by Dracula!\n");
    }

    Hand &hand = player->getHand();

    vector<int> selectedCards = choice.selectedCardIndices;

    if (selectedCards.empty())
    {
        return;
    }

    for (int index : selectedCards)
    {
        if (index < 0 || index >= hand.getSize())
        {
            throw runtime_error("\n[!] ERROR : Invalid card selection!\n");
        }
    }

    sort(selectedCards.begin(),selectedCards.end(),greater<int>());
    int discarded = 0;

    for (int index : selectedCards)
    {
        Card *discardedCard = hand.removeCard(index);

        if (discardedCard != nullptr)
        {
            player->getDiscardPile().addCard(discardedCard);
            discarded++;
        }
    }

    fighter.addTempAttackBoost(discarded);
}

EffectInputKind BeastForm::getInputKind() const
{
    return EffectInputKind::ChooseCardsToDiscard;
}

string BeastForm::getDescription() const
{
    return "> You may discard any number of cards from your hand.\nThis card's attack value is +1 for each card you discard.";
}

Effect *BeastForm::clone() const
{
    return new BeastForm(*this);
}