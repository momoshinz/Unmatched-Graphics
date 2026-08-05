#ifndef ROLLINGFOG_H
#define ROLLINGFOG_H

#include "effect/Effect.h"

class RollingFog : public Effect
{
public:
    RollingFog() = default;
    ~RollingFog() override = default;

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