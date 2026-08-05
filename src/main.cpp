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
                game.run(false);
                break;
            }

            case 1:
            {
                Game game;
                if(game.loadMenu())
                {
                    game.run(true);
                }
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