#include "game/Game.h"
#include "fighter/Dracula.h"
#include "fighter/Sherlock.h"
#include "fighter/Sisters.h"
#include "fighter/DrWatson.h"
#include "fighter/Sidekick.h"
#include "card/Card.h"
#include <iostream>
#include <stdexcept>
#include <unordered_map>
using namespace std;

Game::Game()
{
}

void Game::addPlayer(Player *player)
{
    if (player == nullptr)
    {
        throw invalid_argument("\n[!] ERROR : Player is NULL!\n");
    }
    if (players.size() >= 2)
    {
        throw runtime_error("\n[!] ERROR : Only 2 players are allowed!\n");
    }
    players.push_back(player);
}

void Game::initialize()
{
    int age1, age2;
    while (true)
    {
        cout << "~> Enter age of Player 1 : ";
        cin >> age1;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Plaese enter a number!\n";
            continue;
        }

        cout << "~> Enter age of Player 2 : ";
        cin >> age2;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Plaese enter a number!\n";
            continue;
        }

        if (age1 < 0 || age2 < 0)
        {
            cout << "\nAge, not temperature :> ..\n\n";
            continue;
        }

        if (age1 > 100 || age2 > 100)
        {
            cout << "\nLooks like we have a real Dracula? ..\n\n";
            continue;
        }

        break;
    }

    Player *player1 = new Player(age1);
    Player *player2 = new Player(age2);

    addPlayer(player1);
    addPlayer(player2);

    board.setupMap();

    Player *younger;
    Player *older;

    if (player1->isYounger(*player2))
    {
        younger = player1;
        older = player2;
    }
    else
    {
        younger = player2;
        older = player1;
    }
    cout << "\n** Younger player what will you choose ?\n";
    cout << "1. Dracula\n";
    cout << "2. Sherlock\n~~> ";

    int heroChoice;
    cin >> heroChoice;

    while (heroChoice != 1 && heroChoice != 2)
    {
        cout << "[!] Choose 1 or 2 : ";
        cin >> heroChoice;
    }

    int heroSpace;
    ui.renderBoardOnly(*this);
    cout << "\n> Choose starting home { 7 or 22 } : ";
    cin >> heroSpace;

    while (heroSpace != 7 && heroSpace != 22)
    {
        cin >> heroSpace;
    }

    if (heroChoice == 1)
    {
        younger->setHero(new Dracula());

        younger->addSideKick(new Sisters(1));
        younger->addSideKick(new Sisters(2));
        younger->addSideKick(new Sisters(3));

        older->setHero(new Sherlock());

        older->addSideKick(new Watson());
    }
    else
    {
        younger->setHero(new Sherlock());

        younger->addSideKick(new Watson());

        older->setHero(new Dracula());

        older->addSideKick(new Sisters(1));
        older->addSideKick(new Sisters(2));
        older->addSideKick(new Sisters(3));
    }

    board.moveFighter(younger->getHero(), board.getSpace(heroSpace));

    int otherHeroSpace = (heroSpace == 7 ? 22 : 7);

    board.moveFighter(older->getHero(), board.getSpace(otherHeroSpace));

    vector<ZoneType> heroZones = board.getSpace(heroSpace)->getZones();

    for (Sidekick *sidekick : younger->getSideKicks())
    {
        while (true)
        {
            ui.renderBoardOnly(*this);
            vector<Space *> validSpaces;

            cout << "\nPlace " << sidekick->getName() << endl;
            cout << "Available homes :\n";
            cout << "======================================\n";

            for (Space *space : board.getSpaces())
            {
                if (space->isOccupied())
                {
                    continue;
                }

                bool valid = false;
                for (ZoneType heroZone : heroZones)
                {
                    for (ZoneType zone : space->getZones())
                    {
                        if (heroZone == zone)
                        {
                            valid = true;
                            break;
                        }
                    }

                    if (valid)
                    {
                        break;
                    }
                }
                if (valid)
                {
                    validSpaces.push_back(space);
                    cout << validSpaces.size() << ". home " << space->getId() << endl;
                }
            }
            cout << "\n> Choose a home : ";
            int choice;
            cin >> choice;

            if (choice < 1 || choice > validSpaces.size())
            {
                cout << "\n[!] Invalid choice! :(\n";
                continue;
            }

            board.moveFighter(sidekick, validSpaces[choice - 1]);
            break;
        }
    }

    vector<ZoneType> opponentHeroZones = board.getSpace(otherHeroSpace)->getZones();
    for (Sidekick *sidekick : older->getSideKicks())
    {
        while (true)
        {

            ui.renderBoardOnly(*this);
            vector<Space *> validSpaces;

            cout << "\nPlace " << sidekick->getName() << endl;
            cout << "Available homes :\n";
            cout << "======================================\n";

            for (Space *space : board.getSpaces())
            {
                if (space->isOccupied())
                    continue;

                bool valid = false;

                for (ZoneType heroZone : opponentHeroZones)
                {
                    for (ZoneType zone : space->getZones())
                    {
                        if (heroZone == zone)
                        {
                            valid = true;
                            break;
                        }
                    }

                    if (valid)
                        break;
                }

                if (valid)
                {
                    validSpaces.push_back(space);

                    cout << validSpaces.size()
                         << ". Home "
                         << space->getId()
                         << endl;
                }
            }

            cout << "\n> Choose a home : ";

            int choice;
            cin >> choice;

            if (choice < 1 || choice > validSpaces.size())
            {
                cout << "\n[!] Invalid choice!\n";
                continue;
            }

            board.moveFighter(sidekick, validSpaces[choice - 1]);

            break;
        }
    }

    cout << "\n==========<  Drawing Cards To Hands  >==========\n";

    Player *draculaPlayer = nullptr;
    Player *sherlockPlayer = nullptr;

    for (Player *player : players)
    {
        if (dynamic_cast<Dracula *>(player->getHero()) != nullptr)
        {
            draculaPlayer = player;
        }
        else
        {
            sherlockPlayer = player;
        }
    }
    if (draculaPlayer != nullptr)
    {
        Deck::DraculaDeck(draculaPlayer->getDeck());
    }
    if (sherlockPlayer != nullptr)
    {
        Deck::SherlockDeck(sherlockPlayer->getDeck());
    }

    for (Player *player : players)
    {
        cout << "\n"
             << player->getHero()->getName()
             << " draws 5 cards...\n";

        for (int i = 0; i < 5; i++)
        {
            player->drawCardToHand();
        }

        cout << "[+] Done.\n";
    }

    turnManager.startGame(younger, older);

    cout << "\n===============< ** GAME STARTED ** >===============\n";
}

