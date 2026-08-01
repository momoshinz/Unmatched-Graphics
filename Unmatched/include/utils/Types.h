#ifndef TYPES_H
#define TYPES_H
using namespace std;

enum class AttackType
{
    Melee,
    Ranged
};

enum class CardType
{
    Attack,
    Defense,
    Versatile,
    Scheme
};

enum class Timing
{
    None,
    Immediately,
    DuringCombat,
    AfterCombat
};

enum class OwnerType
{
    Hero,
    Sidekick,
    Any
};

enum class ZoneType
{
    DarkBlue,
    LightBlue,
    Green,
    Grey,
    Brown,
    Beige,
    Purple
};

#endif