#include "board/Board.h"
#include "fighter/Fighter.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <queue>
#include <set>

using namespace std;

Board::Board()
{
}

Board::~Board()
{
    for (Space *space : spaces)
    {
        delete space;
    }
    spaces.clear();
}

const vector<Space *> &Board::getSpaces() const
{
    return spaces;
}

void Board::addSpace(Space *space)
{
    if (space == nullptr)
    {
        throw invalid_argument("\n[!] ERROR : Cannot add a null home to the map!\n");
    }

    if (getSpace(space->getId()) != nullptr)
    {
        throw logic_error("\n[!] ERROR : A home with this ID already exists!\n");
    }

    spaces.push_back(space);
}

Space *Board::getSpace(int id) const
{
    for (Space *space : spaces)
    {
        if (space->getId() == id)
        {
            return space;
        }
    }

    return nullptr;
}

bool Board::isValid(int id) const
{
    return getSpace(id) != nullptr;
}

vector<Space *> Board::findSpacesByZone(ZoneType zone) const
{
    vector<Space *> result;

    for (Space *space : spaces)
    {
        for (ZoneType currentZone : space->getZones())
        {
            if (currentZone == zone)
            {
                result.push_back(space);
                break;
            }
        }
    }

    return result;
}

vector<Space *> Board::findEmptySpaces() const
{
    vector<Space *> result;

    for (Space *space : spaces)
    {
        if (!space->isOccupied())
        {
            result.push_back(space);
        }
    }

    return result;
}

bool Board::isNeighbor(int first, int second) const
{
    Space *firstSpace = getSpace(first);
    Space *secondSpace = getSpace(second);

    if (firstSpace == nullptr || secondSpace == nullptr)
    {
        return false;
    }

    for (Space *neighbor : firstSpace->getNeighbors())
    {
        if (neighbor == secondSpace)
        {
            return true;
        }
    }

    return false;
}

bool Board::sameZone(int first, int second) const
{
    Space *firstSpace = getSpace(first);
    Space *secondSpace = getSpace(second);

    if (firstSpace == nullptr || secondSpace == nullptr)
    {
        return false;
    }

    for (ZoneType zone1 : firstSpace->getZones())
    {
        for (ZoneType zone2 : secondSpace->getZones())
        {
            if (zone1 == zone2)
            {
                return true;
            }
        }
    }

    return false;
}

void Board::connectSpaces(int firstId, int secondId)
{
    Space *first = getSpace(firstId);
    Space *second = getSpace(secondId);

    if (first == nullptr || second == nullptr)
    {
        throw invalid_argument("\n[!] ERROR : Invalid home ID!\n");
    }

    first->addNeighbor(second);
    second->addNeighbor(first);
}

vector<Space *> Board::getAdjacentSpaces(Space *space) const
{
    if (space == nullptr)
        return {};

    return space->getNeighbors();
}

void Board::setupMap()
{
    for (int i = 1; i <= 32; i++)
    {
        addSpace(new Space(i));
    }

    connectSpaces(1, 2);
    connectSpaces(1, 8);
    connectSpaces(2, 7);
    connectSpaces(3, 7);
    connectSpaces(3, 10);
    connectSpaces(3, 4);
    connectSpaces(4, 10);
    connectSpaces(4, 11);
    connectSpaces(5, 11);
    connectSpaces(5, 12);
    connectSpaces(5, 6);
    connectSpaces(8, 9);
    connectSpaces(8, 14);
    connectSpaces(9, 13);
    connectSpaces(7, 13);
    connectSpaces(10, 11);
    connectSpaces(11, 12);
    connectSpaces(11, 17);
    connectSpaces(12, 18);
    connectSpaces(13, 15);
    connectSpaces(14, 15);
    connectSpaces(14, 19);
    connectSpaces(16, 17);
    connectSpaces(16, 21);
    connectSpaces(17, 18);
    connectSpaces(17, 22);
    connectSpaces(17, 27);
    connectSpaces(17, 28);
    connectSpaces(17, 31);
    connectSpaces(18, 23);
    connectSpaces(19, 20);
    connectSpaces(19, 24);
    connectSpaces(20, 21);
    connectSpaces(21, 26);
    connectSpaces(22, 23);
    connectSpaces(23, 29);
    connectSpaces(24, 25);
    connectSpaces(25, 30);
    connectSpaces(26, 27);
    connectSpaces(26, 30);
    connectSpaces(26, 31);
    connectSpaces(27, 28);
    connectSpaces(27, 31);
    connectSpaces(28, 29);
    connectSpaces(28, 32);
    connectSpaces(29, 32);
    connectSpaces(30, 31);

    getSpace(1)->addZone(ZoneType::LightBlue);
    getSpace(2)->addZone(ZoneType::LightBlue);
    getSpace(3)->addZone(ZoneType::Brown);
    getSpace(4)->addZone(ZoneType::Brown);
    getSpace(5)->addZone(ZoneType::Beige);
    getSpace(6)->addZone(ZoneType::Beige);
    getSpace(7)->addZone(ZoneType::Brown);
    getSpace(7)->addZone(ZoneType::LightBlue);
    getSpace(8)->addZone(ZoneType::LightBlue);
    getSpace(9)->addZone(ZoneType::LightBlue);
    getSpace(10)->addZone(ZoneType::Brown);
    getSpace(11)->addZone(ZoneType::Brown);
    getSpace(11)->addZone(ZoneType::Beige);
    getSpace(11)->addZone(ZoneType::Green);
    getSpace(12)->addZone(ZoneType::Beige);
    getSpace(13)->addZone(ZoneType::LightBlue);
    getSpace(13)->addZone(ZoneType::DarkBlue);
    getSpace(14)->addZone(ZoneType::DarkBlue);
    getSpace(15)->addZone(ZoneType::DarkBlue);
    getSpace(16)->addZone(ZoneType::Green);
    getSpace(21)->addZone(ZoneType::Green);
    getSpace(20)->addZone(ZoneType::Green);
    getSpace(20)->addZone(ZoneType::DarkBlue);
    getSpace(19)->addZone(ZoneType::Grey);
    getSpace(19)->addZone(ZoneType::DarkBlue);
    getSpace(24)->addZone(ZoneType::Grey);
    getSpace(25)->addZone(ZoneType::Grey);
    getSpace(30)->addZone(ZoneType::Grey);
    getSpace(31)->addZone(ZoneType::Grey);
    getSpace(26)->addZone(ZoneType::Grey);
    getSpace(26)->addZone(ZoneType::Green);
    getSpace(27)->addZone(ZoneType::Grey);
    getSpace(28)->addZone(ZoneType::Grey);
    getSpace(32)->addZone(ZoneType::Grey);
    getSpace(29)->addZone(ZoneType::Purple);
    getSpace(29)->addZone(ZoneType::Grey);
    getSpace(23)->addZone(ZoneType::Purple);
    getSpace(22)->addZone(ZoneType::Purple);
    getSpace(18)->addZone(ZoneType::Purple);
    getSpace(17)->addZone(ZoneType::Green);
    getSpace(17)->addZone(ZoneType::Purple);

    getSpace(1)->setSecretPassage(true);
    getSpace(6)->setSecretPassage(true);
    getSpace(16)->setSecretPassage(true);
    getSpace(24)->setSecretPassage(true);
}

