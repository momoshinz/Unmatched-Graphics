#ifndef AMBUSH_H
#define AMBUSH_H

#include "effect/Effect.h"

class Ambush : public Effect
{
public:
    Ambush() = default;
    ~Ambush() override = default;

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