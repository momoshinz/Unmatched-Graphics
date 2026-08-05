#ifndef SHERLOCK_H
#define SHERLOCK_H

#include "Hero.h"
#include "game/Game.h"

class SideKick;

class Sherlock : public Hero
{
    public:
        Sherlock();
        ~Sherlock() = default;

        virtual void useAbility(Game& game, Player &player) override;
        virtual bool isAbilityImmune() const override;
};

#endif