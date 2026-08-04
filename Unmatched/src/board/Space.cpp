#include "board/Space.h"
#include "fighter/Fighter.h"
#include <iostream>
#include <stdexcept>
using namespace std;

Space::Space(int id) : id(id), fighter(nullptr), secretPassage(false), fogToken(false)
{
}

int Space::getId() const
{
    return id;
}

Fighter *Space::getFighter() const
{
    return fighter;
}

bool Space::isOccupied() const
{
    return fighter != nullptr;
}

void Space::setFighter(Fighter *fighter)
{
    if (fighter == nullptr)
    {
        throw invalid_argument("\n[!] ERROR : NO fighter selected. Cannot place in home!\n");
    }
    if (this->fighter != nullptr)
    {
        throw logic_error("\n[!] ERROR : This home is already OCCUPIED!\n");
    }
    this->fighter = fighter;
}

void Space::removeFighter(Fighter *fighter)
{
    if (fighter == nullptr)
    {
        throw invalid_argument("\n[!] ERROR : NO fighter selected. Cannot remove from home!\n");
    }
    if (this->fighter == nullptr)
    {
        throw logic_error("\n[!] ERROR : There is NO fighter in this home!\n");
    }
    if (this->fighter != fighter)
    {
        throw logic_error("\n[!] ERROR : The fighter is NOT in this home.\n");
    }
    this->fighter = nullptr;
}

void Space::addNeighbor(Space *neighbor)
{
    if (neighbor == nullptr)
    {
        throw invalid_argument("\n[!] ERROR : Neighbor cannot be NULL!\n");
    }

    if (neighbor == this)
    {
        throw logic_error("\n[!] ERROR : A home cannot be its own neighbor!\n");
    }

    for (Space *existing : neighbors)
    {
        if (existing == neighbor)
        {
            return;
        }
    }

    neighbors.push_back(neighbor);
}

const vector<Space *> &Space::getNeighbors() const
{
    return neighbors;
}

void Space::addZone(ZoneType zone)
{
    for (ZoneType existing : zones)
    {
        if (existing == zone)
        {
            return;
        }
    }
    zones.push_back(zone);
}

const vector<ZoneType> &Space::getZones() const
{
    return zones;
}

bool Space::hasSecretPassage() const
{
    return secretPassage;
}

void Space::setSecretPassage(bool value)
{
    secretPassage = value;
}

bool Space::hasFogToken() const
{
    return fogToken;
}

void Space::setFogToken(bool value)
{
    fogToken = value;
}