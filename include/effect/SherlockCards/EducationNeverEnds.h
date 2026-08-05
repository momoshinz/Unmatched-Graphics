#ifndef EDUCATIONNEVERENDS_H
#define EDUCATIONNEVERENDS_H

#include "effect/Effect.h"

class EducationNeverEnds : public Effect
{
public:
    EducationNeverEnds() = default;
    ~EducationNeverEnds() override = default;

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