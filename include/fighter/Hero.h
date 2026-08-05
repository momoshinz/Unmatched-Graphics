#ifndef HERO_H
#define HERO_H

#include "Fighter.h"
#include <string>
#include <game/Game.h>

using namespace std;

class Hero : public Fighter
{
    protected:
        string abilityDescription;

    public:
        Hero(
            const string &name,
            int maxHealth,
            int movement,
            AttackType attackType,
            const string &abilityDescription);

        virtual ~Hero() = default;

        const string &getAbilityDescription() const;
        virtual void useAbility(Game& game, Player &player);
        virtual bool isAbilityImmune() const override; 
        virtual bool isHero() const override;
};

#endif