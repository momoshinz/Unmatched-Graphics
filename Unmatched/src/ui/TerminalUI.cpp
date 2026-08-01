#include "ui/TerminalUI.h"
#include "ui/MapTemplate.h"
#include "game/Game.h"
#include "board/Board.h"
#include "fighter/Hero.h"
#include "fighter/Dracula.h"
#include "fighter/Sherlock.h"
#include "fighter/Sidekick.h"
#include "fighter/Sisters.h"
#include "player/Player.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>

using namespace std;

string zoneToString(ZoneType zone)
{
    switch (zone)
    {
    case ZoneType::DarkBlue:
        return "Dark Blue";

    case ZoneType::LightBlue:
        return "Light Blue";

    case ZoneType::Green:
        return "Green";

    case ZoneType::Brown:
        return "Brown";

    case ZoneType::Beige:
        return "Beige";

    case ZoneType::Purple:
        return "Purple";

    case ZoneType::Grey:
        return "Grey";
    }

    return "";
}

TerminalUI::TerminalUI()
{
    homePositions.resize(33);

    homePositions[1] = {2, 7};
    homePositions[2] = {2, 17};
    homePositions[3] = {2, 25};
    homePositions[4] = {2, 35};
    homePositions[5] = {2, 47};
    homePositions[6] = {2, 57};
    homePositions[7] = {3, 21};
    homePositions[8] = {4, 5};
    homePositions[9] = {4, 15};
    homePositions[10] = {4, 29};
    homePositions[11] = {4, 42};
    homePositions[12] = {4, 48};
    homePositions[13] = {6, 18};
    homePositions[14] = {7, 5};
    homePositions[15] = {7, 15};
    homePositions[16] = {7, 23};
    homePositions[17] = {7, 35};
    homePositions[18] = {7, 45};
    homePositions[19] = {9, 6};
    homePositions[20] = {9, 13};
    homePositions[21] = {9, 19};
    homePositions[22] = {9, 39};
    homePositions[23] = {9, 44};
    homePositions[24] = {11, 8};
    homePositions[25] = {11, 14};
    homePositions[26] = {11, 21};
    homePositions[27] = {11, 31};
    homePositions[28] = {11, 36};
    homePositions[29] = {11, 43};
    homePositions[30] = {13, 16};
    homePositions[31] = {13, 23};
    homePositions[32] = {13, 39};
}

string TerminalUI::boxLine(const string &text) const
{
    const int INSIDE_WIDTH = 34;

    string line = text;

    if (line.size() > INSIDE_WIDTH)
    {
        line = line.substr(0, INSIDE_WIDTH);
    }

    if (line.size() < INSIDE_WIDTH)
    {
        line += string(INSIDE_WIDTH - line.size(), ' ');
    }

    return "| " + line + " |";
}

string TerminalUI::getAttackTypeString(Fighter *fighter) const
{
    if (fighter == nullptr)
        return "?";

    switch (fighter->getAttackType())
    {
    case AttackType::Melee:
        return "Melee";
    case AttackType::Ranged:
        return "Ranged";
    default:
        return "?";
    }
}

void TerminalUI::renderBoardOnly(const Game &game) const
{
    vector<string> board = buildBoard(game.getBoard(), game.getPlayers());

    cout << "\n------------------------------------------------------------";

    for (const auto &line : board)
    {
        cout << line << '\n';
    }

    cout << "\n------------------------------------------------------------\n";
}

vector<string> TerminalUI::wrapTextForAbility(const string &text, int width) const
{
    vector<string> lines;

    string current;

    stringstream ss(text);

    string word;

    while (ss >> word)
    {
        if (current.empty())
        {
            current = word;
        }
        else if (current.size() + word.size() + 1 <= width)
        {
            current += " " + word;
        }
        else
        {
            lines.push_back(current);
            current = word;
        }
    }

    if (!current.empty())
        lines.push_back(current);

    return lines;
}