vector<Space *> Board::getAvailableMoves(Fighter *fighter, int maxStep) const
{
    vector<Space *> result;
    if (fighter == nullptr)
    {
        return result;
    }
    Space *start = fighter->getPosition();
    if (start == nullptr)
    {
        return result;
    }
    queue<pair<Space *, int>> q;
    set<Space *> visited;

    q.push({start, 0});
    visited.insert(start);

    while (!q.empty())
    {
        Space *current = q.front().first;
        int distance = q.front().second;
        q.pop();

        if (distance == maxStep)
        {
            continue;
        }
        vector<Space *> nextSpaces = current->getNeighbors();

        if (current->hasSecretPassage())
        {
            for (auto space : spaces)
            {
                if (space != current && space->hasSecretPassage())
                {
                    nextSpaces.push_back(space);
                }
            }
        }
        for (auto next : nextSpaces)
        {
            if (visited.count(next))
            {
                continue;
            }
            visited.insert(next);

            Fighter *user = next->getFighter();
            if (user == nullptr)
            {
                result.push_back(next);
                q.push({next, distance + 1});
                continue;
            }
            if (user->getOwner() == fighter->getOwner())
            {
                q.push({next, distance + 1});
                continue;
            }
        }
    }
    return result;
}

vector<pair<Space *, int>> Board::getAvailableMovesWithDistance(Fighter *fighter, int maxStep) const
{
    vector<pair<Space *, int>> result;
    if (fighter == nullptr)
    {
        return result;
    }
    Space *start = fighter->getPosition();
    if (start == nullptr)
    {
        return result;
    }
    queue<pair<Space *, int>> q;
    set<Space *> visited;

    q.push({start, 0});
    visited.insert(start);

    while (!q.empty())
    {
        Space *current = q.front().first;
        int distance = q.front().second;
        q.pop();

        if (distance == maxStep)
        {
            continue;
        }
        vector<Space *> nextSpaces = current->getNeighbors();

        if (current->hasSecretPassage())
        {
            for (auto space : spaces)
            {
                if (space != current && space->hasSecretPassage())
                {
                    nextSpaces.push_back(space);
                }
            }
        }
        for (auto next : nextSpaces)
        {
            if (visited.count(next))
            {
                continue;
            }
            visited.insert(next);

            Fighter *user = next->getFighter();
            if (user == nullptr)
            {
                result.push_back({next, distance + 1});
                q.push({next, distance + 1});
                continue;
            }
            if (user->getOwner() == fighter->getOwner())
            {
                q.push({next, distance + 1});
                continue;
            }
        }
    }
    return result;
}

bool Board::moveFighter(Fighter *fighter, Space *destination)
{
    if (fighter == nullptr || destination == nullptr)
        return false;

    if (destination->isOccupied())
        return false;

    Space *current = fighter->getPosition();

    if (current != nullptr)
    {
        current->removeFighter(fighter);
    }

    destination->setFighter(fighter);
    fighter->setPosition(destination);

    return true;
}

bool Board::swapFighters(Fighter *first,
                         Fighter *second)
{
    if (first == nullptr || second == nullptr)
        return false;

    Space *firstSpace = first->getPosition();
    Space *secondSpace = second->getPosition();

    if (firstSpace == nullptr || secondSpace == nullptr)
        return false;

    firstSpace->removeFighter(first);
    secondSpace->removeFighter(second);

    firstSpace->setFighter(second);
    secondSpace->setFighter(first);

    first->setPosition(secondSpace);
    second->setPosition(firstSpace);

    return true;
}

vector<int> Board::getZoneHomes(ZoneType zone) const
{
    vector<int> homes;

    for (auto space : spaces)
    {
        for (auto z : space->getZones())
        {
            if (z == zone)
            {
                homes.push_back(space->getId());
                break;
            }
        }
    }

    return homes;
}