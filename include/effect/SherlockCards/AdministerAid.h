#pragma once
#include "effect/Effect.h"

class AdministerAid : public Effect
{
public:
    void apply(Game &game, Fighter &fighter, Fighter &target,
               const Card &self, Card *opponentCard, bool didUserWin,
               const EffectChoice &choice) override;

    EffectInputKind getInputKind() const override;
    string getDescription() const override;
    Effect *clone() const override;
};