const vector<Player *> &Game::getPlayers() const
{
    return players;
}

Board &Game::getBoard()
{
    return board;
}

CombatSystem &Game::getCombatSystem()
{
    return combatSystem;
}

TurnManager &Game::getTurnManager()
{
    return turnManager;
}

TerminalUI &Game::getUI()
{
    return ui;
}

Player *Game::selectOpponent(Player &currentPlayer)
{
    vector<Player *> opponents;

    for (Player *player : players)
    {
        if (player != nullptr &&
            player != &currentPlayer)
        {
            opponents.push_back(player);
        }
    }

    if (opponents.empty())
    {
        throw runtime_error("\n[!] ERROR : NO opponent found! :<\n");
    }

    if (opponents.size() == 1)
    {
        return opponents[0];
    }

    cout << "\n========== Choose Opponent ==========\n";

    for (size_t i = 0; i < opponents.size(); i++)
    {
        cout << i + 1
             << ". "
             << opponents[i]->getHero()->getName()
             << endl;
    }

    int choice;

    cin >> choice;

    if (choice < 1 || choice > opponents.size())
    {
        throw out_of_range("\n[!] ERROR : Invalid opponent!\n");
    }

    return opponents[choice - 1];
}

Fighter *Game::selectTarget(Player &currentPlayer, Fighter *user)
{
    if (user == nullptr)
    {
        throw invalid_argument("\n[!] ERROR : Fighter is NULL!\n");
    }

    Player *opponent = selectOpponent(currentPlayer);

    if (opponent == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Opponent NOT found!\n");
    }

    vector<Fighter *> targets;

    Hero *hero = opponent->getHero();

    if (hero != nullptr && hero->isAlive() && combatSystem.isInAttackRange(user, hero))
    {
        targets.push_back(hero);
    }

    for (Sidekick *sidekick : opponent->getSideKicks())
    {
        if (sidekick != nullptr && sidekick->isAlive() &&
            combatSystem.isInAttackRange(user, sidekick))
        {
            targets.push_back(sidekick);
        }
    }

    if (targets.empty())
    {
        cout << "\n[!] NO valid target found.\n";
        return nullptr;
    }

    cout << "\n==========( Choose Target )==========\n";

    for (size_t i = 0; i < targets.size(); i++)
    {
        cout << i + 1
             << ". "
             << targets[i]->getName();

        if (targets[i]->isHero())
        {
            cout << " { Hero }";
        }
        else
        {
            cout << " { Sidekick }";
        }

        cout << endl;
    }

    int choice;
    cout << "~~> ";

    cin >> choice;

    if (choice < 1 || choice > targets.size())
    {
        throw out_of_range("\n[!] ERROR : Invalid target!\n");
    }

    return targets[choice - 1];
}

