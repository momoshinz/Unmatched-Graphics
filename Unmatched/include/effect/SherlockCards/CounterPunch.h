#ifndef COUNTERPUNCH_H
#define COUNTERPUNCH_H

#include "effect/Effect.h"

class CounterPunch : public Effect
{
public:
    CounterPunch() = default;
    ~CounterPunch() override = default;

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