#ifndef DASH_H
#define DASH_H

#include "effect/Effect.h"

class Dash : public Effect
{
public:
    Dash() = default;
    ~Dash() override = default;

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