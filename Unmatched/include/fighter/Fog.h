#ifndef FOG_H
#define FOG_H
#include "Sidekick.h"

class Fog
{
    private:
        int id;
        Space* position;

    public:
        Fog(int id);
        ~Fog() = default;
        int getID() const;
        Space* getPosition() const;
        void setPosition(Space* space);
};

#endif