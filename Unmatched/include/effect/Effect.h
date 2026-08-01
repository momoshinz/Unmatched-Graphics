#ifndef EFFECT_H
#define EFFECT_H
#include <string>
class Fighter;
class Game;
class Card;

using namespace std;

class Effect
{
public:
    Effect();
    virtual ~Effect() = default;

    virtual void apply(Game &game,
                       Fighter &fighter,
                       Fighter &target,
                       const Card &self,
                       Card *opponentCard,
                       bool didUserWin) = 0;

    virtual string getDescription() const = 0;
    virtual Effect *clone() const = 0;
};

#endif