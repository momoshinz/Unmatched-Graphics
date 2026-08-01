#ifndef IMPOSSIBLETOSEE_H
#define IMPOSSIBLETOSEE_H

#include "effect/Effect.h"

class ImpossibleToSee : public Effect
{
public:
    ImpossibleToSee() = default;
    ~ImpossibleToSee() override = default;

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