#ifndef THEGAMEISAFOOT_H
#define THEGAMEISAFOOT_H

#include "effect/Effect.h"

class TheGameIsAfoot : public Effect
{
public:
    TheGameIsAfoot() = default;
    ~TheGameIsAfoot() override = default;

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