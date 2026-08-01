#ifndef FIGHTER_H
#define FIGHTER_H
#include <string>
#include "board/Board.h"
#include "utils/Types.h"

using namespace std;

class Player;
class Space;

class Fighter
{
    protected:
        string name;
        int health;
        int maxHealth;
        int movement;
        AttackType attackType;
        Player *owner;
        Space *position;
        int tempAttackBoost = 0;
        int OpponentBoostValue = 0;
        bool useOpponentBoostValue;

    public:
        Fighter(const string &name, int maxHealth, int movement, AttackType attackType);
        virtual ~Fighter() = default;
        void takeDamage(int damage);
        void heal(int amount);
        bool isAlive() const;
        void moveTo(Space *newSpace);

        virtual bool isHero() const = 0;
        bool isMelee() const;
        bool isRanged() const;
        int getHealth() const;
        int getMaxHealth() const;
        int getMovement() const;
        AttackType getAttackType() const;
        Player *getOwner() const;
        Space *getPosition() const;
        const string &getName() const;
        bool isAdjacent(const Fighter *other, const Board &board) const;
        bool isInSameZone(const Fighter *other, const Board &board) const;
        virtual bool isAbilityImmune() const { return false; } 
        virtual bool isSister() const {return false;}

        void setOwner(Player *owner);
        void setPosition(Space *position);

        void addTempAttackBoost(int value);
        int getTempAttackBoost() const;
        void resetTempAttackBoost();

        int getOpponentBoostValue() const;
        void setOpponentBoostValue(int value);
        void enableOpponentBoostValue();
        void disableOpponentBoostValue();
        bool shouldUseOpponentBoostValue() const;

};
#endif