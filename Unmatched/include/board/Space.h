#ifndef SPACE_H
#define SPACE_H

#include <vector>
#include "utils/Types.h"

class Fighter;

class Space
{
    private:
        int id;
        Fighter *fighter;
        vector<Space *> neighbors;
        vector<ZoneType> zones;
        bool secretPassage;

    public:
        explicit Space(int id);
        ~Space() = default;

        int getId() const;

        Fighter *getFighter() const;
        bool isOccupied() const;

        void setFighter(Fighter *fighter);
        void removeFighter(Fighter *fighter);

        void addNeighbor(Space *neighbor);
        const vector<Space *> &getNeighbors() const;

        void addZone(ZoneType zone);
        const vector<ZoneType> &getZones() const;

        bool hasSecretPassage() const;
        void setSecretPassage(bool value);

        void display() const;
};

#endif