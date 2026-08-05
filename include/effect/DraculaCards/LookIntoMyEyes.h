#ifndef LOOKINTOMYEYES_H
#define LOOKINTOMYEYES_H

#include "effect/Effect.h"

class LookIntoMyEyes : public Effect
{
public:
    LookIntoMyEyes() = default;
    ~LookIntoMyEyes() override = default;

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