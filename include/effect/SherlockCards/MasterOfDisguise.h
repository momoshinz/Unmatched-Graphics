#ifndef MASTEROFDISGUISE_H
#define MASTEROFDISGUISE_H

#include "effect/Effect.h"

class MasterOfDisguise : public Effect
{
public:
    MasterOfDisguise() = default;
    ~MasterOfDisguise() override = default;

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