void Game::processPlayerAction()
{
    Player *currentPlayer = turnManager.getCurrentPlayer();

    if (currentPlayer == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Current player not found!\n");
    }

    cout << "\nCurrent Player : "
         << currentPlayer->getHero()->getName()
         << endl;

    cout << "\n[o] Choose an action :\n";
    cout << "1. Maneuver\n";
    cout << "2. Attack\n";
    cout << "3. Play Scheme\n ~~> ";

    int choice;
    cin >> choice;
    while (choice < 1 || choice > 3)
    {
        cout << "\n[!] ERROR : Invalid choice! :( Try again.\n";
        cout << "\n ~~> ";
        cin >> choice;
    }

    switch (choice)
    {
    case 1:
        maneuver();
        break;

    case 2:
        attack();
        break;

    case 3:
        playSchemeCard();
        break;
    }
}

void Game::processTurn()
{
    try
    {
        combatSystem.setGame(this);
        Player *currentPlayer = turnManager.getCurrentPlayer();

        if (currentPlayer == nullptr)
        {
            throw runtime_error("\n[!] ERROR : Current player not found!\n");
        }

        ui.renderScreen(*this);

        cout << "\nTURN : " << currentPlayer->getHero()->getName() << endl;

        Hero *hero = currentPlayer->getHero();
        if (dynamic_cast<Dracula *>(hero) != nullptr)
        {
            cout << "\n[*] Dracula may use his special ability.\n";

            try
            {
                hero->useAbility(*this, *currentPlayer);
            }
            catch (const exception &e)
            {
                cerr << "\n[!] Error using Dracula's ability : " << e.what() << endl;
            }
        }

        turnManager.resetActions();

        while (turnManager.hasActions())
        {
            try
            {
                processPlayerAction();
            }
            catch (const exception &e)
            {
                cerr << "\n[!] Error in action : " << e.what() << endl;
                cout << "\nPlease try again.\n";
            }
        }
        try
        {
            discardUntilHandLimit();
        }
        catch (const exception &e)
        {
            cerr << "\n[!] Error discarding cards : " << e.what() << endl;
        }

        turnManager.endTurn();
    }
    catch (const exception &e)
    {
        cerr << "\n[!] Error in processTurn: " << e.what() << endl;
        throw;
    }
}

