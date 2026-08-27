#pragma once
#include "board/Board.h"
#include "player/Player.h"
#include "CombatSystem.h"
#include "TurnManager.h"
#include "ui/TerminalUI.h"
#include <string>

class Game
{
private:
    Board board;
    vector<Player *> players;
    Player *youngerPlayer;
    Player *olderPlayer;
    CombatSystem combatSystem;
    TurnManager turnManager;
    TerminalUI ui;
    bool isMapSetUp = false;
    bool feintBlockedFlag = false; 
    std::string feintBlockedMessage;

public:
    Game();
    ~Game() = default;

    void initialize(int age1, int age2);
    void addPlayer(Player *player);
    void processTurn();
    void processPlayerAction();
    void playSchemeCard();
    void maneuver();
    void attack();
    void discardUntilHandLimit();

    const vector<Player *> &getPlayers() const;

    bool isGameOver() const;
    void endGame();

    Player *selectOpponent(Player &currentPlayer);
    Fighter *selectTarget(Player &currentPlayer, Fighter *user);

    Board &getBoard();
    const Board &getBoard() const;
    CombatSystem &getCombatSystem();
    const CombatSystem &getCombatSystem() const;
    TurnManager &getTurnManager();
    const TurnManager &getTurnManager() const;
    TerminalUI &getUI();

    void saveGame(const string &filename);
    void loadGame(const string &filename);
    void saveMenu();
    bool loadMenu();

    Player *getYoungerPlayer() const;
    Player *getOlderPlayer() const;

    bool assignHero(int playerIndex, const std::string &heroName);
    void startGame();
    void beginTurns();

    void setFeintBlocked(const std::string &message);
    bool consumeFeintBlockedFlag(std::string &outMessage);

<<<<<<< HEAD
    void resetGame(); // to start a new game after exiting one
};
=======
    void resetGame(); 
};
#endif
>>>>>>> a9073d1d4f411fb1f12484d5667471d831251d21
