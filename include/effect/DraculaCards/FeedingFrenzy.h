#ifndef FEEDINGFRENZY_H
#define FEEDINGFRENZY_H

#include "effect/Effect.h"

class FeedingFrenzy : public Effect
{
public:
    FeedingFrenzy() = default;
    ~FeedingFrenzy() override = default;

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