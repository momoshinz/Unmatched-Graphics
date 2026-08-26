#include "effect/InvisibleManCards/StepLightly.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "fighter/Fog.h"
#include "board/Board.h"
#include "board/Space.h"
#include <stdexcept>

using namespace std;

void StepLightly::apply(Game &game, Fighter &fighter, Fighter &target,
                        const Card &self, Card *opponentCard, bool didUserWin,
                        const EffectChoice &choice)
{
    Player* player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    if (choice.selectedFighter != nullptr)
    {
        int damage = 1;
        if (fighter.getPosition() != nullptr && fighter.getPosition()->hasFogToken())
        {
            damage = 3;
        }

        choice.selectedFighter->takeDamage(damage);
    }

    vector<Fog *> fogs = player->getFogs();
    if (fogs.empty())
    {
        return;
    }

    if (choice.selectedFogId < 0 || choice.selectedFogId >= static_cast<int>(fogs.size()))
    {
        return;
    }

    Fog *fog = fogs[choice.selectedFogId];

    if (choice.secondSpace == nullptr)
    {
        return;
    }

    if (fog->getPosition() != nullptr)
    {
        fog->getPosition()->setFogToken(false);
    }

    fog->setPosition(choice.secondSpace);
    choice.secondSpace->setFogToken(true);
}

EffectInputKind StepLightly::getInputKind() const
{
    return EffectInputKind::ChooseEnemyAndFogDestination;
}

int StepLightly::getFogMoveRange() const
{
    return 2;
}

string StepLightly::getDescription() const
{
    return "> Deal 1 damage to an adjacent fighter (3 instead if Invisible Man is on a Fog). Then your opponent moves one Fog token up to 2 homes.";
}

Effect *StepLightly::clone() const
{
    return new StepLightly(*this);
}