void Game::maneuver()
{
    Player *currentPlayer = turnManager.getCurrentPlayer();

    if (currentPlayer == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Current player not found!\n");
    }

    vector<Fighter *> fighters;

    if (currentPlayer->getHero() != nullptr && currentPlayer->getHero()->isAlive())
    {
        fighters.push_back(currentPlayer->getHero());
    }

    for (Sidekick *sidekick : currentPlayer->getSideKicks())
    {
        if (sidekick != nullptr && sidekick->isAlive())
        {
            fighters.push_back(sidekick);
        }
    }

    if (fighters.empty())
    {
        throw runtime_error("\n[!] ERROR : No fighter available! :<\n");
    }

    currentPlayer->drawCardToHand();
    cout << "\n[+] Drew one card.\n";

    unordered_map<Fighter *, int> movedDistance;

    for (Fighter *fighter : fighters)
    {
        movedDistance[fighter] = 0;
    }

    while (true)
    {
        cout << "\n==========< Choose Fighter To Move >==========\n\n";

        for (int i = 0; i < fighters.size(); i++)
        {
            cout << i + 1 << ". " << fighters[i]->getName();

            if (movedDistance[fighters[i]] >= fighters[i]->getMovement())
                cout << " { Movement Finished }";

            cout << endl;
        }

        cout << "0. Finish Maneuver\n";

        int fighterChoice;
        cout << "~~> ";
        cin >> fighterChoice;

        if (fighterChoice == 0)
            break;

        if (fighterChoice < 1 || fighterChoice > fighters.size())
        {
            cout << "\n[!] Invalid choice! :<\n";
            continue;
        }

        Fighter *fighter = fighters[fighterChoice - 1];

        if (movedDistance[fighter] >= fighter->getMovement())
        {
            cout << "\n[!] "
                 << fighter->getName()
                 << " has already used ALL of its movement in this maneuver.. :)\n";
            continue;
        }

        int answer = 0;

        while (answer < 1 || answer > 2)
        {
            cout << "\n[?] Move "
                 << fighter->getName()
                 << " ?\n";

            cout << "1. Yes\n";
            cout << "2. No\n";
            cout << "~~> ";

            cin >> answer;
        }

        if (answer == 2)
            continue;

        int remainingMovement = fighter->getMovement() - movedDistance[fighter];

        int totalMovement = remainingMovement;

        if (!currentPlayer->getHand().isEmpty())
        {
            int boostAnswer = 0;

            while (boostAnswer < 1 || boostAnswer > 2)
            {
                cout << "\n[?] Use BOOST card?\n";
                cout << "1. Yes\n";
                cout << "2. No\n";
                cout << "~~> ";

                cin >> boostAnswer;
            }

            if (boostAnswer == 1)
            {
                cout << "\n========== HAND ==========\n";
                currentPlayer->getHand().display();

                int cardIndex = 0;

                while (cardIndex < 1 || cardIndex > currentPlayer->getHand().getSize())
                {
                    cout << "> Choose a card to burn : ";
                    cin >> cardIndex;
                }

                Card *discardedCard = currentPlayer->getHand().removeCard(cardIndex - 1);

                if (discardedCard != nullptr)
                {
                    totalMovement += discardedCard->getBoost();

                    cout << "\n[+] BOOST = "
                         << discardedCard->getBoost()
                         << endl;

                    currentPlayer->getDiscardPile().addCard(discardedCard);
                }
            }
        }

        auto availableMoves = board.getAvailableMovesWithDistance(fighter, totalMovement);

        if (availableMoves.empty())
        {
            cout << "\n[!] No available movement. :<\n";
            continue;
        }

        cout << "\n========== Available Homes ==========\n\n";

        for (int i = 0; i < availableMoves.size(); i++)
        {
            cout << i + 1
                 << ". Home "
                 << availableMoves[i].first->getId()
                 << " { "
                 << availableMoves[i].second
                 << " step";

            if (availableMoves[i].second > 1)
                cout << "s";

            cout << " }\n";
        }

        int choice = 0;

        while (choice < 1 || choice > availableMoves.size())
        {
            choice = ui.chooseSpace();

            if (choice < 1 || choice > availableMoves.size())
            {
                cout << "\n[!] Invalid choice! :<\n";
            }
        }

        board.moveFighter(fighter, availableMoves[choice - 1].first);

        movedDistance[fighter] += availableMoves[choice - 1].second;

        ui.renderScreen(*this);

        cout << "\n[+] "
             << fighter->getName()
             << " moved successfully.\n";

        if (fighter->getMovement() - movedDistance[fighter] < 0)
        {
            cout << "[*] Remaining movement : 0" << endl;
        }

        else
        {
            cout << "[*] Remaining movement : "
                 << fighter->getMovement() - movedDistance[fighter]
                 << endl;
        }
    }

    turnManager.useAction();
}

