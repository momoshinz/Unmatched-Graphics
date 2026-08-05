#ifndef FEINTSHERLOCK_H
#define FEINTSHERLOCK_H

#include "effect/Effect.h"

class FeintSherlock : public Effect
{
public:
    FeintSherlock() = default;
    ~FeintSherlock() override = default;

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