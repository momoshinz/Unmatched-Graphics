#ifndef SLIPAWAY_H
#define SLIPAWAY_H

#include "effect/Effect.h"

class SlipAway : public Effect
{
public:
    SlipAway() = default;
    ~SlipAway() override = default;

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