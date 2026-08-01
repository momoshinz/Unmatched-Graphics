#ifndef EMERGEFROMMIST_H
#define EMERGEFROMMIST_H

#include "effect/Effect.h"

class EmergeFromMist : public Effect
{
public:
    EmergeFromMist() = default;
    ~EmergeFromMist() override = default;

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