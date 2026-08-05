#ifndef REIGNOFTERROR_H
#define REIGNOFTERROR_H

#include "effect/Effect.h"

class ReignOfTerror : public Effect
{
public:
    ReignOfTerror() = default;
    ~ReignOfTerror() override = default;

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