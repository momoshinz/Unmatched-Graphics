#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Hero.h"
#include "fighter/Dracula.h"
#include "fighter/Sherlock.h"
#include "fighter/InvisibleMan.h"
#include "fighter/Sisters.h"
#include "fighter/DrWatson.h"
#include "fighter/Sidekick.h"
#include "fighter/Fog.h"
#include "card/Card.h"
#include "card/Deck.h"
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
#include <algorithm>
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
    if (age1 <= 0 || age2 <= 0)
    {
        throw invalid_argument("\n[!] ERROR : Age must be greater than 0!\n");
    }

    if (age1 > 100 || age2 > 100)
    {
        throw invalid_argument("\n[!] ERROR : Age cannot be greater than 100!\n");
    }

    Player *player1 = new Player(age1);
    Player *player2 = new Player(age2);

    addPlayer(player1);
    addPlayer(player2);

    board.setupMap();
    isMapSetUp = true;

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
        return nullptr;
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

        Hero *hero = currentPlayer->getHero();

        if (turnManager.consumeTurnStart())
        {
            if (dynamic_cast<Dracula *>(hero) != nullptr)
            {
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

    currentPlayer->drawCardToHand();
}

void Game::playSchemeCard()
{
    Player *currentPlayer = turnManager.getCurrentPlayer();

    if (currentPlayer == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Current player not found!\n");
    }

    Card *scheme = nullptr;

    if (scheme == nullptr)
    {
        return;
    }

    Hero *hero = currentPlayer->getHero();
    if (hero == nullptr)
    {
        return;
    }

    try
    {
        Effect *effect = scheme->getEffect();

        if (effect != nullptr)
        {
            EffectChoice choice;

            effect->apply(*this, *hero, *hero, *scheme, nullptr, false, choice);
        }

        Hand &hand = currentPlayer->getHand();

        for (int i = 0; i < hand.getSize(); i++)
        {
            if (hand.getCard(i) == scheme)
            {
                Card *playedCard = hand.removeCard(i);
                currentPlayer->getDiscardPile().addCard(playedCard);
                break;
            }
        }

        turnManager.useAction();
    }
    catch (const exception &e)
    {
        cerr << "\n[!] Error playing Scheme card: " << e.what() << endl;
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
        cin >> fighterChoice;

        if (fighterChoice < 1 || fighterChoice > fighters.size())
            cout << "[!] Invalid choice! :<\n";
    }

    Fighter *attacker = fighters[fighterChoice - 1];

    Fighter *defender = selectTarget(*currentPlayer, attacker);

    if (defender == nullptr)
    {
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
        return;
    }

    Card *attackCard = nullptr;

    while (true)
    {
        int attackIndex;

        cin >> attackIndex;

        if (attackIndex < 1 || attackIndex > currentPlayer->getHand().getSize())
        {
            continue;
        }

        Card *selected = currentPlayer->getHand().getCard(attackIndex - 1);

        if (!(selected->isAttack() || selected->isVersatile()))
        {
            continue;
        }

        if (!selected->isPlayableBy(*attacker))
        {
            continue;
        }

        attackCard = currentPlayer->getHand().removeCard(attackIndex - 1);

        break;
    }

    Card *defenceCard = nullptr;

    int answer = 0;

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
                int defenceIndex;

                cin >> defenceIndex;

                if (defenceIndex < 1 || defenceIndex > opponent->getHand().getSize())
                {
                    continue;
                }

                Card *selected = opponent->getHand().getCard(defenceIndex - 1);

                if (!(selected->isDefense() || selected->isVersatile()))
                {
                    continue;
                }

                if (!selected->isPlayableBy(*defender))
                {
                    continue;
                }

                defenceCard = opponent->getHand().removeCard(defenceIndex - 1);

                break;
            }
        }
    }

    combatSystem.resolveCombat(*this, *attacker, *defender, *attackCard, defenceCard);

    turnManager.useAction();
}

void Game::discardUntilHandLimit()
{
    Player *currentPlayer = turnManager.getCurrentPlayer();

    if (currentPlayer == nullptr)
    {
        throw runtime_error("\n[!] ERROR : Current player NOT found!\n");
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
    board.clear();
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

            if (type == "* Sister 1" || type == "* Sister 2" || type == "* Sister 3" || type == "Sister 3")
            {
                int id = sidekickJson["ID"];
                sidekick = new Sisters(id);
            }
            else if (type == "* Dr. Watson")
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

    if (player->hasHero())
    {
        return false;
    }

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

void Game::startGame()
{
    if (players.size() != 2)
    {
        return;
    }

    for (Player *player : players)
    {
        if (player == nullptr || !player->hasHero())
        {
            return;
        }
    }

    for (Player *player : players)
    {
        Hero *hero = player->getHero();

        if (hero == nullptr)
        {
            continue;
        }

        if (hero->getName() == "SHERLOCK HOLMES")
        {
            Deck::SherlockDeck(player->getDeck());
        }
        else if (hero->getName() == "DRACULA")
        {
            Deck::DraculaDeck(player->getDeck());
        }
        else if (hero->getName() == "INVISIBLE MAN")
        {
            Deck::InvisibleManDeck(player->getDeck());
        }
    }

    for (Player *player : players)
    {
        if (player == nullptr)
        {
            continue;
        }

        int drawn = player->drawCards(5);
    }
}

void Game::beginTurns()
{
    if (youngerPlayer == nullptr || olderPlayer == nullptr)
    {
        throw std::runtime_error("\n[!] ERROR : Players not determined yet!\n");
    }

    turnManager.startGame(youngerPlayer, olderPlayer);
}

void Game::setFeintBlocked(const std::string &message)
{
    feintBlockedFlag = true;
    feintBlockedMessage = message;
}

bool Game::consumeFeintBlockedFlag(std::string &outMessage)
{
    if (!feintBlockedFlag)
    {
        return false;
    }

    feintBlockedFlag = false;
    outMessage = feintBlockedMessage;
    return true;
}

void Game::resetGame()
{
    for (Player *player : players)
    {
        delete player;
    }

    players.clear();

    youngerPlayer = nullptr;
    olderPlayer = nullptr;

    isMapSetUp = false;

    turnManager = TurnManager();
    combatSystem = CombatSystem();

    board = Board();
    feintBlockedFlag = false;
    feintBlockedMessage.clear();
}