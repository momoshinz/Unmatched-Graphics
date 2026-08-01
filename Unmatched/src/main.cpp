#include <iostream>
#include "game/Game.h"
#include "ui/MainMenu.h"
#include <limits>
using namespace std;
MainMenu menu;
int main()
{
    while (true)
    {
        int choice = menu.show();

        switch (choice)
        {
        case 0:
        {
            Game game;
            game.run();
            break;
        }

        case 1:
        {
            system("cls");

            cout << "\n==============================| INSTRUCTIONS |=================================\n";
            cout << "Don't You Really Know How To Play :o ? Then Read Carefully ..\n\n";
            cout << "[ ACTIONS ]\n";
            cout << "Attack   ~>  Attack an enemy fighter.\n";
            cout << "Maneuver ~>  Move a fighter and draw a card.\n";
            cout << "Scheme   ~>  Play a Scheme card.\n";
            cout << "\n[ RULES ]\n";
            cout << "[o] Each turn you have 2 actions.\n";
            cout << "[o] Every Hero card carries a unique effect,\n    and on top of that each Hero possesses a special ability of their own.\n";
            cout << "[o] Your hand must never exceed 7 cards.\n";
            cout << "[o] Heroes enter the battlefield alongside their Sidekicks, NEVER ALONE.\n";
            cout << "[o] Any home marked with a '*' conceals a Secret Passage,\n    allowing instant teleportation between them.\n";
            cout << "[o] Through tactical moves in each turn, your goal is clear:\n    defeat the enemy Hero and claim VICTORY.\n";
            cout << "[o] The younger player chooses the fighter and steps onto the field first.\n";
            cout << "\nGood Luck, Fighter. You'll need it :]\n";
            cout << "==========================================================================\n";

            cout << "\nPress ENTER to return...";
            cin.get();

            system("cls");

            break;
        }

        case 2:
        {
            cout << "\nBye! But Hope You'll Change Your Mind Later :<\n";
            return 0;
        }
        }
    }
}