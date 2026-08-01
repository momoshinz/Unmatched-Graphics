#include "game/TurnManager.h"
#include "player/Player.h"
#include "fighter/Hero.h"
#include "ui/MapTemplate.h"
#include <stdexcept>
#include <iostream>
using namespace std;

TurnManager::TurnManager()
    : currentPlayer(nullptr),
      waitingPlayer(nullptr),
      remainingActions(0),
      turnNumber(0)
{
}

void TurnManager::startGame(Player *younger,
                            Player *older)
{
    if (younger == nullptr || older == nullptr)
    {
        throw runtime_error("\n[!] ERROR : NO player! Cannot start the game.\n");
    }

    currentPlayer = younger;
    waitingPlayer = older;

    turnNumber = 1;

    startTurn();
}

void TurnManager::startTurn()
{
    remainingActions = 2;
}

void TurnManager::useAction()
{
    if (remainingActions <= 0)
    {
        throw runtime_error("\n[!] ERROR : NO action remaining!\n");
    }

    remainingActions--;
}

void TurnManager::addAction()
{
    remainingActions++;
}

void TurnManager::resetActions()
{
    remainingActions = 2;
}

bool TurnManager::hasActions() const
{
    return remainingActions > 0;
}

bool TurnManager::isTurnFinished() const
{
    return remainingActions == 0;
}

void TurnManager::endTurn()
{
    Player *temp = currentPlayer;
    currentPlayer = waitingPlayer;
    waitingPlayer = temp;
    turnNumber++;

    startTurn();
}

Player *TurnManager::getCurrentPlayer() const
{
    return currentPlayer;
}

Player *TurnManager::getWaitingPlayer() const
{
    return waitingPlayer;
}

int TurnManager::getRemainingActions() const
{
    return remainingActions;
}

int TurnManager::getTurnNumber() const
{
    return turnNumber;
}

bool TurnManager::checkHandLimit() const
{
    if (currentPlayer == nullptr)
    {
        return false;
    }

    return currentPlayer->getHand().getSize() > 7;
}