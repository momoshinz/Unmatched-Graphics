#ifndef FIXEDPOINT_H
#define FIXEDPOINT_H

#include "effect/Effect.h"

class FixedPoint : public Effect
{
public:
    FixedPoint() = default;
    ~FixedPoint() override = default;

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