void TerminalUI::buildInfoPanel(vector<string> &panel, const Game &game) const
{
    panel.clear();

    const Player *player =
        game.getTurnManager().getCurrentPlayer();

    if (player == nullptr)
        return;

    Hero *hero = player->getHero();

    panel.push_back("+------------------------------------+");
    panel.push_back(boxLine("CURRENT PLAYER"));
    panel.push_back("+------------------------------------+");

    panel.push_back(boxLine("Hero : " + hero->getName()));

    panel.push_back(boxLine("HP : " + to_string(hero->getHealth()) + "/" +
                            to_string(hero->getMaxHealth())));

    panel.push_back(boxLine("Attack Type : " + getAttackTypeString(hero)));

    panel.push_back(boxLine("Movement : " + to_string(hero->getMovement())));

    panel.push_back(boxLine("Actions : " +
                            to_string(game.getTurnManager().getRemainingActions())));

    panel.push_back(boxLine("Cards : " +
                            to_string(player->getHand().getSize())));

    panel.push_back(boxLine(""));

    panel.push_back(boxLine("Special Ability :"));

    auto abilityLines = wrapTextForAbility(hero->getAbilityDescription(), 34);

    for (const auto &line : abilityLines)
    {
        panel.push_back(boxLine(line));
    }

    panel.push_back(boxLine(""));

    panel.push_back(boxLine("SIDEKICKS"));

    const auto &sidekicks = player->getSideKicks();

    if (sidekicks.empty())
    {
        panel.push_back(boxLine("None"));
    }
    else
    {
        for (size_t i = 0; i < sidekicks.size(); i++)
        {
            Sidekick *s = sidekicks[i];

            string line1 = "*" + s->getName();

            if (!s->isAlive())
            {
                line1 += " {DEAD}";
            }
            panel.push_back(boxLine(line1));

            string line2 = "HP : " + to_string(s->getHealth());
            panel.push_back(boxLine(line2));

            string line3 = "Movement : " + to_string(s->getMovement());
            panel.push_back(boxLine(line3));

            string line4 = "Attack Type : " + getAttackTypeString(s);
            panel.push_back(boxLine(line4));
        }
    }

    panel.push_back("+------------------------------------+");
}

void TerminalUI::buildHeroLocationPanel(vector<string> &panel, const Game &game) const
{
    panel.clear();

    Player *player = game.getTurnManager().getCurrentPlayer();

    Hero *hero = player->getHero();

    panel.push_back("+------------------------------------+");
    panel.push_back(boxLine("HERO LOCATION"));
    panel.push_back("+------------------------------------+");

    panel.push_back(boxLine("Name : " + hero->getName()));

    if (hero->getPosition() == nullptr)
    {
        panel.push_back(boxLine("Not on map!"));
        panel.push_back("+------------------------------------+");
        return;
    }

    Space *space = hero->getPosition();

    panel.push_back(boxLine("Home ID : " + to_string(space->getId())));

    for (auto zone : space->getZones())
    {
        panel.push_back(boxLine("Zone : " + zoneToString(zone)));

        string homes = "Same zones ID : ";

        auto ids = game.getBoard().getZoneHomes(zone);

        for (size_t i = 0; i < ids.size(); i++)
        {
            homes += to_string(ids[i]);

            if (i + 1 < ids.size())
                homes += ",";
        }

        panel.push_back(boxLine(homes));
    }

    panel.push_back("+------------------------------------+");
}

void TerminalUI::buildSidekickLocationPanel(vector<string> &panel,
                                            const Game &game) const
{
    panel.clear();

    Player *player = game.getTurnManager().getCurrentPlayer();

    panel.push_back("+------------------------------------+");
    panel.push_back(boxLine("SIDEKICKS LOCATION"));
    panel.push_back("+------------------------------------+");

    for (auto s : player->getSideKicks())
    {
        panel.push_back(boxLine(s->getName()));

        if (!s->isAlive())
        {
            panel.push_back(boxLine("{ Dead }"));
            panel.push_back(boxLine(""));
            continue;
        }

        Space *space = s->getPosition();

        if (space == nullptr)
        {
            panel.push_back(boxLine("Not on map!"));
            panel.push_back(boxLine(""));
            continue;
        }

        panel.push_back(boxLine("Home ID : " + to_string(space->getId())));

        for (auto zone : space->getZones())
        {
            panel.push_back(boxLine("Zone : " + zoneToString(zone)));

            string homes = "Same zones ID : ";

            auto ids = game.getBoard().getZoneHomes(zone);

            for (size_t i = 0; i < ids.size(); i++)
            {
                homes += to_string(ids[i]);

                if (i + 1 < ids.size())
                    homes += ",";
            }

            panel.push_back(boxLine(homes));
        }

        panel.push_back(boxLine(""));
    }

    panel.push_back("+------------------------------------+");
}

