#ifndef PREYUPON_H
#define PREYUPON_H

#include "effect/Effect.h"

class PreyUpon : public Effect
{
public:
    PreyUpon() = default;
    ~PreyUpon() override = default;

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