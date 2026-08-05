#ifndef STUDYMETHODS_H
#define STUDYMETHODS_H

#include "effect/Effect.h"

class StudyMethods : public Effect
{
public:
    StudyMethods() = default;
    ~StudyMethods() override = default;

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