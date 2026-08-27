#pragma once
#include <string>
#include <vector>
using namespace std;
class Player;
class Fighter;
class Card;
class Board;
class Game;

struct CellPosition
{
    int row;
    int col;
};

class TerminalUI
{
private:
    static constexpr int INFO_WIDTH = 38;
    vector<CellPosition> homePositions;

public:
    TerminalUI();
    ~TerminalUI() = default;
   
    string getAttackTypeString(Fighter *fighter) const;
};