#pragma once
#include "effect/Effect.h"

class StudyMethods : public Effect
{
public:
    void apply(Game &game, Fighter &fighter, Fighter &target,
               const Card &self, Card *opponentCard, bool didUserWin,
               const EffectChoice &choice) override;

    EffectInputKind getInputKind() const override;
    bool shouldRequestInput(Game &game, Fighter &user, Fighter &target, bool didUserWin) const override;

    string getDescription() const override;
    Effect *clone() const override;
};