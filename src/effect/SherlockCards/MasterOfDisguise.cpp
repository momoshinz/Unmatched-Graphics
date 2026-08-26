#include "effect/SherlockCards/MasterOfDisguise.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Hero.h"
#include "fighter/Fighter.h"
#include "board/Board.h"
#include <stdexcept>

using namespace std;

void MasterOfDisguise::apply(Game &game, Fighter &fighter, Fighter &target,
                             const Card &self, Card *opponentCard, bool didUserWin,
                             const EffectChoice &choice)
{
    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Master Of Disguise can only be used by Sherlock Holmes!\n");
    }

    Player *myPlayer = fighter.getOwner();
    if (myPlayer == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }

    if (choice.selectedFighter == nullptr)
    {
        throw runtime_error("\n[!] ERROR : No opponent fighter selected!\n");
    }
    Fighter *selectedEnemy = choice.selectedFighter;

    Player *enemyOwner = selectedEnemy->getOwner();
    if (enemyOwner == nullptr)
    {
        throw runtime_error("Selected fighter has no owner! :<");
    }

    Hero *enemyHero = enemyOwner->getHero();
    game.getBoard().swapFighters(&fighter, enemyHero);
    enemyHero->takeDamage(1);
}

EffectInputKind MasterOfDisguise::getInputKind() const
{
    return EffectInputKind::ChooseEnemyFighter;
}

string MasterOfDisguise::getDescription() const
{
    return "> Choose an opponent. Swap Holmes with that opponent's hero and deal 1 damage to that hero.";
}

Effect *MasterOfDisguise::clone() const
{
    return new MasterOfDisguise(*this);
}