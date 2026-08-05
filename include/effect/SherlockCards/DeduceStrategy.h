#ifndef DEDUCESTRATEGY_H
#define DEDUCESTRATEGY_H

#include "effect/Effect.h"

class DeduceStrategy : public Effect
{
public:
    DeduceStrategy() = default;
    ~DeduceStrategy() override = default;

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