#include "fighter/Fighter.h"
#include "fighter/Hero.h"
#include "board/Space.h"
#include "board/Board.h"
#include <iostream>
#include <stdexcept>
using namespace std;

Fighter::Fighter(const string &name, int maxHealth, int movement, AttackType attackType)
    : name(name), health(maxHealth), maxHealth(maxHealth),
      movement(movement), attackType(attackType), owner(nullptr),
      position(nullptr), tempAttackBoost(0), useOpponentBoostValue(false)
{
}

void Fighter::takeDamage(int damage)
{
    if (damage < 0)
    {
        throw invalid_argument("\n[!] ERROR : Damage can't be NEGATIVE!\n");
    }
    health -= damage;
    if (health < 0)
    {
        health = 0;
    }
}

void Fighter::heal(int amount)
{
    if (amount < 0)
    {
        throw invalid_argument("\n[!] ERROR : Heal amount can't be NEGATIVE!\n");
    }
    health += amount;
    if (health > maxHealth)
    {
        health = maxHealth;
    }
}

bool Fighter::isAlive() const
{
    return health > 0;
}

void Fighter::moveTo(Space *newSpace)
{
    if (newSpace == nullptr)
    {
        throw invalid_argument("\n[!] ERROR : Destination home is NULL!\n");
    }

    if (newSpace->isOccupied())
    {
        throw logic_error("\n[!] ERROR : Destination home is already OCCUPIED!\n");
    }

    if (position != nullptr)
    {
        position->removeFighter(this);
    }
    newSpace->setFighter(this);
    position = newSpace;
}

bool Fighter::isMelee() const
{
    return attackType == AttackType::Melee;
}

bool Fighter::isRanged() const
{
    return attackType == AttackType::Ranged;
}

int Fighter::getHealth() const
{
    return health;
}

int Fighter::getMaxHealth() const
{
    return maxHealth;
}

int Fighter::getMovement() const
{
    return movement;
}

AttackType Fighter::getAttackType() const
{
    return attackType;
}

Player *Fighter::getOwner() const
{
    return owner;
}

Space *Fighter::getPosition() const
{
    return position;
}

const string &Fighter::getName() const
{
    return name;
}

bool Fighter::isAdjacent(const Fighter *other, const Board &board) const
{
    if (other == nullptr || position == nullptr || other->position == nullptr)
    {
        return false;
    }
    if (this == other)
    {
        return false;
    }
    int firstId = position->getId();
    int secondId = other->position->getId();
    return board.isNeighbor(firstId, secondId);
}

bool Fighter::isInSameZone(const Fighter *other, const Board &board) const
{
    if (other == nullptr)
        return false;

    if (position == nullptr || other->position == nullptr)
        return false;

    return board.sameZone(position->getId(), other->position->getId());
}

void Fighter::setOwner(Player *owner)
{
    if (owner == nullptr)
    {
        throw invalid_argument("\n[!] ERROR : Fighter cannot be NULL!\n");
    }
    this->owner = owner;
}

void Fighter::setPosition(Space *position)
{
    this->position = position;
}

void Fighter::addTempAttackBoost(int value)
{
    tempAttackBoost += value;
}

int Fighter::getTempAttackBoost() const
{
    return tempAttackBoost;
}

void Fighter::resetTempAttackBoost()
{
    tempAttackBoost = 0;
}

void Fighter::enableOpponentBoostValue()
{
    useOpponentBoostValue = true;
}

void Fighter::disableOpponentBoostValue()
{
    useOpponentBoostValue = false;
}

bool Fighter::shouldUseOpponentBoostValue() const
{
    return useOpponentBoostValue;
}

int Fighter::getOpponentBoostValue() const
{
    return OpponentBoostValue;
}

void Fighter::setOpponentBoostValue(int value)
{
    OpponentBoostValue = value;
}