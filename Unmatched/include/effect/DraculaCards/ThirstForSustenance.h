#ifndef THIRSTFORSUSTENANCE_H
#define THIRSTFORSUSTENANCE_H
using namespace std;
#include "effect/Effect.h"

class ThirstForSustenance : public Effect
{
public:
    ThirstForSustenance() = default;
    ~ThirstForSustenance() override = default;

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