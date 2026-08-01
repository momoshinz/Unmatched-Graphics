#ifndef RAVENINGSEDUCTION_H
#define RAVENINGSEDUCTION_H
using namespace std;
#include "effect/Effect.h"

class RaveningSeduction : public Effect
{
public:
    RaveningSeduction() = default;
    ~RaveningSeduction() override = default;

    void apply(Game &game,
               Fighter &fighter,
               Fighter &target,
               const Card &self,
               Card *opponentCard,
               bool didUserWin) override;

    virtual string getDescription() const override;
    Effect *clone() const override;
};

#endif