#ifndef MISTFORM_H
#define MISTFORM_H

#include "effect/Effect.h"

class MistForm : public Effect
{
public:
    MistForm() = default;
    ~MistForm() override = default;

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