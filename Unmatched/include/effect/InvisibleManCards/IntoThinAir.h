#ifndef INTOTHINAIR_H
#define INTOTHINAIR_H

#include "effect/Effect.h"

class IntoThinAir : public Effect
{
public:
    IntoThinAir() = default;
    ~IntoThinAir() override = default;

    virtual void apply(Game &game,
                       Fighter &fighter,
                       Fighter &target,
                       const Card &self,
                       Card *opponentCard,
                       bool didUserWin) override;

    virtual string getDescription() const override;
    Effect *clone() const override;
};

#endif