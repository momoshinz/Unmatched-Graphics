#include "effect/DraculaCards/MistForm.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "board/Space.h"
#include "board/Board.h"
#include <iostream>
#include <stdexcept>

using namespace std;

void MistForm::apply(Game &game,
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
        throw runtime_error("\n[!] ERROR : Mistform can only be used by Dracula!\n");
    }

    if (choice.selectedSpace == nullptr)
    {
        throw runtime_error("\n[!] ERROR : No destination selected for MistForm!\n");
    }

    Space *chosenSpace = choice.selectedSpace;

    if (chosenSpace->isOccupied())
    {
        throw runtime_error("\n[!] ERROR : Selected home is already OCCUPIED!\n");
    }

    Space *currentSpace = fighter.getPosition();

    if (currentSpace != nullptr)
    {
        currentSpace->removeFighter(&fighter);
    }

    fighter.setPosition(chosenSpace);
    chosenSpace->setFighter(&fighter);

    game.getTurnManager().addAction();
}


EffectInputKind MistForm::getInputKind() const
{
    return EffectInputKind::ChooseAnyEmptySpace;
}

string MistForm::getDescription() const
{
    return "> Place Dracula in any empty space. Gain 1 action.";
}


Effect *MistForm::clone() const
{
    return new MistForm(*this);
}