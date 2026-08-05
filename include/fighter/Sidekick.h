#ifndef SIDEKICK_H
#define SIDEKICK_H

#include "Fighter.h"

class Sidekick : public Fighter
{
    public:
        Sidekick(const string& name, int maxHealth, int movement, AttackType attackType);

        ~Sidekick() = default;

        virtual bool isSister() const;
        virtual bool isAbilityImmune() const override;
        virtual bool isHero() const override;
};

#endif