#ifndef STEPLIGHTLY_H
#define STEPLIGHTLY_H

#include "effect/Effect.h"

class StepLightly : public Effect
{
    public:
        StepLightly() = default;
        ~StepLightly() override = default;

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