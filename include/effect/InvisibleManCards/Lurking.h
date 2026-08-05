#ifndef LURKING_H
#define LURKING_H

#include "effect/Effect.h"

class Lurking : public Effect
{
    public:
        Lurking() = default;
        ~Lurking() override = default;

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