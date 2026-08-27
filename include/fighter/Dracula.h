#pragma once
using namespace std;
#include "Hero.h"

class Dracula : public Hero
{
    public:
        Dracula();
        ~Dracula() = default;
        virtual void useAbility(Game& game, Player &player) override;
};