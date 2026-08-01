#ifndef FEINTDRACULA_H
#define FEINTDRACULA_H

#include "effect/Effect.h"

class FeintDracula : public Effect
{
public:
    FeintDracula() = default;
    ~FeintDracula() override = default;

    void apply(Game &game,
               Fighter &fighter,
               Fighter &target,
               const Card &self,
               Card *opponentCard,
               bool didUserWin) override;

    virtual string getDescription() const override;
    virtual Effect *clone() const override;
};

#endif