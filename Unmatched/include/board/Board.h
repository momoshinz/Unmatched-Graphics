#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include "Space.h"
using namespace std;

class Board
{
private:
    vector<Space *> spaces;

public:
    Board();
    ~Board();

    const vector<Space *> &getSpaces() const;

    vector<Space *> findSpacesByZone(ZoneType zone) const;
    vector<Space *> findEmptySpaces() const;

    void addSpace(Space *space);
    void setupMap();
    void connectSpaces(int firstId, int secondId);
    Space *getSpace(int id) const;
    bool isValid(int id) const;
    bool isNeighbor(int first, int second) const;
    bool sameZone(int first, int second) const;
    vector<Space *> getAdjacentSpaces(Space *space) const;
    vector<int> getZoneHomes(ZoneType zone) const;

    vector<Space *> getAvailableMoves(Fighter *fighter, int maxStep) const;
    vector<pair<Space *, int>> getAvailableMovesWithDistance(Fighter *fighter, int maxStep) const;
    bool moveFighter(Fighter *fighter, Space *destination);
    bool swapFighters(Fighter *first, Fighter *second);
};
#endif