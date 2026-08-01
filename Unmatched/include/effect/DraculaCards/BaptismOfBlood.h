#ifndef BAPTISMOFBLOOD_H
#define BAPTISMOFBLOOD_H

#include "effect/Effect.h"

class BaptismOfBlood : public Effect
{
public:
        BaptismOfBlood() = default;
        ~BaptismOfBlood() override = default;

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