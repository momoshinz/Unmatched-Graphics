#pragma once
using namespace std;
#include "Hero.h"

class InvisibleMan : public Hero
{
public:
    InvisibleMan();
    virtual void useAbility(Game& game, Player& player) override;
    bool isOnFog() const;
    bool canTeleport() const;
};