#ifndef INVISIBLEMAN_H
#define INVISIBLEMAN_H
using namespace std;
#include "Hero.h"

class invisibleMan : public Hero
{
    public:
        invisibleMan();
        ~invisibleMan() = default;
        virtual void useAbility(Game& game, Player &player) override;
};
#endif