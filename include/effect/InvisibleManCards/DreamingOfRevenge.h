#ifndef DREAMINGOFREVENGE_H
#define DREAMINGOFREVENGE_H

#include "effect/Effect.h"

class DreamingOfRevenge : public Effect
{
    public:
        DreamingOfRevenge() = default;
        ~DreamingOfRevenge() override = default;

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