void Game::playSchemeCard()
{
    Player *currentPlayer = turnManager.getCurrentPlayer();

    if (currentPlayer == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Current player not found!\n");
    }

    vector<Fighter *> fighters;

    if (currentPlayer->getHero() != nullptr && currentPlayer->getHero()->isAlive())
    {
        fighters.push_back(currentPlayer->getHero());
    }

    for (Sidekick *sidekick : currentPlayer->getSideKicks())
    {
        if (sidekick != nullptr && sidekick->isAlive())
        {
            fighters.push_back(sidekick);
        }
    }

    if (fighters.empty())
    {
        throw runtime_error("\n[!] ERROR : No fighter available!\n");
    }

    int fighterChoice = 0;
    while (true)
    {
        cout << "\n========== Choose Fighter ==========\n";
        for (int i = 0; i < fighters.size(); i++)
        {
            cout << i + 1 << ". " << fighters[i]->getName() << endl;
        }
        cout << "~~>  ";
        cin >> fighterChoice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "[!] Please enter a number!\n";
            continue;
        }

        if (fighterChoice >= 1 && fighterChoice <= fighters.size())
        {
            break;
        }
        cout << "[!] Invalid choice! :( Try again.\n";
    }

    Fighter *fighter = fighters[fighterChoice - 1];

    Hand &hand = currentPlayer->getHand();

    while (true)
    {
        vector<int> playableSchemes;

        cout << "\n========== Playable Scheme Cards ==========\n";

        int displayIndex = 1;

        for (int i = 0; i < hand.getSize(); i++)
        {
            Card *card = hand.getCard(i);

            if (card == nullptr)
                continue;

            if (!card->isScheme())
                continue;

            if (!card->isPlayableBy(*fighter))
                continue;

            playableSchemes.push_back(i);

            cout << "\n{" << displayIndex << "}\n";
            card->display();

            displayIndex++;
        }

        if (playableSchemes.empty())
        {
            cout << "\n[!] This fighter has no playable Scheme card.\n";
            return;
        }

        int choice = 0;
        while (true)
        {
            cout << "\n> Choose Scheme Card (1 to " << playableSchemes.size() << ") : ";
            cin >> choice;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "[!] Please enter a number!\n";
                continue;
            }

            if (choice >= 1 && choice <= playableSchemes.size())
            {
                break;
            }
            cout << "[!] Invalid choice! :( Try again.\n";
        }

        int handIndex = playableSchemes[choice - 1];
        Card *scheme = hand.getCard(handIndex);

        try
        {
            Effect *effect = scheme->getEffect();

            if (effect != nullptr)
            {
                effect->apply(
                    *this,
                    *fighter,
                    *fighter,
                    *scheme,
                    nullptr,
                    false);
            }
            hand.removeCard(handIndex);
            currentPlayer->getDiscardPile().addCard(scheme);

            ui.renderScreen(*this);

            cout << "\n[+] Scheme card played successfully.\n";
            turnManager.useAction();
            return;
        }
        catch (const exception &e)
        {
            cerr << "\n[!] Error playing card: " << e.what() << endl;
            return;
        }
    }
}

void Game::attack()
{
    Player *currentPlayer = turnManager.getCurrentPlayer();
    Player *opponent = turnManager.getWaitingPlayer();

    vector<Fighter *> fighters;

    if (currentPlayer->getHero()->isAlive())
    {
        fighters.push_back(currentPlayer->getHero());
    }

    for (Sidekick *sidekick : currentPlayer->getSideKicks())
    {
        if (sidekick->isAlive())
        {
            fighters.push_back(sidekick);
        }
    }

    int fighterChoice = 0;

    while (fighterChoice < 1 || fighterChoice > fighters.size())
    {
        cout << "\n==========< Choose Fighter >==========\n";

        for (int i = 0; i < fighters.size(); i++)
        {
            cout << i + 1 << ". " << fighters[i]->getName() << endl;
        }

        cout << "~~> ";
        cin >> fighterChoice;

        if (fighterChoice < 1 || fighterChoice > fighters.size())
            cout << "[!] Invalid choice! :<\n";
    }

    Fighter *attacker = fighters[fighterChoice - 1];

    Fighter *defender = selectTarget(*currentPlayer, attacker);

    if (defender == nullptr)
    {
        cerr << "\n[!] ERROR : No target available! :<\n";
        return;
    }

    bool hasAttackCard = false;

    for (Card *card : currentPlayer->getHand().getCards())
    {
        if ((card->isAttack() || card->isVersatile()) && card->isPlayableBy(*attacker))
        {
            hasAttackCard = true;
            break;
        }
    }

    if (!hasAttackCard)
    {
        cout << "\n[!] " << attacker->getName() << " has no playable Attack card. :<\n";
        return;
    }

    ui.renderScreen(*this);

    Card *attackCard = nullptr;

    while (true)
    {
        cout << "\n========== * Attacker's Hand * ==========\n";
        currentPlayer->getHand().display();

        int attackIndex;

        cout << "~~> ";
        cin >> attackIndex;

        if (attackIndex < 1 || attackIndex > currentPlayer->getHand().getSize())
        {
            cout << "[!] Invalid choice!\n";
            continue;
        }

        Card *selected = currentPlayer->getHand().getCard(attackIndex - 1);

        if (!(selected->isAttack() || selected->isVersatile()))
        {
            cout << "\n[!] This card cannot be used for attack! :<\n";
            continue;
        }

        if (!selected->isPlayableBy(*attacker))
        {
            cout << "\n[!] " << attacker->getName() << " cannot play this card! :<\n";
            continue;
        }

        attackCard = currentPlayer->getHand().removeCard(attackIndex - 1);

        break;
    }

    Card *defenceCard = nullptr;

    int answer = 0;

    while (answer != 1 && answer != 2)
    {
        cout << "\n[?] Does defender want to defend?\n";
        cout << "1. Yes\n";
        cout << "2. No\n";
        cout << "~~> ";

        cin >> answer;

        if (answer != 1 && answer != 2)
        {
            cout << "[!] Invalid choice!\n";
        }
    }

    if (answer == 1)
    {
        bool hasDefenseCard = false;

        for (Card *card : opponent->getHand().getCards())
        {
            if ((card->isDefense() || card->isVersatile()) && card->isPlayableBy(*defender))
            {
                hasDefenseCard = true;
                break;
            }
        }

        if (!hasDefenseCard)
        {
            cout << "\n[!] Defender has no playable Defense card. :<\n";
        }
        else
        {
            while (true)
            {
                cout << "\n========== * Defender's Hand * ==========\n";
                opponent->getHand().display();

                int defenceIndex;

                cout << "~~> ";
                cin >> defenceIndex;

                if (defenceIndex < 1 || defenceIndex > opponent->getHand().getSize())
                {
                    cout << "[!] Invalid choice!\n";
                    continue;
                }

                Card *selected = opponent->getHand().getCard(defenceIndex - 1);

                if (!(selected->isDefense() || selected->isVersatile()))
                {
                    cout << "\n[!] This card cannot be used for defense! :<\n";
                    continue;
                }

                if (!selected->isPlayableBy(*defender))
                {
                    cout << "\n[!] " << defender->getName() << " cannot play this card! :<\n";
                    continue;
                }

                defenceCard = opponent->getHand().removeCard(defenceIndex - 1);

                break;
            }
        }
    }

    combatSystem.resolveCombat(*this, *attacker, *defender, *attackCard, defenceCard);

    ui.renderScreen(*this);

    turnManager.useAction();
}

