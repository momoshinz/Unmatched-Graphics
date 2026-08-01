#ifndef INVISIBLEMAN_H
#define INVISIBLEMAN_H
using namespace std;
#include "Hero.h"

class InvisibleMan : public Hero
{
    public:
        InvisibleMan();
        ~InvisibleMan() = default;
        virtual void useAbility(Game& game, Player &player) override;
};
#endif