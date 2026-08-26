#include "effect/InvisibleManCards/Lurking.h"
#include "game/Game.h"
#include "fighter/InvisibleMan.h"
#include "fighter/Fog.h"
#include "player/Player.h"
#include "board/Board.h"
#include "board/Space.h"
#include <stdexcept>
using namespace std;

void Lurking::apply(Game &game, Fighter &fighter, Fighter &target,
                    const Card &self, Card *opponentCard, bool didUserWin,
                    const EffectChoice &choice)
{
    Player *player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Lurking can only be used by Invisible Man!\n");
    }

    Board &board = game.getBoard();

    player->drawCardToHand();

    if (choice.selectedOptionIndex == 1)
    {
        if (choice.selectedSpace == nullptr)
        {
            throw runtime_error("\n[!] ERROR : No destination selected!\n");
        }
        board.moveFighter(&fighter, choice.selectedSpace);
    }
    else if (choice.selectedOptionIndex == 2)
    {
        vector<Fog *> fogs = player->getFogs();

        if (choice.selectedFogId < 0 || choice.selectedFogId >= static_cast<int>(fogs.size()))
        {
            throw runtime_error("\n[!] ERROR : Invalid Fog selection!\n");
        }

        Fog *fog = fogs[choice.selectedFogId];

        if (choice.secondSpace == nullptr)
        {
            throw runtime_error("\n[!] ERROR : No Fog destination selected!\n");
        }

        if (fog->getPosition() != nullptr)
        {
            fog->getPosition()->setFogToken(false);
        }
        fog->setPosition(choice.secondSpace);
        choice.secondSpace->setFogToken(true);
    }
    else
    {
        throw runtime_error("\n[!] ERROR : Invalid option selected!\n");
    }
}

EffectInputKind Lurking::getInputKind() const
{
    return EffectInputKind::ChooseLurkingOption;
}

int Lurking::getFogMoveRange() const
{
    return 3;
}

string Lurking::getDescription() const
{
    return "> Draw 1 card. Choose one effect: Move Invisible Man to a home with a Fog token, or move one Fog token up to 3 homes.";
}

Effect *Lurking::clone() const
{
    return new Lurking(*this);
}