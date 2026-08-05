#include "fighter/Sisters.h"
#include <iostream>
#include <iomanip>
using namespace std;

Sisters::Sisters(int id)
    : Sidekick("Sister " + to_string(id), 1, 2, AttackType::Melee), id(id)
{
}

bool Sisters::isSister() const
{
     return true;
}

int Sisters::getID() const
{
     return id;
}