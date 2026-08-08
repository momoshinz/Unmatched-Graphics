#ifndef TURNMANAGER_H
#define TURNMANAGER_H

class Player;

class TurnManager
{
private:
    Player *currentPlayer;
    Player *waitingPlayer;

    int remainingActions;
    int turnNumber;

    bool turnJustStarted;

public:
    TurnManager();
    ~TurnManager() = default;

    void startGame(Player *younger, Player *older);
    void startTurn();
    void endTurn();

    void useAction();
    void addAction();
    void resetActions();
    bool hasActions() const;
    bool isTurnFinished() const;
    bool checkHandLimit() const;
    bool consumeTurnStart();
    bool getTurnJustStarted() const;
    void setTurnJustStarted(bool value);

    Player *getCurrentPlayer() const;
    Player *getWaitingPlayer() const;

    int getRemainingActions() const;
    int getTurnNumber() const;

    void setPlayers(Player *current, Player *waiting);
    void setRemainingActions(int actions);
    void setTurnNumber(int number);
};

#endif