void Game::discardUntilHandLimit()
{
    Player *currentPlayer = turnManager.getCurrentPlayer();

    if (currentPlayer == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Current player NOT found!\n");
    }

    while (turnManager.checkHandLimit())
    {
        cout << "[!] Your hand contains MORE THAN 7 cards!\n";

        cout << "[o] Choose one card to discard.\n";

        currentPlayer->getHand().display();

        int choice = ui.chooseCard(*currentPlayer);

        if (choice < 1 || choice > currentPlayer->getHand().getSize())
        {
            throw out_of_range("\n[!] ERROR : Invalid card!\n");
        }

        Card *discarded = currentPlayer->getHand().removeCard(choice - 1);

        currentPlayer->getDiscardPile().addCard(discarded);

        cout << "\n[+] "
             << discarded->getName()
             << " discarded successfully.\n\n";
    }
}

void Game::run()
{

    vector<string> entries =
        {
            "Play",
            "Help",
            "Exit"};

    int selected = 0;

    try
    {
        initialize();

        while (!isGameOver())
        {
            try
            {
                processTurn();
            }
            catch (const exception &e)
            {
                cerr << "\n[!] Error in turn: " << e.what() << endl;
                cout << "\n[.] Press Enter to continue...";
                cin.ignore();
                cin.get();
            }
        }

        endGame();
    }
    catch (const exception &e)
    {
        cerr << "\n[!] Game initialization failed : " << e.what() << endl;
        return;
    }
}

bool Game::isGameOver() const
{
    if (players.size() != 2)
    {
        return true;
    }

    Hero *hero1 = players[0]->getHero();
    Hero *hero2 = players[1]->getHero();

    if (hero1 == nullptr || hero2 == nullptr)
    {
        return true;
    }
    if (hero1->isAlive() && hero2->isAlive())
    {
        return false;
    }
    return true;
}

void Game::endGame()
{
    if (players.size() != 2)
    {
        return;
    }

    Hero *hero1 = players[0]->getHero();
    Hero *hero2 = players[1]->getHero();

    if (hero1->isAlive() && !hero2->isAlive())
    {
        ui.showWinner(*players[0]);
    }
    else if (!hero1->isAlive() && hero2->isAlive())
    {
        ui.showWinner(*players[1]);
    }
    else
    {
        cout << "\n __________________________________________";
        cout << "\n|                GAME OVER                 |";
        cout << "\n ------------------------------------------";
        cout << "\n| * o * . * O * . * o * . * O * . * o * O  |";
        cout << "\n|    ~~~>THE GAME ENDED IN A DRAW! :[<~~~  |";
        cout << "\n ------------------------------------------\n";
    }
}

const TurnManager &Game::getTurnManager() const
{
    return turnManager;
}

const Board &Game::getBoard() const
{
    return board;
}

const CombatSystem &Game::getCombatSystem() const
{
    return combatSystem;
}