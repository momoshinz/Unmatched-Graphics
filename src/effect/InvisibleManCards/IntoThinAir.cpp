#include "effect/InvisibleManCards/IntoThinAir.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "fighter/Fog.h"
#include "board/Board.h"
#include "board/Space.h"
#include <iostream>
#include <stdexcept>
using namespace std;

void IntoThinAir::apply(Game &game, Fighter &fighter, Fighter &target,
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
        throw runtime_error("\n[!] ERROR : Into Thin Air can only be used by Invisible Man!\n");
    }

    cout << "\n========================================\n";
    cout << "-< Into Thin Air >- ACTIVATED!\n";

    // نکته: جابه‌جایی Invisible Man از قبل توسط EffectUI انجام شده
    // (برای بازخورد بصری آنی هنگام کلیک روی نقشه)

    vector<Fog *> fogs = player->getFogs();
    if (fogs.empty())
    {
        return;
    }

    if (choice.selectedFogId < 0 || choice.selectedFogId >= static_cast<int>(fogs.size()))
    {
        throw out_of_range("\n[!] ERROR : Invalid Fog selection!\n");
    }

    Fog *fog = fogs[choice.selectedFogId];

    if (choice.secondSpace == nullptr)
    {
        cout << "\n[!] No Fog destination selected.\n";
        return;
    }

    if (fog->getPosition() != nullptr)
    {
        fog->getPosition()->setFogToken(false);
    }
    fog->setPosition(choice.secondSpace);
    choice.secondSpace->setFogToken(true);

    cout << "\n[+] Fog moved successfully.\n";
    cout << "\n========================================\n";
}

EffectInputKind IntoThinAir::getInputKind() const
{
    return EffectInputKind::ChooseFighterMoveThenFogMove;
}

int IntoThinAir::getMoveRange() const
{
    return 1;
}

int IntoThinAir::getFogMoveRange() const
{
    return 3;
}

string IntoThinAir::getDescription() const
{
    return "> Move Invisible Man up to 1 Home. Then move one Fog token up to 3 Homes.";
}

Effect *IntoThinAir::clone() const
{
    return new IntoThinAir(*this);
}