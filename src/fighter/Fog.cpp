#include "fighter/Fog.h"
#include "board/Space.h"
#include <iostream>

using namespace std;

Fog::Fog(int id) : id(id), position(nullptr)
{
}

int Fog::getID() const
{
    return id;
}
Space* Fog::getPosition() const
{
    return position;
}

void Fog::setPosition(Space* space)
{
    position = space;
}