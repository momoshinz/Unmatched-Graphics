#ifndef CODEDNOTES_H
#define CODEDNOTES_H

#include "effect/Effect.h"

class CodedNotes : public Effect
{
public:
    CodedNotes() = default;
    ~CodedNotes() override = default;

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