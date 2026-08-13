#include "game/Game.h" //kjhkjkffhuhiu
#include "fighter/Dracula.h"
#include "fighter/Sherlock.h"
#include "fighter/InvisibleMan.h"
#include "fighter/Sisters.h"
#include "fighter/DrWatson.h"
#include "fighter/Sidekick.h"
#include "card/Card.h"
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <nlohmann/json.hpp>
#include <raylib.h>
using json = nlohmann::json;
using namespace std;

Card *createCardByName(const string &name)
{
    if (name == "Feeding Frenzy")
        return new Card(Card::createFeedingFrenzy());

    if (name == "Mistform")
        return new Card(Card::createMistForm());

    if (name == "Ambush")
        return new Card(Card::createAmbush());

    if (name == "Baptism Of Blood")
        return new Card(Card::createBaptismOfBlood());

    if (name == "Beastform")
        return new Card(Card::createBeastform());

    if (name == "Dash")
        return new Card(Card::createDash());

    if (name == "Exploit")
        return new Card(Card::createExploit());

    if (name == "Look Into My Eyes")
        return new Card(Card::createLookIntoMyEyes());

    if (name == "Prey Upon")
        return new Card(Card::createPreyUpon());

    if (name == "Ravening Seduction")
        return new Card(Card::createRaveningSeduction());

    if (name == "Thirst For Sustenance")
        return new Card(Card::createThirstForSustenance());

    if (name == "Feint")
        return new Card(Card::createFeintDracula());

    if (name == "Administer Aid")
        return new Card(Card::createAdministerAid());

    if (name == "Counter Punch")
        return new Card(Card::createCounterPunch());

    if (name == "Deduce Strategy")
        return new Card(Card::createDeduceStrategy());

    if (name == "Education Never Ends")
        return new Card(Card::createEducationNeverEnds());

    if (name == "Eliminate The Impossible")
        return new Card(Card::createEliminateTheImpossible());

    if (name == "Fixed Point")
        return new Card(Card::createFixedPoint());

    if (name == "Master Of Disguise")
        return new Card(Card::createMasterOfDisguise());

    if (name == "Study Methods")
        return new Card(Card::createStudyMethods());

    if (name == "The Game Is Afoot")
        return new Card(Card::createTheGameIsAfoot());

    if (name == "Service Revolver")
        return new Card(Card::createServiceRevolver());

    if (name == "Feint")
        return new Card(Card::createFeintSherlock());

    if (name == "Coded Notes")
        return new Card(Card::createCodedNotes());

    if (name == "Dreaming Of Revenge")
        return new Card(Card::createDreamingOfRevenge());

    if (name == "Emerge From Mist")
        return new Card(Card::createEmergeFromMist());

    if (name == "Impossible To See")
        return new Card(Card::createImpossibleToSee());

    if (name == "Into Thin Air")
        return new Card(Card::createIntoThinAir());

    if (name == "Lurking")
        return new Card(Card::createLurking());

    if (name == "Reign Of Terror")
        return new Card(Card::createReignOfTerror());

    if (name == "Rolling Fog")
        return new Card(Card::createRollingFog());

    if (name == "Slip Away")
        return new Card(Card::createSlipAway());

    if (name == "Step Lightly")
        return new Card(Card::createStepLightly());

    return nullptr;
}

Game::Game()
    : youngerPlayer(nullptr),
      olderPlayer(nullptr)
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

