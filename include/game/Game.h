#ifndef GAME_H
#define GAME_H
using namespace std;
#include "board/Board.h"
#include "player/Player.h"
#include "CombatSystem.h"
#include "TurnManager.h"
#include "ui/TerminalUI.h"

class Game
{
    private:
        Board board;
        vector<Player *> players;
        CombatSystem combatSystem;
        TurnManager turnManager;
        TerminalUI ui;
        bool isMapSetUp = false;

    public:
        Game();
        ~Game() = default;

        void initialize();
        void addPlayer(Player *player);
        void run();
        void processTurn();
        void processPlayerAction();
        void playSchemeCard();
        void maneuver();
        void attack();
        void discardUntilHandLimit();

        const vector<Player *> &getPlayers() const;

        bool isGameOver() const;
        void endGame();

        Player* selectOpponent(Player &currentPlayer);
        Fighter* selectTarget(Player &currentPlayer, Fighter *user);

        Board &getBoard();
        const Board& getBoard() const;
        CombatSystem &getCombatSystem();
        const CombatSystem& getCombatSystem() const;
        TurnManager &getTurnManager();
        const TurnManager& getTurnManager() const;
        TerminalUI &getUI();
};
#endif