void TerminalUI::renderScreen(const Game &game) const
{

    vector<string> board = buildBoard(game.getBoard(), game.getPlayers());

    vector<string> info;
    buildInfoPanel(info, game);

    const int PANEL_WIDTH = 38;

    int rows = max(info.size(), board.size());

    for (int i = 0; i < rows; i++)
    {
        if (i < info.size())
            cout << left << setw(PANEL_WIDTH) << info[i];
        else
            cout << string(PANEL_WIDTH, ' ');

        if (i < board.size())
            cout << board[i];

        cout << '\n';
    }

    cout << "========================================================================\n";

    vector<string> heroPanel;
    vector<string> sidePanel;

    buildHeroLocationPanel(heroPanel, game);
    buildSidekickLocationPanel(sidePanel, game);

    int rows2 = max(heroPanel.size(), sidePanel.size());

    for (int i = 0; i < rows2; i++)
    {
        if (i < heroPanel.size())
            cout << left << setw(38) << heroPanel[i];
        else
            cout << string(38, ' ');

        if (i < sidePanel.size())
            cout << sidePanel[i];

        cout << '\n';
    }
}

void TerminalUI::drawStaticMap(vector<string> &canvas) const
{
    canvas = MAP_TEMPLATE;
}

void TerminalUI::drawHomes(vector<string> &canvas) const
{
    for (int i = 1; i <= 32; i++)
    {
        int row = homePositions[i].row;
        int col = homePositions[i].col;

        string id = to_string(i);

        canvas[row][col] = id[0];

        if (id.size() == 2)
            canvas[row][col + 1] = id[1];
    }
}

void TerminalUI::showBoard(const Board &board, const vector<Player *> &players) const
{
}

vector<string> TerminalUI::buildBoard(const Board &board, const vector<Player *> &players) const
{
    vector<string> canvas = MAP_TEMPLATE;

    drawHomes(canvas);

    drawFighters(canvas, players);

    return canvas;
}

void TerminalUI::drawFighters(vector<string> &canvas, const vector<Player *> &players) const
{
    for (Player *player : players)
    {
        Hero *hero = player->getHero();

        if (hero != nullptr && hero->isAlive())
        {
            if (hero->getPosition() == nullptr)
                continue;

            int homeId = hero->getPosition()->getId();

            CellPosition pos = homePositions[homeId];

            string symbol = getSymbol(hero);

            for (int i = 0; i < symbol.size(); i++)
            {
                canvas[pos.row][pos.col + i] = symbol[i];
            }
        }

        for (Sidekick *sidekick : player->getSideKicks())
        {
            if (sidekick == nullptr || !sidekick->isAlive())
                continue;

            if (sidekick->getPosition() == nullptr)
                continue;

            int homeId = sidekick->getPosition()->getId();

            CellPosition pos = homePositions[homeId];

            string symbol = getSymbol(sidekick);

            for (int i = 0; i < symbol.size(); i++)
            {
                canvas[pos.row][pos.col + i] = symbol[i];
            }
        }
    }
}

string TerminalUI::getSymbol(Fighter *fighter) const
{
    if (dynamic_cast<Sherlock *>(fighter))
        return "Sh";

    if (dynamic_cast<Watson *>(fighter))
        return "ّW ";

    if (dynamic_cast<Dracula *>(fighter))
        return "D ";

    Sisters *sister = dynamic_cast<Sisters *>(fighter);
    if (sister != nullptr)
    {
        return "S" + to_string(sister->getID());
    }
    return "?";
}

void TerminalUI::waitForEnter() const
{
    cout << "\nPress ENTER to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void TerminalUI::clearScreen() const
{
    system("cls");
}

int TerminalUI::chooseCard(Player &player) const
{
    player.getHand().display();
    int choice;
    cout << "\nChoose card: ";
    cin >> choice;
    return choice;
}

int TerminalUI::chooseFighter(const vector<Fighter *> &fighters) const
{
    for (int i = 0; i < fighters.size(); i++)
    {
        cout << i + 1 << ". " << fighters[i]->getName() << endl;
    }
    int choice;
    cout << "\n~~> ";
    cin >> choice;
    return choice;
}

int TerminalUI::chooseSpace() const
{
    int id;
    cout << "\nEnter Space ID : ";
    cin >> id;
    return id;
}

void TerminalUI::showWinner(const Player &winner) const
{
    string text = "~~~> " + winner.getHero()->getName() + " <~~~";

    int width = 42;
    int left = (width - text.length()) / 2;
    int right = width - text.length() - left;

    cout << "\n __________________________________________";
    cout << "\n|                GAME OVER                 |";
    cout << "\n ------------------------------------------";
    cout << "\n| * o * . * O * . * o * . * O * . * o * O  |";
    cout << "\n|" << string(left, ' ')
         << text
         << string(right, ' ') << "|";
    cout << "\n|             IS THE WINNER !              |";
    cout << "\n ------------------------------------------\n";
}

int TerminalUI::chooseIndexOnly() const
{
    int choice;

    cout << "\nChoose card : ";
    cin >> choice;

    return choice;
}