#include "effect/DraculaCards/MistForm.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "board/Space.h"
#include "board/Board.h"
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

void MistForm::apply(Game &game, Fighter &fighter, Fighter &target, const Card &self, Card *opponentCard, bool didUserWin)
{
    Player *player = fighter.getOwner();
    if (player == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Fighter has NO owner!\n");
    }
    if (!fighter.isHero())
    {
        throw runtime_error("\n[!] ERROR : Mistform can only be used by Dracula!\n");
    }

    vector<Space*> targetSpaces;
    for(auto space : game.getBoard().getSpaces())
    {
        if(!space->isOccupied())
        {
            targetSpaces.push_back(space);
        }
    }
    if(targetSpaces.empty())
    {
        cout << "\n[!] ERROR : No empty home available.\n";
        return;
    }
    cout << "\n========================================\n";
    cout << "-< MistForm >- ACTIVATED!\n";

    cout << "\n[o] Choose a home to go :\n";
    
    for(int i=0 ; i<targetSpaces.size() ; i++)
    {
        cout << i+1 << ". home " << targetSpaces[i]->getId() << endl;
    }
    cout << "\n> Enter your choice :";
    int choice;
    cin >> choice;
    if(choice < 1 || choice > targetSpaces.size())
    {
        throw invalid_argument("\n[!] ERROR : Invalid choice!\n");
    }

    Space* currentPos = fighter.getPosition();
    Space* chosenSpace = targetSpaces[choice-1];
    if(currentPos != nullptr)
    {
        currentPos->removeFighter(&fighter);
    }
    fighter.setPosition(chosenSpace);
    chosenSpace->setFighter(&fighter);
    cout << "\n[+] " << fighter.getName() << " moved to home " << chosenSpace->getId() << "!\n";

    game.getTurnManager().addAction();
    cout << "[+] " << fighter.getName() << " gain 1 extra action!\n";
    cout << "========================================\n";
}

string MistForm::getDescription() const
{
    return "> Place Dracula in any space.Gain 1 action.";
}

Effect *MistForm::clone() const
{
    return new MistForm(*this);
}