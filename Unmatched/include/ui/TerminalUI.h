#ifndef TERMINALUI_H
#define TERMINALUI_H
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
    void drawStaticMap(vector<string> &canvas) const;
    void drawHomes(vector<string> &canvas) const;
    void drawFighters(vector<string> &canvas, const vector<Player *> &players) const;
    string getColor(Fighter *fighter) const;
    string getSymbol(Fighter *fighter) const;

public:
    TerminalUI();
    ~TerminalUI() = default;

    void showBoard(const Board &board, const vector<Player *> &players) const;
    vector<string> buildBoard(const Board &board, const vector<Player *> &players) const;
    void buildInfoPanel(vector<string> &panel, const Game &game) const;
    void buildHeroLocationPanel(vector<string> &panel, const Game &game) const;
    void buildSidekickLocationPanel(vector<string> &panel, const Game &game) const;
    void renderBoardOnly(const Game& game) const;
    vector <string> wrapTextForAbility(const string &text, int width) const;
  
    void renderScreen(const Game &game) const;
    string boxLine(const string &text) const;
   
    string getAttackTypeString(Fighter *fighter) const;

    int chooseIndexOnly() const;
    int chooseCard(Player &player) const;
    int chooseFighter(const vector<Fighter *> &fighters) const;
    int chooseSpace() const;
    void waitForEnter() const;
    void clearScreen() const;

    void showCombatResult(const Fighter &attacker,
                          const Fighter &defender,
                          int damage) const;

    void showWinner(const Player &winner) const;
};

#endif