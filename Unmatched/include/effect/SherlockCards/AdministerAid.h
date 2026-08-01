#ifndef ADMINISTERAID_H
#define ADMINISTERAID_H

#include "effect/Effect.h"

class AdministerAid : public Effect
{
public:
    AdministerAid() = default;
    ~AdministerAid() override = default;

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