void Game::initialize(int age1, int age2)
{
    // =========================================
    // VALIDATE AGES
    // =========================================

    if (age1 <= 0 || age2 <= 0)
    {
        throw invalid_argument(
            "\n[!] ERROR : Age must be greater than 0!\n");
    }

    if (age1 > 100 || age2 > 100)
    {
        throw invalid_argument(
            "\n[!] ERROR : Age cannot be greater than 100!\n");
    }

    // =========================================
    // CREATE PLAYERS
    // =========================================

    Player *player1 = new Player(age1);
    Player *player2 = new Player(age2);

    addPlayer(player1);
    addPlayer(player2);

    // =========================================
    // SETUP BOARD
    // =========================================

    board.setupMap();
    isMapSetUp = true;

    // =========================================
    // DETERMINE PLAYER ORDER
    // =========================================

    if (age1 < age2)
    {
        youngerPlayer = player1;
        olderPlayer = player2;
    }
    else if (age2 < age1)
    {
        youngerPlayer = player2;
        olderPlayer = player1;
    }
    else
    {
        // Same age -> random first player
        if (GetRandomValue(0, 1) == 0)
        {
            youngerPlayer = player1;
            olderPlayer = player2;
        }
        else
        {
            youngerPlayer = player2;
            olderPlayer = player1;
        }
    }

    // =========================================
    // INFORMATION
    // =========================================

    cout << "\n========================================\n";
    cout << "         GAME INITIALIZED\n";
    cout << "========================================\n";

    cout << "Player 1 age : "
         << player1->getAge()
         << '\n';

    cout << "Player 2 age : "
         << player2->getAge()
         << '\n';

    if (age1 == age2)
    {
        cout << "\nBoth players have the same age.\n";
        cout << youngerPlayer->getAge()
             << " years old was randomly selected to play first.\n";
    }

    cout << "\nFirst player : "
         << (youngerPlayer == player1
                 ? "Player 1"
                 : "Player 2")
         << '\n';

    cout << "Second player : "
         << (olderPlayer == player1
                 ? "Player 1"
                 : "Player 2")
         << '\n';

    cout << "========================================\n";
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
    cout << "3. Play Scheme\n";
    cout << "4. Save Game\n ~~> ";

    int choice;
    cin >> choice;
    while (choice < 1 || choice > 4)
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

    case 4:
        saveMenu();
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

        if (turnManager.consumeTurnStart())
        {
            if (dynamic_cast<Dracula *>(hero) != nullptr)
            {
                cout << "\n[*] Dracula may use his special ability.\n";

                try
                {
                    hero->useAbility(*this, *currentPlayer);
                }
                catch (const exception &e)
                {
                    cerr << "\n[!] Error using Dracula's ability : "
                         << e.what() << endl;
                }
            }
        }

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

void Game::run(bool loaded)
{

    vector<string> entries =
        {
            "New Game",
            "Load Game",
            "Exit"};

    int selected = 0;

    try
    {
        if (!loaded)
        {
           // initialize();
        }

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

void Game::placeFog(Fog *fog)
{
    if (fog == nullptr)
    {
        return;
    }

    while (true)
    {
        ui.renderBoardOnly(*this);

        vector<Space *> validSpaces;

        cout << "\nPlace Fog " << fog->getID() << endl;
        cout << "Available homes :\n";
        cout << "======================================\n";

        for (Space *space : board.getSpaces())
        {
            if (space->hasFogToken())
            {
                continue;
            }

            validSpaces.push_back(space);

            cout << validSpaces.size()
                 << ". Home "
                 << space->getId()
                 << endl;
        }

        cout << "\n> Choose a home : ";

        int choice;
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');

            cout << "\n[!] Please enter a number!\n";
            continue;
        }

        if (choice < 1 || choice > static_cast<int>(validSpaces.size()))
        {
            cout << "\n[!] Invalid choice!\n";
            continue;
        }

        Space *selectedSpace = validSpaces[choice - 1];

        fog->setPosition(selectedSpace);
        selectedSpace->setFog(fog);
        selectedSpace->setFogToken(true);

        break;
    }
}

void Game::saveGame(const string &filename)
{
    json j;

    json playersArray = json::array();

    for (Player *player : players)
    {
        json playerJson;

        playerJson["Age"] = player->getAge();

        Hero *hero = player->getHero();

        playerJson["Hero"]["Type"] = hero->getName();
        playerJson["Hero"]["Health"] = hero->getHealth();

        if (hero->getPosition() != nullptr)
            playerJson["Hero"]["Position"] = hero->getPosition()->getId();
        else
            playerJson["Hero"]["Position"] = -1;

        json sidekickArray = json::array();

        for (Sidekick *sidekick : player->getSideKicks())
        {
            json s;

            s["Type"] = sidekick->getName();
            s["Health"] = sidekick->getHealth();

            Sisters *sister = dynamic_cast<Sisters *>(sidekick);

            if (sister != nullptr)
                s["ID"] = sister->getID();

            Watson *watson = dynamic_cast<Watson *>(sidekick);

            if (watson != nullptr)
                s["ID"] = 0;

            if (sidekick->getPosition() != nullptr)
                s["Position"] = sidekick->getPosition()->getId();
            else
                s["Position"] = -1;

            sidekickArray.push_back(s);
        }

        playerJson["Sidekicks"] = sidekickArray;

        json fogArray = json::array();

        for (Fog *fog : player->getFogs())
        {
            json f;

            f["ID"] = fog->getID();

            if (fog->getPosition() != nullptr)
                f["Position"] = fog->getPosition()->getId();
            else
                f["Position"] = -1;

            fogArray.push_back(f);
        }

        playerJson["Fogs"] = fogArray;

        json handArray = json::array();

        for (Card *card : player->getHand().getCards())
        {
            handArray.push_back(card->getName());
        }

        playerJson["Hand"] = handArray;

        json deckArray = json::array();

        for (Card *card : player->getDeck().getCards())
        {
            deckArray.push_back(card->getName());
        }

        playerJson["Deck"] = deckArray;

        json discardArray = json::array();

        for (Card *card : player->getDiscardPile().getCards())
        {
            discardArray.push_back(card->getName());
        }

        playerJson["Discard"] = discardArray;

        playersArray.push_back(playerJson);
    }

    j["Players"] = playersArray;

    j["Turn"]["RemainingActions"] =
        turnManager.getRemainingActions();

    j["Turn"]["TurnNumber"] =
        turnManager.getTurnNumber();

    j["Turn"]["TurnJustStarted"] = turnManager.getTurnJustStarted();

    Player *current = turnManager.getCurrentPlayer();

    if (current == players[0])
        j["Turn"]["CurrentPlayer"] = 0;
    else
        j["Turn"]["CurrentPlayer"] = 1;

    Hero *hero1 = players[0]->getHero();
    Hero *hero2 = players[1]->getHero();

    j["GameOver"] = isGameOver();

    if (isGameOver())
    {
        if (hero1->isAlive() && !hero2->isAlive())
        {
            j["Winner"] = hero1->getName();
        }
        else if (!hero1->isAlive() && hero2->isAlive())
        {
            j["Winner"] = hero2->getName();
        }
        else
        {
            j["Winner"] = "Draw";
        }
    }
    else
    {
        j["Winner"] = "";
    }
    ofstream file(filename);

    if (!file)
    {
        throw runtime_error("Cannot open save file.");
    }
    file << setw(4) << j;

    file.close();

    cout << "\n[+] Game Saved Successfully.\n";
}

void Game::loadGame(const string &filename)
{
    ifstream file(filename);

    if (!file)
    {
        throw runtime_error("Cannot open save file.");
    }

    json j;
    file >> j;
    file.close();

    for (Player *player : players)
    {
        delete player;
    }

    players.clear();
    board.setupMap();

    for (const auto &playerJson : j["Players"])
    {
        int age = playerJson["Age"];

        Player *player = new Player(age);

        string heroType = playerJson["Hero"]["Type"];

        Hero *hero = nullptr;

        if (heroType == "DRACULA")
        {
            hero = new Dracula();
        }
        else if (heroType == "SHERLOCK HOLMES")
        {
            hero = new Sherlock();
        }
        else if (heroType == "INVISIBLE MAN")
        {
            hero = new InvisibleMan();
        }
        else
        {
            delete player;
            throw runtime_error("Unknown hero type : " + heroType);
        }

        player->setHero(hero);

        hero->setHealth(playerJson["Hero"]["Health"]);

        int heroPosition = playerJson["Hero"]["Position"];

        if (heroPosition != -1)
        {
            board.moveFighter(hero, board.getSpace(heroPosition));
        }

        for (const auto &sidekickJson : playerJson["Sidekicks"])
        {
            string type = sidekickJson["Type"];

            Sidekick *sidekick = nullptr;

            if (type == "Sister 1" || type == "Sister 2" || type == "Sister 3")
            {
                int id = sidekickJson["ID"];
                sidekick = new Sisters(id);
            }
            else if (type == "Dr. Watson")
            {
                sidekick = new Watson();
            }
            else
            {
                throw runtime_error("Unknown sidekick type : " + type);
            }

            sidekick->setHealth(sidekickJson["Health"]);

            player->addSideKick(sidekick);

            int position = sidekickJson["Position"];

            if (position != -1)
            {
                board.moveFighter(sidekick, board.getSpace(position));
            }
        }

        for (const auto &fogJson : playerJson["Fogs"])
        {
            int id = fogJson["ID"];

            Fog *fog = new Fog(id);

            player->addFog(fog);

            int position = fogJson["Position"];

            if (position != -1)
            {
                Space *space = board.getSpace(position);

                fog->setPosition(space);

                space->setFogToken(true);
            }
        }

        for (const auto &cardName : playerJson["Hand"])
        {
            Card *card = createCardByName(cardName);

            if (card == nullptr)
            {
                throw runtime_error(
                    "Unknown card : " + cardName.get<string>());
            }

            player->getHand().addCard(card);
        }

        for (const auto &cardName : playerJson["Deck"])
        {
            Card *card = createCardByName(cardName);

            if (card == nullptr)
            {
                throw runtime_error("Unknown card : " + cardName.get<string>());
            }

            player->getDeck().addCard(card);
        }

        for (const auto &cardName : playerJson["Discard"])
        {
            Card *card = createCardByName(cardName);

            if (card == nullptr)
            {
                throw runtime_error("Unknown card : " + cardName.get<string>());
            }

            player->getDiscardPile().addCard(card);
        }

        players.push_back(player);
    }

    int currentPlayerIndex = j["Turn"]["CurrentPlayer"];

    int remainingActions = j["Turn"]["RemainingActions"];

    int turnNumber = j["Turn"]["TurnNumber"];

    if (players.size() != 2)
    {
        throw runtime_error("Save file must contain exactly 2 players.");
    }

    Player *currentPlayer = players[currentPlayerIndex];

    Player *waitingPlayer = players[currentPlayerIndex == 0 ? 1 : 0];

    turnManager.setPlayers(currentPlayer, waitingPlayer);

    turnManager.setRemainingActions(remainingActions);

    turnManager.setTurnNumber(turnNumber);

    turnManager.setTurnJustStarted(j["Turn"]["TurnJustStarted"]);

    cout << "\n[+] Game Loaded Successfully!\n";
}

void Game::saveMenu()
{
    cout << "\n========== Save Game ==========\n";

    int slot = 1;
    while (true)
    {
        string filename = "save" + to_string(slot) + ".json";

        ifstream file(filename);

        if (!file)
            break;

        cout << slot << ". " << filename << endl;

        slot++;
    }
    cout << slot << ". New Save\n";
    cout << "0. Cancel\n";

    int choice;

    while (true)
    {
        cout << "\n> Choose slot: ";
        cin >> choice;

        if (choice >= 0 && choice <= 3)
            break;

        cout << "\n[!] ERROR : Invalid choice!\n";
    }

    if (choice == 0)
        return;

    string filename = "save" + to_string(choice) + ".json";

    ifstream test(filename);

    if (test)
    {
        int answer;

        cout << "\nThis slot already contains a saved game.\n";
        cout << "Overwrite?\n";
        cout << "1. Yes\n";
        cout << "2. No\n";
        cout << "Choice: ";

        cin >> answer;

        if (answer != 1)
            return;
    }

    saveGame(filename);
}

bool Game::loadMenu()
{
    vector<string> saves;

    int slot = 1;

    while (true)
    {
        string filename = "save" + to_string(slot) + ".json";

        ifstream file(filename);

        if (!file)
            break;

        saves.push_back(filename);
        slot++;
    }

    if (saves.empty())
    {
        cout << "\n[!] ERROR : No saved games found.\n";
        cout << "> Press Enter to return...";
        return false;
    }

    cout << "\n========== LOAD GAME ==========\n";

    for (int i = 0; i < saves.size(); i++)
    {
        cout << i + 1 << ". " << saves[i] << endl;
    }

    cout << "0. Back\n";

    int choice;

    while (true)
    {
        cout << "\n> Choose a save: ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 0)
            return false;

        if (choice >= 1 && choice <= saves.size())
            break;

        cout << "\n[!] ERROR : Invalid choice!\n";
    }

    loadGame(saves[choice - 1]);

    return true;
}

Player *Game::getYoungerPlayer() const
{
    return youngerPlayer;
}

Player *Game::getOlderPlayer() const
{
    return olderPlayer;
}

bool Game::assignHero(int playerIndex, const std::string &heroName)
{
    // =========================================
    // VALIDATE PLAYER INDEX
    // =========================================

    if (playerIndex < 1 ||
        playerIndex > static_cast<int>(players.size()))
    {
        return false;
    }

    Player *player = players[playerIndex - 1];

    if (player == nullptr)
    {
        return false;
    }

    // =========================================
    // PLAYER MUST NOT ALREADY HAVE A HERO
    // =========================================

    if (player->hasHero())
    {
        return false;
    }

    // =========================================
    // CHECK THAT HERO IS NOT ALREADY SELECTED
    // BY THE OTHER PLAYER
    // =========================================

    for (Player *otherPlayer : players)
    {
        if (otherPlayer == nullptr ||
            otherPlayer == player)
        {
            continue;
        }

        Hero *otherHero = otherPlayer->getHero();

        if (otherHero == nullptr)
        {
            continue;
        }

        if (otherHero->getName() == heroName)
        {
            return false;
        }
    }

    // =========================================
    // CREATE HERO
    // =========================================

    if (heroName == "dracula")
    {
        player->setHero(new Dracula());

        player->addSideKick(new Sisters(1));
        player->addSideKick(new Sisters(2));
        player->addSideKick(new Sisters(3));
    }
    else if (heroName == "sherlock")
    {
        player->setHero(new Sherlock());

        player->addSideKick(new Watson());
    }
    else if (heroName == "invisible_man")
    {
        player->setHero(new InvisibleMan());

        player->addFog(new Fog(1));
        player->addFog(new Fog(2));
        player->addFog(new Fog(3));
    }
    else
    {
        return false;
    }

    return true;
}