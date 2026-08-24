#include "graphics/GameScreen.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include "graphics/MapCoordinates.h"
#include "board/Space.h"
#include "fighter/Hero.h"
#include "fighter/Fog.h"
#include "card/Card.h"
#include "card/Hand.h"
#include "fighter/InvisibleMan.h"
#include "fighter/Dracula.h"
#include "fighter/Sherlock.h"
#include <sstream>
#include <unordered_map>
#include "graphics/SaveManager.h"

static std::string getCardTextureKeyForHandLimit(const Card *card, const std::string &heroName)
{
    if (card == nullptr)
        return "";

    const std::string &name = card->getName();

    if (name == "Feint")
    {
        return (heroName == "DRACULA") ? "FeintDracula" : "FeintSherlock";
    }
    static const std::unordered_map<std::string, std::string> nameToKey = {
        {"Feeding Frenzy", "FeedingFrenzy"}, {"Beastform", "BeastForm"}, {"Ambush", "Ambush"}, {"Thirst For Sustenance", "ThirstForSustenance"}, {"Look Into My Eyes", "LookIntoMyEyes"}, {"Dash", "Dash"}, {"Exploit", "Exploit"}, {"Mistform", "MistForm"}, {"Baptism Of Blood", "BaptismOfBlood"}, {"Prey Upon", "PreyUpon"}, {"Ravening Seduction", "RaveningSeduction"}, {"The Game Is Afoot", "TheGameIsAfoot"}, {"Service Revolver", "ServiceRevolver"}, {"Counter Punch", "CounterPunch"}, {"Deduce Strategy", "DeduceStrategy"}, {"Education Never Ends", "EducationNeverEnds"}, {"Fixed Point", "FixedPoint"}, {"Study Methods", "StudyMethods"}, {"Administer Aid", "AdministerAid"}, {"Eliminate The Impossible", "EliminateTheImpossible"}, {"Master Of Disguise", "MasterOfDisguise"}, {"Coded Notes", "CodedNotes"}, {"Dreaming Of Revenge", "DreamingOfRevange"}, {"Emerge From Mist", "EmergeFromMist"}, {"Impossible To See", "ImpossibleToSee"}, {"Into Thin Air", "IntoThinAir"}, {"Lurking", "Lurking"}, {"Reign Of Terror", "ReignOfTerror"}, {"Rolling Fog", "RollingFog"}, {"Slip Away", "SlipAway"}, {"Step Lightly", "StepLightly"}};

    auto it = nameToKey.find(name);
    return (it == nameToKey.end()) ? "" : it->second;
}

GameScreen::GameScreen(AssetManager *assets, Game *game)
    : assets(assets),
      game(game),
      leftPlayerPanel(assets),
      rightPlayerPanel(assets),
      attackUI(assets),
      schemeUI(assets),
      maneuverUI(assets),
      effectUI(assets),
      draculaAbilityUI(assets)
{
}

int GameScreen::update()
{
    if (assets == nullptr)
    {
        return 0;
    }

    Vector2 mousePosition = GetMousePosition();

    // =========================================
    // DRACULA START-OF-TURN ABILITY
    // =========================================

    // needs to be first due to the priority it has
    if (!guideOpen && !draculaAbilityUI.isOpen() && game->getTurnManager().getTurnJustStarted())
    {
        if (game->getTurnManager().consumeTurnStart())
        {
            Player *currentPlayer = game->getTurnManager().getCurrentPlayer();

            if (currentPlayer != nullptr)
            {
                Hero *hero = currentPlayer->getHero();

                if (dynamic_cast<Dracula *>(hero) != nullptr)
                {
                    draculaAbilityUI.open(game, hero, currentPlayer);
                }
            }
        }
    }

    if (feintBlockedPopupOpen)
    {
        feintBlockedPopupTimer -= GetFrameTime();

        if (feintBlockedPopupTimer <= 0.0f)
        {
            feintBlockedPopupOpen = false;
        }

        return 0;
    }

    if (draculaAbilityUI.isOpen())
    {
        draculaAbilityUI.update();
    }

    if (draculaAbilityUI.consumeFinished())
    {
        std::cout << "[.] Dracula ability phase finished." << std::endl;
    }

    // =========================================
    // Button dimensions
    // =========================================

    const float buttonWidth = 145.0f;
    const float buttonHeight = 48.0f;
    const float buttonGap = 15.0f;

    const float totalWidth = 3.0f * buttonWidth + 2.0f * buttonGap;

    const float startX = (GetScreenWidth() - totalWidth) / 2.0f;

    const float buttonY = 12.0f;

    Rectangle exitButton{startX, buttonY, buttonWidth, buttonHeight};

    Rectangle saveButton{startX + buttonWidth + buttonGap, buttonY, buttonWidth, buttonHeight};

    Rectangle guideButton{startX + 2.0f * (buttonWidth + buttonGap),
                          buttonY,
                          buttonWidth,
                          buttonHeight};

    if (saveMenuOpen)
    {
        updateSaveMenu();
        return 0;
    }

    if (guideOpen)
    {
        const float popupWidth = 900.0f;
        const float popupHeight = 720.0f;

        const float popupX = (GetScreenWidth() - popupWidth) / 2.0f;

        const float popupY = (GetScreenHeight() - popupHeight) / 2.0f;

        const float backWidth = 130.0f;
        const float backHeight = 45.0f;

        Rectangle backButton{(GetScreenWidth() - backWidth) / 2.0f, popupY + popupHeight - 65.0f,
                             backWidth,
                             backHeight};

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(mousePosition, backButton))
            {
                guideOpen = false;
            }
        }
        return 0;
    }

    // =========================================
    // Mouse click
    // =========================================

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (handLimitPopupOpen)
        {
            for (size_t i = 0; i < handLimitCardBoxes.size(); i++)
            {
                if (CheckCollisionPointRec(mousePosition, handLimitCardBoxes[i]))
                {
                    handLimitSelectedIndex = static_cast<int>(i);
                    return 0;
                }
            }
            if (handLimitSelectedIndex != -1 && CheckCollisionPointRec(mousePosition, handLimitBurnButton))
            {
                if (handLimitPlayer != nullptr)
                {
                    Card *toDiscard = handLimitCards[handLimitSelectedIndex];

                    Hand &hand = handLimitPlayer->getHand();
                    const std::vector<Card *> &cards = hand.getCards();

                    for (int i = 0; i < static_cast<int>(cards.size()); i++)
                    {
                        if (cards[i] == toDiscard)
                        {
                            Card *removed = hand.removeCard(i);
                            handLimitPlayer->getDiscardPile().addCard(removed);
                            break;
                        }
                    }
                }

                handLimitPopupOpen = false;
                if (handLimitPlayer != nullptr && handLimitPlayer->getHand().getSize() > 7)
                {
                    openHandLimitPopup(handLimitPlayer);
                }
            }
            return 0;
        }
        // =========================================
        // DECK EMPTY POPUP (بالاترین اولویت)
        // =========================================

        if (deckEmptyPopupOpen)
        {
            if (CheckCollisionPointRec(mousePosition, deckEmptyOkButton))
            {
                deckEmptyPopupOpen = false;
            }
            return 0;
        }
        // =========================================
        // EXIT
        // =========================================

        if (CheckCollisionPointRec(mousePosition, exitButton))
        {
            return 1;
        }

        // =========================================
        // SAVE GAME
        // =========================================

        if (CheckCollisionPointRec(mousePosition, saveButton))
        {
            refreshSaveFiles();
            saveMenuOpen = true;
            return 0;
        }

        // =========================================
        // GUIDE
        // =========================================

        if (CheckCollisionPointRec(mousePosition, guideButton))
        {
            guideOpen = true;

            // بستن تمام UIهای موقتی بازی
            attackUI.reset();
            schemeUI.resetConfirmed();
            maneuverUI.reset();
            effectUI.reset();

            combatShowLookButton = false;
            combatEffectRequested = false;

            return 0;
        }

        // =========================================
        // COMBAT RESULT REVEAL
        // =========================================

        if (combatInProgress &&
            !combatResultPopupOpen &&
            game->getCombatSystem().isAwaitingResultReveal())
        {
            if (CheckCollisionPointRec(mousePosition, resultRevealButton))
            {
                combatResultPopupOpen = true;
                return 0;
            }
        }

        // =========================================
        // ACTION BUTTONS (فقط وقتی مانور باز نیست)
        // =========================================

        if (!maneuverUI.isOpen() && !draculaAbilityUI.isOpen() && !combatInProgress)
        {
            if (CheckCollisionPointRec(mousePosition, attackButton))
            {
                selectedAction = ActionChoice::ATTACK;
                std::cout << "Selected Action: ATTACK" << std::endl;
                Player *currentPlayer = game->getTurnManager().getCurrentPlayer();

                if (currentPlayer != nullptr)
                {
                    std::vector<Fighter *> fighters;
                    Hero *hero = currentPlayer->getHero();

                    if (hero != nullptr && hero->isAlive())
                    {
                        fighters.push_back(hero);
                    }

                    std::vector<Sidekick *> sidekicks = currentPlayer->getSideKicks();

                    for (Sidekick *sidekick : sidekicks)
                    {
                        if (sidekick != nullptr && sidekick->isAlive())
                        {
                            fighters.push_back(sidekick);
                        }
                    }

                    attackUI.openAttack(currentPlayer, fighters, game);
                }

                selectedAction = ActionChoice::NONE;
                return 0;
            }

            if (CheckCollisionPointRec(mousePosition, maneuverButton))
            {
                selectedAction = ActionChoice::MANEUVER;

                std::cout << "Selected Action: MANEUVER" << std::endl;

                Player *currentPlayer = game->getTurnManager().getCurrentPlayer();

                if (currentPlayer != nullptr)
                {
                    maneuverUI.open(currentPlayer);
                }

                selectedAction = ActionChoice::NONE;
                return 0;
            }

            if (CheckCollisionPointRec(mousePosition, schemeButton))
            {
                selectedAction = ActionChoice::SCHEME;

                std::cout << "Selected Action: SCHEME" << std::endl;

                Player *currentPlayer = game->getTurnManager().getCurrentPlayer();

                if (currentPlayer != nullptr)
                {
                    schemeUI.openScheme(currentPlayer->getHand());
                }
                selectedAction = ActionChoice::NONE;
                return 0;
            }
        }

        if (combatShowLookButton && CheckCollisionPointRec(mousePosition, lookButton))
        {
            CombatSystem &combatSystem = game->getCombatSystem();

            effectUI.open(game, combatSystem.getPendingEffect(), combatSystem.getPendingUser(),
                          combatSystem.getPendingTarget());

            combatEffectRequested = true;
            combatShowLookButton = false;

            return 0;
        }

        if (combatInProgress && !combatResultPopupOpen && game->getCombatSystem().isAwaitingResultReveal() &&
            CheckCollisionPointRec(mousePosition, resultRevealButton))
        {
            combatResultPopupOpen = true;
            return 0;
        }

        if (combatResultPopupOpen && CheckCollisionPointRec(mousePosition, resultBackButton))
        {
            combatResultPopupOpen = false;
            game->getCombatSystem().acknowledgeResult();
            return 0;
        }

        // =========================================
        // MAP SPACE CLICK
        // =========================================
        int clickedSpaceId = getClickedSpaceId();

        if (clickedSpaceId != -1)
        {
            std::cout << "Clicked Space: " << clickedSpaceId << std::endl;

            if (maneuverUI.isSelectingSpace())
            {
                for (Space *space : maneuverUI.getAvailableMoves())
                {
                    if (space != nullptr && space->getId() == clickedSpaceId)
                    {
                        game->getBoard().moveFighter(maneuverUI.getSelectedFighter(), space);
                        maneuverUI.finishAfterMove();
                        break;
                    }
                }
            }

            if (effectUI.isChoosingSpace())
            {
                Space *clickedSpace = game->getBoard().getSpace(clickedSpaceId);

                if (clickedSpace != nullptr)
                {
                    std::cout << "[+] Effect destination clicked: Space " << clickedSpace->getId() << std::endl;

                    effectUI.selectSpace(clickedSpace);

                    if (effectUI.isReady() && combatInProgress && combatEffectRequested)
                    {
                        game->getCombatSystem().provideEffectChoice(effectUI.getChoice());
                        effectUI.reset();
                        combatEffectRequested = false;
                        combatShowLookButton = false;
                    }
                }
            }
        }
    } // <-- پایان if (IsMouseButtonPressed(...))

    if (combatInProgress)
    {
        CombatSystem &combatSystem = game->getCombatSystem();

        if (combatSystem.isWaitingForEffectInput() && !combatEffectRequested)
        {
            Effect *pending = combatSystem.getPendingEffect();

            if (pending != nullptr && pending->getInputKind() == EffectInputKind::ShowOpponentHand)
            {
                // به‌جای باز کردن خودکار، دکمه‌ی LOOK رو نشون بده
                combatShowLookButton = true;
            }
            else
            {
                effectUI.open(game, combatSystem.getPendingEffect(), combatSystem.getPendingUser(),
                              combatSystem.getPendingTarget());

                combatEffectRequested = true;
            }
        }
    }

    if (attackUI.isOpen())
    {
        attackUI.update();
    }

    if (attackUI.isAttackConfirmed())
    {
        Player *currentPlayer = game->getTurnManager().getCurrentPlayer();
        Fighter *chosenAttacker = attackUI.getSelectedAttacker();
        Fighter *chosenTarget = attackUI.getSelectedTarget();
        Card *chosenAttackCard = attackUI.getSelectedAttackCard();
        Card *chosenDefenseCard = attackUI.getSelectedDefenseCard();

        if (chosenAttacker != nullptr && chosenTarget != nullptr && chosenAttackCard != nullptr)
        {
            Player *defenderPlayer = chosenTarget->getOwner();
            Card *removedAttackCard = nullptr;
            Card *removedDefenceCard = nullptr;

            if (currentPlayer != nullptr)
            {
                Hand &attackerHand = currentPlayer->getHand();

                const std::vector<Card *> &attackerCards = attackerHand.getCards();

                for (int i = 0; i < static_cast<int>(attackerCards.size()); i++)
                {
                    if (attackerCards[i] == chosenAttackCard)
                    {
                        removedAttackCard = attackerHand.removeCard(i);
                        break;
                    }
                }
            }

            if (chosenDefenseCard != nullptr && defenderPlayer != nullptr)
            {
                Hand &defenderHand = defenderPlayer->getHand();

                const std::vector<Card *> &defenderCards = defenderHand.getCards();

                for (int i = 0; i < static_cast<int>(defenderCards.size()); i++)
                {
                    if (defenderCards[i] == chosenDefenseCard)
                    {
                        removedDefenceCard = defenderHand.removeCard(i);
                        break;
                    }
                }
            }

            if (removedAttackCard != nullptr)
            {
                game->getCombatSystem().beginCombat(*game, *chosenAttacker, *chosenTarget,
                                                    *removedAttackCard,
                                                    removedDefenceCard);

                combatInProgress = true;
                combatEffectRequested = false;
            }
        }
        attackUI.resetAttackConfirmed();
    }

    if (schemeUI.isOpen())
    {
        schemeUI.update();
    }

    if (schemeUI.isConfirmed())
    {
        Card *playedCard = schemeUI.getSelectedCard();

        if (playedCard != nullptr)
        {
            Player *currentPlayer = game->getTurnManager().getCurrentPlayer();

            if (currentPlayer != nullptr)
            {
                Effect *effect = playedCard->getEffect();

                if (effect == nullptr || effect->getInputKind() == EffectInputKind::None)
                {
                    // خودکار: مستقیم اجرا کن
                    if (effect != nullptr)
                    {
                        EffectChoice emptyChoice;
                        effect->apply(*game, *currentPlayer->getHero(),
                                      *currentPlayer->getHero(), *playedCard,
                                      nullptr, false, emptyChoice);
                    }

                    finalizeSchemeCard(playedCard);

                    game->getTurnManager().useAction();

                    std::cout << "[.] Actions remaining: "
                              << game->getTurnManager().getRemainingActions()
                              << std::endl;

                    checkAndEndTurnIfNeeded();
                }
                else
                {
                    // <<<< دقیقاً همینجا، به‌جای else فعلی، این نسخه رو بذارید >>>>
                    pendingSchemeCard = playedCard;

                    Fighter *actingFighter = currentPlayer->getHero();
                    Fighter *referenceFighter = currentPlayer->getHero();

                    if (playedCard->getOwnerType() == OwnerType::Sidekick)
                    {
                        if (!currentPlayer->getSideKicks().empty())
                        {
                            actingFighter = currentPlayer->getSideKicks()[0];
                        }
                    }

                    pendingEffectFighter = actingFighter;

                    effectUI.open(game, effect, actingFighter, referenceFighter);
                }
            }
        }
        schemeUI.resetConfirmed();
    }

    if (maneuverUI.isOpen())
    {
        maneuverUI.update();
    }

    if (maneuverUI.needsAvailableMoves())
    {
        Fighter *fighter = maneuverUI.getFighterNeedingMoves();

        if (fighter != nullptr)
        {
            int movementBudget = maneuverUI.getMovementBudget();

            std::vector<Space *> moves = game->getBoard().getAvailableMoves(fighter, movementBudget);

            if (moves.empty())
            {
                std::cout << "\n[!] No available moves for " << fighter->getName() << std::endl;
                maneuverUI.finishAfterMove();
            }
            else
            {
                maneuverUI.beginSpaceSelection(moves);
            }
        }
    }

    if (maneuverUI.consumeReadyToFinalize())
    {
        Player *currentPlayer = game->getTurnManager().getCurrentPlayer();

        if (currentPlayer != nullptr)
        {
            currentPlayer->drawCardToHand();

            std::cout << "[+] Drew one card from maneuver." << std::endl;

            game->getTurnManager().useAction();

            std::cout << "[.] Actions remaining: "
                      << game->getTurnManager().getRemainingActions()
                      << std::endl;

            checkAndEndTurnIfNeeded();
        }
    }

    if (effectUI.isOpen())
    {
        effectUI.update();
    }

    if (effectUI.isReady() && pendingSchemeCard != nullptr)
    {
        Player *currentPlayer = game->getTurnManager().getCurrentPlayer();

        if (currentPlayer != nullptr)
        {
            Effect *effect = pendingSchemeCard->getEffect();

            Fighter *actingFighter = currentPlayer->getHero();

            if (effect != nullptr && pendingEffectFighter != nullptr)
            {
                effect->apply(
                    *game,
                    *pendingEffectFighter,
                    *pendingEffectFighter,
                    *pendingSchemeCard,
                    nullptr,
                    false,
                    effectUI.getChoice());
            }
            finalizeSchemeCard(pendingSchemeCard);
            game->getTurnManager().useAction();

            std::cout << "[.] Actions remaining: "
                      << game->getTurnManager().getRemainingActions()
                      << std::endl;

            checkAndEndTurnIfNeeded();
        }
        pendingSchemeCard = nullptr;
        pendingEffectFighter = nullptr;
        effectUI.reset();
    }

    if (effectUI.isReady() && combatInProgress && combatEffectRequested)
    {
        game->getCombatSystem().provideEffectChoice(effectUI.getChoice());
        effectUI.reset();
        combatEffectRequested = false;
        combatShowLookButton = false;
    }

    if (combatInProgress && game->getCombatSystem().isFinished())
    {
        game->getTurnManager().useAction();

        std::cout << "[.] Actions remaining: "
                  << game->getTurnManager().getRemainingActions()
                  << std::endl;

        checkAndEndTurnIfNeeded();
        combatInProgress = false;
    }

    // =========================================
    // FEINT BLOCKED POPUP
    // =========================================

    if (!feintBlockedPopupOpen)
    {
        std::string message;

        if (game->consumeFeintBlockedFlag(message))
        {
            feintBlockedMessage = message;
            feintBlockedPopupOpen = true;
            feintBlockedPopupTimer = feintBlockedPopupDuration;
        }
    }

    // =========================================
    // DECK EMPTY POPUP
    // =========================================

    if (!deckEmptyPopupOpen)
    {
        Player *currentPlayer = game->getTurnManager().getCurrentPlayer();

        if (currentPlayer != nullptr && currentPlayer->consumeDeckEmptyFlag())
        {
            deckEmptyPopupOpen = true;
        }

        Player *waitingPlayer = game->getTurnManager().getWaitingPlayer();

        if (waitingPlayer != nullptr && waitingPlayer->consumeDeckEmptyFlag())
        {
            deckEmptyPopupOpen = true;
        }
    }

    // =========================================
    // HAND LIMIT POPUP
    // =========================================
    checkHandLimit(); // <-- خط جدید
    return 0;
}

void GameScreen::draw()
{
    if (assets == nullptr)
    {
        return;
    }

    Texture2D background = assets->getMainPanelBackground();

    if (background.id != 0)
    {
        DrawTexturePro(
            background,
            Rectangle{
                0.0f,
                0.0f,
                static_cast<float>(background.width),
                static_cast<float>(background.height)},
            Rectangle{
                0.0f,
                0.0f,
                static_cast<float>(GetScreenWidth()),
                static_cast<float>(GetScreenHeight())},
            Vector2{0.0f, 0.0f},
            0.0f,
            WHITE);
    }
    else
    {
        ClearBackground(Color{45, 30, 20, 255});
    }

    drawMap();
    drawPlayerPanels();
    drawTopButtons();
    drawTurnIndicator();

    if (saveMenuOpen)
    {
        drawSaveMenu();
        return;
    }

    // ========================================================
    // NORMAL GAME UI
    // ========================================================

    if (!deckEmptyPopupOpen &&
        !handLimitPopupOpen &&
        !feintBlockedPopupOpen &&
        !guideOpen)
    {
        drawCombatEffectText();
        drawResultRevealButton();
        drawLookButton();

        if (!maneuverUI.isOpen() &&
            !combatInProgress &&
            !draculaAbilityUI.isOpen())
        {
            drawActionButtons();
        }

        if (attackUI.isOpen())
        {
            attackUI.draw();
        }

        if (schemeUI.isOpen())
        {
            schemeUI.draw();
        }

        if (maneuverUI.isOpen())
        {
            maneuverUI.draw();
        }

        if (effectUI.isOpen())
        {
            effectUI.draw();
        }

        if (combatResultPopupOpen)
        {
            drawCombatResultPopup();
        }

        if (draculaAbilityUI.isOpen())
        {
            draculaAbilityUI.draw();
        }
    }

    // ========================================================
    // OTHER POPUPS
    // ========================================================

    drawDeckEmptyPopup();
    drawHandLimitPopup();
    drawFeintBlockedPopup();

    // ========================================================
    // GUIDE - ALWAYS ON TOP
    // ========================================================

    if (guideOpen)
    {
        drawGuidePopup();
    }
}

void GameScreen::drawMap()
{
    Texture2D map = assets->getGameMap();

    if (map.id == 0)
    {
        return;
    }

    // =====================================
    // DRAW MAP
    // =====================================

    float mapX;
    float mapY;
    float scale;
    float mapWidth;
    float mapHeight;

    calculateMapTransform(mapX, mapY, scale, mapWidth, mapHeight);

    Rectangle source{0.0f, 0.0f,
                     static_cast<float>(map.width),
                     static_cast<float>(map.height)};

    Rectangle destination{mapX, mapY, mapWidth, mapHeight};
    DrawTexturePro(map, source, destination, Vector2{0.0f, 0.0f}, 0.0f, WHITE);

    // =====================================
    // DRAW SPACE CIRCLES + NUMBERS
    // =====================================
    drawSpaces();
    drawPlacedFighters();
    drawFogs();
    drawManeuverMovableSpaces();
    drawEffectSelectableSpaces();
}

// =========================================
// DRAW PLAYER PANELS
// =========================================

void GameScreen::drawPlayerPanels()
{
    if (game == nullptr)
    {
        return;
    }

    const float panelWidth = 320.0f;
    const float margin = 12.0f;

    const float panelHeight = GetScreenHeight() - 2.0f * margin;

    leftPlayerPanel.setBounds(Rectangle{margin, margin, panelWidth, panelHeight});

    rightPlayerPanel.setBounds(Rectangle{GetScreenWidth() - panelWidth - margin,
                                         margin,
                                         panelWidth,
                                         panelHeight});

    const auto &players = game->getPlayers();

    if (players.size() > 0)
    {
        leftPlayerPanel.draw(players[0]);
    }

    if (players.size() > 1)
    {
        rightPlayerPanel.draw(players[1]);
    }
}

// =========================================
// DRAW TOP BUTTONS
// =========================================

void GameScreen::drawTopButtons()
{
    Font font = assets->getGameFont();

    // =========================================
    // Button dimensions
    // =========================================

    const float buttonWidth = 145.0f;
    const float buttonHeight = 48.0f;

    const float buttonGap = 15.0f;

    const float totalWidth = 3.0f * buttonWidth + 2.0f * buttonGap;

    const float startX = (GetScreenWidth() - totalWidth) / 2.0f;
    const float buttonY = 12.0f;

    // =========================================
    // Button rectangles
    // =========================================

    Rectangle exitButton{startX, buttonY, buttonWidth, buttonHeight};

    Rectangle saveButton{startX + buttonWidth + buttonGap, buttonY, buttonWidth, buttonHeight};

    Rectangle guideButton{startX + 2.0f * (buttonWidth + buttonGap),
                          buttonY,
                          buttonWidth,
                          buttonHeight};

    // =========================================
    // Mouse
    // =========================================
    Vector2 mousePosition = GetMousePosition();

    // =========================================
    // Colors
    // =========================================
    Color normalColor{25, 25, 25, 190};
    Color hoverColor{65, 65, 65, 210};

    // =========================================
    // EXIT
    // =========================================

    Color exitColor = CheckCollisionPointRec(mousePosition, exitButton) ? hoverColor : normalColor;

    DrawRectangleRounded(exitButton, 1.0f, 32, exitColor);

    // =========================================
    // SAVE GAME
    // =========================================

    Color saveColor = CheckCollisionPointRec(mousePosition, saveButton) ? hoverColor : normalColor;

    DrawRectangleRounded(saveButton, 1.0f, 32, saveColor);

    // =========================================
    // GUIDE
    // =========================================

    Color guideColor = CheckCollisionPointRec(mousePosition, guideButton)
                           ? hoverColor
                           : normalColor;

    DrawRectangleRounded(guideButton, 1.0, 32, guideColor);
    // =========================================
    // Text
    // =========================================
    const float fontSize = 26.0f;
    const float spacing = 2.0f;

    const char *exitText = "EXIT";
    const char *saveText = "SAVE GAME";
    const char *guideText = "HELP";

    // =========================================
    // Measure text
    // =========================================
    Vector2 exitTextSize = MeasureTextEx(font, exitText, fontSize, spacing);
    Vector2 saveTextSize = MeasureTextEx(font, saveText, fontSize, spacing);
    Vector2 guideTextSize = MeasureTextEx(font, guideText, fontSize, spacing);

    // =========================================
    // EXIT text
    // =========================================

    DrawTextEx(font, exitText, Vector2{exitButton.x + (exitButton.width - exitTextSize.x) / 2.0f,

                                       exitButton.y + (exitButton.height - exitTextSize.y) / 2.0f},
               fontSize,
               spacing,
               WHITE);

    // =========================================
    // SAVE GAME text
    // =========================================

    DrawTextEx(font, saveText, Vector2{saveButton.x + (saveButton.width - saveTextSize.x) / 2.0f,

                                       saveButton.y + (saveButton.height - saveTextSize.y) / 2.0f},
               fontSize,
               spacing,
               WHITE);
    // =========================================
    // GUIDE text
    // =========================================
    DrawTextEx(font, guideText, Vector2{guideButton.x + (guideButton.width - guideTextSize.x) / 2.0f,

                                        guideButton.y + (guideButton.height - guideTextSize.y) / 2.0f},
               fontSize,
               spacing,
               WHITE);
}

void GameScreen::drawGuidePopup()
{
    Font font = assets->getGuideFont();
    // =========================================
    // Dark overlay
    // =========================================
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{0, 0, 0, 120});
    // =========================================
    // Popup size
    // =========================================
    const float popupWidth = 900.0f;
    const float popupHeight = 730.0f;

    Rectangle popup{(GetScreenWidth() - popupWidth) / 2.0f,
                    (GetScreenHeight() - popupHeight) / 2.0f,
                    popupWidth,
                    popupHeight};

    // =========================================
    // Popup background
    // =========================================

    DrawRectangleRounded(popup, 0.04f, 20, Color{25, 20, 18, 245});

    // =========================================
    // Popup border
    // =========================================

    DrawRectangleRoundedLines(popup, 0.04f, 20, Color{180, 160, 130, 255});

    // =========================================
    // Title
    // =========================================

    const char *title = "INSTRUCTIONS";
    const float titleSize = 38.0f;

    Vector2 titleSizeVec = MeasureTextEx(font, title, titleSize, 2.0f);

    DrawTextEx(font, title, Vector2{popup.x + (popup.width - titleSizeVec.x) / 2.0f, popup.y + 25.0f},
               titleSize,
               2.0f,
               WHITE);

    // =========================================
    // Guide text
    // =========================================

    const float textSize = 25.0f;
    const float spacing = 1.5f;

    float x = popup.x + 45.0f;
    float y = popup.y + 100.0f;

    const float lineHeight = 32.0f;

    // =========================================
    // ACTIONS
    // =========================================
    DrawTextEx(font, "[ ACTIONS ]", Vector2{x, y}, textSize, spacing, WHITE);
    y += lineHeight;

    DrawTextEx(font, "Attack   ->>  Attack an enemy fighter.", Vector2{x, y}, textSize,
               spacing,
               WHITE);
    y += lineHeight;

    DrawTextEx(font, "Maneuver ->>  Move a fighter and draw a card.", Vector2{x, y},
               textSize,
               spacing,
               WHITE);
    y += lineHeight;

    DrawTextEx(
        font,
        "Scheme   ->>  Play a Scheme card.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    // =========================================
    // RULES
    // =========================================

    y += lineHeight * 1.4f;

    DrawTextEx(
        font,
        "[ RULES ]",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "[*] The younger player chooses the fighter and steps",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "    onto the field first.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "[*] Each turn you have 2 actions.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "[*] Every Hero card carries a unique effect,",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "    and on top of that each Hero possesses a special",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "    ability of their own.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "[*] Your hand must never exceed 7 cards.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font, "[*] Fighters can move directly between the marked",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font, "    spaces, which are hidden passages.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "[*] Through tactical moves in each turn, your goal",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "    is clear : defeat the enemy Hero and claim VICTORY.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    
    // =========================================
    // BACK BUTTON
    // =========================================

    const float backWidth = 130.0f;
    const float backHeight = 42.0f;

    Rectangle backButton{
        (GetScreenWidth() - backWidth) / 2.0f,
        popup.y + popup.height - 65.0f,
        backWidth,
        backHeight};

    Vector2 mousePosition =
        GetMousePosition();

    bool hovered =
        CheckCollisionPointRec(
            mousePosition,
            backButton);

    Color backColor =
        hovered
            ? Color{75, 75, 75, 255}
            : Color{40, 40, 40, 255};

    DrawRectangleRounded(
        backButton,
        1.0,
        20,
        backColor);

    const char *backText = "BACK";

    const float backTextSize = 24.0f;

    Vector2 backTextMeasure =
        MeasureTextEx(
            font,
            backText,
            backTextSize,
            1.5f);

    DrawTextEx(
        font,
        backText,

        Vector2{
            backButton.x +
                (backButton.width -
                 backTextMeasure.x) /
                    2.0f,

            backButton.y +
                (backButton.height -
                 backTextMeasure.y) /
                    2.0f},

        backTextSize,
        1.5f,
        WHITE);
}

// ============================================================
// CALCULATE MAP TRANSFORMATION
// ============================================================

void GameScreen::calculateMapTransform(
    float &mapX,
    float &mapY,
    float &scale,
    float &mapWidth,
    float &mapHeight) const
{
    Texture2D map =
        assets->getGameMap();

    if (map.id == 0)
    {
        mapX = 0.0f;
        mapY = 0.0f;
        scale = 1.0f;
        mapWidth = 0.0f;
        mapHeight = 0.0f;
        return;
    }

    // =========================================
    // Map layout settings
    // =========================================

    const float panelWidth = 300.0f;
    const float mapPadding = 15.0f;
    const float topSpace = 75.0f;

    // =========================================
    // Available map area
    // =========================================

    const float mapAreaX =
        panelWidth + mapPadding;

    const float mapAreaWidth =
        GetScreenWidth() -
        2.0f * (panelWidth + mapPadding);

    const float mapAreaY =
        topSpace;

    const float mapAreaHeight =
        GetScreenHeight() -
        topSpace;

    // =========================================
    // Calculate scale
    // =========================================

    const float scaleX =
        mapAreaWidth /
        static_cast<float>(map.width);

    const float scaleY =
        mapAreaHeight /
        static_cast<float>(map.height);

    scale =
        std::min(scaleX, scaleY);

    // Slight margin around map
    scale *= 0.95f;

    // =========================================
    // Final map dimensions
    // =========================================

    mapWidth =
        static_cast<float>(map.width) *
        scale;

    mapHeight =
        static_cast<float>(map.height) *
        scale;

    // =========================================
    // Center map horizontally
    // =========================================

    mapX =
        mapAreaX +
        (mapAreaWidth - mapWidth) / 2.0f;

    // =========================================
    // Center map vertically
    // =========================================

    mapY =
        mapAreaY +
        (mapAreaHeight - mapHeight) / 2.0f;

    // =========================================
    // Move map slightly upward
    // =========================================

    mapY -= 100.0f;
}

int GameScreen::getClickedSpaceId()
{
    if (assets == nullptr)
    {
        return -1;
    }

    Texture2D map =
        assets->getGameMap();

    if (map.id == 0)
    {
        return -1;
    }

    // =========================================
    // Mouse position
    // =========================================

    Vector2 mouse =
        GetMousePosition();

    // =========================================
    // Map transformation
    // =========================================

    float mapX;
    float mapY;
    float scale;
    float mapWidth;
    float mapHeight;

    calculateMapTransform(
        mapX,
        mapY,
        scale,
        mapWidth,
        mapHeight);

    if (scale <= 0.0f)
    {
        return -1;
    }

    // =========================================
    // Screen -> Original map image
    // =========================================

    float imageX =
        (mouse.x - mapX) / scale;

    float imageY =
        (mouse.y - mapY) / scale;

    // =========================================
    // Outside map
    // =========================================

    if (imageX < 0.0f ||
        imageY < 0.0f ||
        imageX > static_cast<float>(map.width) ||
        imageY > static_cast<float>(map.height))
    {
        return -1;
    }

    // =========================================
    // Check all 32 spaces
    // =========================================

    for (int i = 0; i < 32; i++)
    {
        const SpaceGraphic &space =
            SPACE_GRAPHICS[i];

        float dx =
            imageX - space.center.x;

        float dy =
            imageY - space.center.y;

        float distanceSquared =
            dx * dx + dy * dy;

        float radiusSquared =
            space.radius * space.radius;

        if (distanceSquared <= radiusSquared)
        {
            return i + 1;
        }
    }

    return -1;
}

Vector2 GameScreen::mapImageToScreen(
    Vector2 imagePosition) const
{
    float mapX;
    float mapY;
    float scale;
    float mapWidth;
    float mapHeight;

    calculateMapTransform(
        mapX,
        mapY,
        scale,
        mapWidth,
        mapHeight);

    return Vector2{
        mapX +
            imagePosition.x * scale,

        mapY +
            imagePosition.y * scale};
}

void GameScreen::drawSpaces()
{
    if (assets == nullptr)
    {
        return;
    }

    Font font = assets->getGameFont();

    float mapX;
    float mapY;
    float scale;
    float mapWidth;
    float mapHeight;

    calculateMapTransform(
        mapX,
        mapY,
        scale,
        mapWidth,
        mapHeight);

    for (int i = 0; i < 32; i++)
    {
        Vector2 center =
            mapImageToScreen(
                SPACE_GRAPHICS[i].center);

        float radius =
            SPACE_GRAPHICS[i].radius * scale;

        DrawCircleV(
            center,
            radius,
            Color{255, 255, 255, 70});

        std::string number =
            std::to_string(i + 1);

        Vector2 textSize =
            MeasureTextEx(
                font,
                number.c_str(),
                30.0f,
                1.0f);

        DrawTextEx(
            font,
            number.c_str(),

            Vector2{
                center.x - textSize.x / 2.0f,
                center.y - textSize.y / 2.0f},

            30.0f,
            1.0f,
            WHITE);
    }
}

void GameScreen::drawPlacedFighters()
{
    if (game == nullptr || assets == nullptr)
    {
        return;
    }

    Font font = assets->getGameFont();
    const std::vector<Player *> &players = game->getPlayers();

    for (Player *player : players)
    {
        if (player == nullptr)
        {
            continue;
        }

        // =====================================
        // HERO
        // =====================================

        Hero *hero = player->getHero();
        if (hero != nullptr)
        {
            Space *space = hero->getPosition();

            if (space != nullptr)
            {
                int spaceId = space->getId();

                if (spaceId >= 1 && spaceId <= 32)
                {
                    Vector2 center = mapImageToScreen(SPACE_GRAPHICS[spaceId - 1].center);
                    Color heroColor = Color{180, 40, 40, 230};

                    if (dynamic_cast<Sherlock *>(hero) != nullptr)
                    {
                        heroColor = Color{115, 70, 35, 230};
                    }
                    else if (dynamic_cast<Dracula *>(hero) != nullptr)
                    {
                        heroColor = Color{120, 20, 40, 230};
                    }
                    else if (dynamic_cast<InvisibleMan *>(hero) != nullptr)
                    {
                        heroColor = Color{90, 100, 105, 230};
                    }

                    DrawCircleV(center, 22.0f, heroColor);
                    std::string heroName = hero->getName();
                    Vector2 textSize = MeasureTextEx(font, heroName.c_str(), 17.0f, 1.0f);

                    DrawTextEx(font, heroName.c_str(), Vector2{center.x - textSize.x / 2.0f, center.y - 35.0f},
                               17.0f,
                               1.0f,
                               WHITE);
                }
            }
        }

        // =====================================
        // SIDEKICKS
        // =====================================

        std::vector<Sidekick *> sidekicks = player->getSideKicks();
        for (Sidekick *sidekick : sidekicks)
        {
            if (sidekick == nullptr)
            {
                continue;
            }

            Space *space = sidekick->getPosition();
            if (space == nullptr)
            {
                continue;
            }

            int spaceId = space->getId();
            if (spaceId < 1 || spaceId > 32)
            {
                continue;
            }

            Vector2 center = mapImageToScreen(SPACE_GRAPHICS[spaceId - 1].center);
            Color sidekickColor = Color{60, 120, 200, 230};
            if (dynamic_cast<Watson *>(sidekick) != nullptr)
            {
                sidekickColor = Color{205, 165, 55, 230};
            }
            else if (dynamic_cast<Sisters *>(sidekick) != nullptr)
            {
                sidekickColor = Color{28, 28, 30, 230};
            }

            DrawCircleV(center, 15.0f, sidekickColor);
            std::string sidekickName = sidekick->getName();

            Vector2 textSize = MeasureTextEx(font, sidekickName.c_str(), 16.0f, 1.0f);

            DrawTextEx(font, sidekickName.c_str(), Vector2{center.x - textSize.x / 2.0f, center.y - 28.0f},
                       16.0f,
                       1.0f,
                       WHITE);
        }
    }
}

void GameScreen::drawFogs()
{
    if (game == nullptr ||
        assets == nullptr)
    {
        return;
    }

    Texture2D fogTexture =
        assets->getFogTexture();

    if (fogTexture.id == 0)
    {
        return;
    }

    float mapX;
    float mapY;
    float scale;
    float mapWidth;
    float mapHeight;

    calculateMapTransform(
        mapX,
        mapY,
        scale,
        mapWidth,
        mapHeight);

    const std::vector<Player *> &players =
        game->getPlayers();

    for (Player *player : players)
    {
        if (player == nullptr)
        {
            continue;
        }

        const std::vector<Fog *> &fogs =
            player->getFogs();

        for (Fog *fog : fogs)
        {
            if (fog == nullptr)
            {
                continue;
            }

            Space *space =
                fog->getPosition();

            if (space == nullptr)
            {
                continue;
            }

            int spaceId =
                space->getId();

            if (spaceId < 1 ||
                spaceId > 32)
            {
                continue;
            }

            // =====================================
            // SPACE CENTER
            // =====================================

            Vector2 center =
                mapImageToScreen(
                    SPACE_GRAPHICS[spaceId - 1].center);

            // =====================================
            // FOG SIZE
            // =====================================

            const float fogImageSize = 140.0f;

            const float fogSize =
                fogImageSize * scale;

            Rectangle source{
                0.0f,
                0.0f,
                static_cast<float>(
                    fogTexture.width),
                static_cast<float>(
                    fogTexture.height)};

            Rectangle destination{
                center.x - fogSize / 2.0f,
                center.y + 30.0f - fogSize / 2.0f,
                fogSize,
                fogSize};

            DrawTexturePro(
                fogTexture,
                source,
                destination,
                Vector2{0.0f, 0.0f},
                0.0f,
                WHITE);
        }
    }
}

void GameScreen::drawActionButtons()
{
    if (assets == nullptr)
    {
        return;
    }

    Font font =
        assets->getGameFont();

    // =========================================
    // BUTTON SIZE
    // =========================================

    const float buttonWidth = 180.0f;
    const float buttonHeight = 55.0f;
    const float gap = 15.0f;

    // =========================================
    // POSITION
    // =========================================

    const float totalWidth =
        3.0f * buttonWidth +
        2.0f * gap;

    const float startX =
        (GetScreenWidth() - totalWidth) / 2.0f;

    const float buttonY =
        GetScreenHeight() - 160.0f;

    // =========================================
    // BUTTON RECTANGLES
    // =========================================

    attackButton = Rectangle{
        startX,
        buttonY,
        buttonWidth,
        buttonHeight};

    maneuverButton = Rectangle{
        startX +
            buttonWidth +
            gap,
        buttonY,
        buttonWidth,
        buttonHeight};

    schemeButton = Rectangle{
        startX +
            2.0f *
                (buttonWidth + gap),
        buttonY,
        buttonWidth,
        buttonHeight};

    // =========================================
    // MOUSE
    // =========================================

    Vector2 mousePosition =
        GetMousePosition();

    // =========================================
    // COLORS
    // =========================================

    Color normalColor{
        25,
        25,
        25,
        220};

    Color hoverColor{
        70,
        70,
        70,
        230};

    Color selectedColor{
        120,
        85,
        40,
        240};

    // =========================================
    // ATTACK COLOR
    // =========================================

    Color attackColor;

    if (selectedAction == ActionChoice::ATTACK)
    {
        attackColor = selectedColor;
    }
    else if (CheckCollisionPointRec(
                 mousePosition,
                 attackButton))
    {
        attackColor = hoverColor;
    }
    else
    {
        attackColor = normalColor;
    }

    // =========================================
    // MANEUVER COLOR
    // =========================================

    Color maneuverColor;

    if (selectedAction == ActionChoice::MANEUVER)
    {
        maneuverColor = selectedColor;
    }
    else if (CheckCollisionPointRec(
                 mousePosition,
                 maneuverButton))
    {
        maneuverColor = hoverColor;
    }
    else
    {
        maneuverColor = normalColor;
    }

    // =========================================
    // SCHEME COLOR
    // =========================================

    Color schemeColor;

    if (selectedAction == ActionChoice::SCHEME)
    {
        schemeColor = selectedColor;
    }
    else if (CheckCollisionPointRec(
                 mousePosition,
                 schemeButton))
    {
        schemeColor = hoverColor;
    }
    else
    {
        schemeColor = normalColor;
    }

    // =========================================
    // DRAW BUTTONS
    // =========================================

    DrawRectangleRounded(
        attackButton,
        1.0f,
        20,
        attackColor);

    DrawRectangleRounded(
        maneuverButton,
        1.0f,
        20,
        maneuverColor);

    DrawRectangleRounded(
        schemeButton,
        1.0f,
        20,
        schemeColor);

    // =========================================
    // TEXT
    // =========================================

    const float fontSize = 25.0f;
    const float spacing = 1.5f;

    const char *attackText =
        "ATTACK";

    const char *maneuverText =
        "MANEUVER";

    const char *schemeText =
        "SCHEME";

    // =========================================
    // ATTACK TEXT
    // =========================================

    Vector2 attackTextSize =
        MeasureTextEx(
            font,
            attackText,
            fontSize,
            spacing);
    DrawTextEx(
        font,
        attackText,

        Vector2{
            attackButton.x +
                (attackButton.width -
                 attackTextSize.x) /
                    2.0f,

            attackButton.y +
                (attackButton.height -
                 attackTextSize.y) /
                    2.0f},

        fontSize,
        spacing,
        WHITE);

    // =========================================
    // MANEUVER TEXT
    // =========================================

    Vector2 maneuverTextSize =
        MeasureTextEx(
            font,
            maneuverText,
            fontSize,
            spacing);

    DrawTextEx(
        font,
        maneuverText,

        Vector2{
            maneuverButton.x +
                (maneuverButton.width -
                 maneuverTextSize.x) /
                    2.0f,

            maneuverButton.y +
                (maneuverButton.height -
                 maneuverTextSize.y) /
                    2.0f},

        fontSize,
        spacing,
        WHITE);

    // =========================================
    // SCHEME TEXT
    // =========================================

    Vector2 schemeTextSize =
        MeasureTextEx(
            font,
            schemeText,
            fontSize,
            spacing);

    DrawTextEx(
        font,
        schemeText,

        Vector2{
            schemeButton.x +
                (schemeButton.width -
                 schemeTextSize.x) /
                    2.0f,

            schemeButton.y +
                (schemeButton.height -
                 schemeTextSize.y) /
                    2.0f},

        fontSize,
        spacing,
        WHITE);
}

Fighter *GameScreen::getClickedFighter()
{
    if (game == nullptr)
    {
        return nullptr;
    }

    Vector2 mouse = GetMousePosition();

    const std::vector<Player *> &players =
        game->getPlayers();

    for (Player *player : players)
    {
        if (player == nullptr)
        {
            continue;
        }

        // =====================================
        // HERO
        // =====================================

        Hero *hero = player->getHero();

        if (hero != nullptr && hero->isAlive())
        {
            Space *space = hero->getPosition();

            if (space != nullptr)
            {
                int spaceId = space->getId();

                if (spaceId >= 1 && spaceId <= 32)
                {
                    Vector2 center =
                        mapImageToScreen(
                            SPACE_GRAPHICS[spaceId - 1].center);

                    float distance =
                        std::sqrt(
                            (mouse.x - center.x) *
                                (mouse.x - center.x) +
                            (mouse.y - center.y) *
                                (mouse.y - center.y));

                    if (distance <= 25.0f)
                    {
                        return hero;
                    }
                }
            }
        }

        // =====================================
        // SIDEKICKS
        // =====================================

        for (Sidekick *sidekick : player->getSideKicks())
        {
            if (sidekick == nullptr ||
                !sidekick->isAlive())
            {
                continue;
            }

            Space *space = sidekick->getPosition();

            if (space == nullptr)
            {
                continue;
            }

            int spaceId = space->getId();

            if (spaceId < 1 || spaceId > 32)
            {
                continue;
            }

            Vector2 center =
                mapImageToScreen(
                    SPACE_GRAPHICS[spaceId - 1].center);

            float distance =
                std::sqrt(
                    (mouse.x - center.x) *
                        (mouse.x - center.x) +
                    (mouse.y - center.y) *
                        (mouse.y - center.y));

            if (distance <= 20.0f)
            {
                return sidekick;
            }
        }
    }

    return nullptr;
}

void GameScreen::drawTurnIndicator()
{
    if (assets == nullptr || game == nullptr)
    {
        return;
    }

    Player *currentPlayer =
        game->getTurnManager().getCurrentPlayer();

    if (currentPlayer == nullptr)
    {
        return;
    }

    Hero *hero = currentPlayer->getHero();

    if (hero == nullptr)
    {
        return;
    }

    Font font = assets->getGameFont();

    // =========================================
    // لبه‌ی پایین نقشه
    // =========================================

    float mapX;
    float mapY;
    float scale;
    float mapWidth;
    float mapHeight;

    calculateMapTransform(
        mapX,
        mapY,
        scale,
        mapWidth,
        mapHeight);

    std::string text = "> Turn : " + hero->getName();

    const float fontSize = 28.0f;
    const float spacing = 2.0f;

    Vector2 textSize =
        MeasureTextEx(font, text.c_str(), fontSize, spacing);

    float textX =
        (GetScreenWidth() - textSize.x) / 2.0f;

    float textY =
        mapY + mapHeight + 12.0f; // فاصله‌ی کم از لبه‌ی نقشه

    // پس‌زمینه‌ی کوچیک برای خوانایی بهتر
    Rectangle background{
        textX - 15.0f,
        textY - 6.0f,
        textSize.x + 30.0f,
        textSize.y + 12.0f};

    DrawRectangleRounded(
        background,
        1.0f,
        16,
        Color{20, 20, 20, 190});

    DrawTextEx(
        font,
        text.c_str(),
        Vector2{textX, textY},
        fontSize,
        spacing,
        WHITE);
}

void GameScreen::checkAndEndTurnIfNeeded()
{
    if (game == nullptr)
    {
        return;
    }

    TurnManager &turnManager = game->getTurnManager();

    if (turnManager.hasActions())
    {
        return;
    }

    turnManager.endTurn();

    Player *newCurrentPlayer = turnManager.getCurrentPlayer();

    if (newCurrentPlayer != nullptr &&
        newCurrentPlayer->getHero() != nullptr)
    {
        std::cout
            << "[.] Turn ended. Next turn: "
            << newCurrentPlayer->getHero()->getName()
            << std::endl;
    }
}

void GameScreen::drawManeuverMovableSpaces()
{
    if (!maneuverUI.isSelectingSpace())
    {
        return;
    }

    float mapX;
    float mapY;
    float scale;
    float mapWidth;
    float mapHeight;

    calculateMapTransform(
        mapX,
        mapY,
        scale,
        mapWidth,
        mapHeight);

    for (Space *space : maneuverUI.getAvailableMoves())
    {
        if (space == nullptr)
        {
            continue;
        }

        int spaceId = space->getId();

        if (spaceId < 1 || spaceId > 32)
        {
            continue;
        }

        Vector2 center =
            mapImageToScreen(
                SPACE_GRAPHICS[spaceId - 1].center);

        float radius =
            SPACE_GRAPHICS[spaceId - 1].radius * scale;

        DrawCircleV(
            center,
            radius,
            Color{80, 220, 100, 130});

        DrawCircleLinesV(
            center,
            radius,
            Color{120, 255, 150, 220});
    }
}

void GameScreen::finalizeSchemeCard(Card *playedCard)
{
    Player *currentPlayer = game->getTurnManager().getCurrentPlayer();

    if (currentPlayer == nullptr || playedCard == nullptr)
    {
        return;
    }

    Hand &hand = currentPlayer->getHand();
    const std::vector<Card *> &cards = hand.getCards();

    for (int i = 0; i < static_cast<int>(cards.size()); i++)
    {
        if (cards[i] == playedCard)
        {
            Card *removed = hand.removeCard(i);
            currentPlayer->getDiscardPile().addCard(removed);
            break;
        }
    }
}

void GameScreen::drawEffectSelectableSpaces()
{
    if (!effectUI.isChoosingSpace())
    {
        return;
    }

    float mapX, mapY, scale, mapWidth, mapHeight;
    calculateMapTransform(mapX, mapY, scale, mapWidth, mapHeight);

    for (Space *space : effectUI.getCandidateSpaces())
    {
        int spaceId = space->getId();

        if (spaceId < 1 || spaceId > 32)
        {
            continue;
        }

        Vector2 center = mapImageToScreen(SPACE_GRAPHICS[spaceId - 1].center);
        float radius = SPACE_GRAPHICS[spaceId - 1].radius * scale;

        DrawCircleV(center, radius, Color{255, 200, 60, 130});
        DrawCircleLinesV(center, radius, Color{255, 220, 100, 220});
    }
}

void GameScreen::drawLookButton()
{
    if (!combatShowLookButton || assets == nullptr)
    {
        return;
    }

    Font font = assets->getGameFont();

    float mapX, mapY, scale, mapWidth, mapHeight;
    calculateMapTransform(mapX, mapY, scale, mapWidth, mapHeight);

    const float buttonWidth = 160.0f;
    const float buttonHeight = 50.0f;

    lookButton = Rectangle{
        (GetScreenWidth() - buttonWidth) / 2.0f,
        mapY + mapHeight + 110.0f, // کمی پایین‌تر از نوار "Turn"
        buttonWidth,
        buttonHeight};

    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, lookButton);

    Color buttonColor = hovered ? Color{75, 75, 75, 245} : Color{35, 35, 35, 235};

    DrawRectangleRounded(lookButton, 1.0f, 20, buttonColor);
    DrawRectangleRoundedLines(lookButton, 1.0f, 20, hovered ? WHITE : Color{150, 150, 150, 255});

    const char *text = "LOOK";
    const float fontSize = 24.0f;

    Vector2 textSize = MeasureTextEx(font, text, fontSize, 1.5f);

    DrawTextEx(
        font, text,
        Vector2{
            lookButton.x + (lookButton.width - textSize.x) / 2.0f,
            lookButton.y + (lookButton.height - textSize.y) / 2.0f},

        fontSize,
        1.5f, WHITE);
}

void GameScreen::drawCombatEffectText()
{
    if (!combatInProgress || assets == nullptr || game == nullptr)
    {
        return;
    }

    const std::string &description = game->getCombatSystem().getCurrentEffectDescription();

    if (description.empty())
    {
        return;
    }

    Font font = assets->getGameFont();

    float mapX, mapY, scale, mapWidth, mapHeight;
    calculateMapTransform(mapX, mapY, scale, mapWidth, mapHeight);

    const float fontSize = 22.0f;
    const float spacing = 1.5f;
    const float maxWidth = mapWidth - 40.0f;

    std::vector<std::string> lines;
    std::string currentLine;
    std::string word;

    std::istringstream stream(description);
    while (stream >> word)
    {
        std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
        Vector2 testSize = MeasureTextEx(font, testLine.c_str(), fontSize, spacing);

        if (testSize.x > maxWidth && !currentLine.empty())
        {
            lines.push_back(currentLine);
            currentLine = word;
        }
        else
        {
            currentLine = testLine;
        }
    }
    if (!currentLine.empty())
    {
        lines.push_back(currentLine);
    }

    const float lineHeight = 28.0f;
    float textY = mapY + mapHeight + 55.0f;
    float blockHeight = lines.size() * lineHeight + 16.0f;

    Rectangle background{
        mapX + 20.0f,
        textY - 8.0f,
        maxWidth,
        blockHeight};

    DrawRectangleRounded(background, 0.15f, 12, Color{20, 20, 20, 190});

    for (size_t i = 0; i < lines.size(); i++)
    {
        Vector2 lineSize = MeasureTextEx(font, lines[i].c_str(), fontSize, spacing);

        DrawTextEx(
            font, lines[i].c_str(),
            Vector2{
                mapX + 20.0f + (maxWidth - lineSize.x) / 2.0f,
                textY + i * lineHeight},
            fontSize, spacing, WHITE);
    }
}

void GameScreen::drawResultRevealButton()
{
    if (!combatInProgress || assets == nullptr || game == nullptr)
    {
        return;
    }

    if (!game->getCombatSystem().isAwaitingResultReveal())
    {
        return;
    }

    if (combatResultPopupOpen)
    {
        return;
    }

    Font font = assets->getGameFont();

    float mapX, mapY, scale, mapWidth, mapHeight;
    calculateMapTransform(mapX, mapY, scale, mapWidth, mapHeight);

    const float buttonWidth = 220.0f;
    const float buttonHeight = 50.0f;

    resultRevealButton = Rectangle{
        (GetScreenWidth() - buttonWidth) / 2.0f,
        mapY + mapHeight + 80.0f,
        buttonWidth,
        buttonHeight};

    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, resultRevealButton);

    Color buttonColor = hovered ? Color{75, 75, 75, 245} : Color{35, 35, 35, 235};

    DrawRectangleRounded(resultRevealButton, 1.0f, 20, buttonColor);
    DrawRectangleRoundedLines(resultRevealButton, 1.0f, 20, hovered ? WHITE : Color{150, 150, 150, 255});

    const char *text = "SHOW RESULT";
    const float fontSize = 22.0f;

    Vector2 textSize = MeasureTextEx(font, text, fontSize, 1.5f);

    DrawTextEx(
        font, text,
        Vector2{
            resultRevealButton.x + (resultRevealButton.width - textSize.x) / 2.0f,
            resultRevealButton.y + (resultRevealButton.height - textSize.y) / 2.0f},
        fontSize, 1.5f, WHITE);
}

void GameScreen::drawCombatResultPopup()
{
    if (!combatResultPopupOpen || assets == nullptr || game == nullptr)
    {
        return;
    }

    Font font = assets->getGameFont();
    CombatSystem &combatSystem = game->getCombatSystem();

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{0, 0, 0, 190});

    const float popupWidth = 500.0f;
    const float popupHeight = 350.0f;

    Rectangle popup{
        (GetScreenWidth() - popupWidth) / 2.0f,
        (GetScreenHeight() - popupHeight) / 2.0f,
        popupWidth,
        popupHeight};

    DrawRectangleRounded(popup, 0.06f, 20, Color{25, 20, 18, 245});
    DrawRectangleRoundedLines(popup, 0.06f, 20, Color{180, 160, 130, 255});

    const char *title = "COMBAT RESULT";
    const float titleSize = 32.0f;

    Vector2 titleTextSize = MeasureTextEx(font, title, titleSize, 2.0f);

    DrawTextEx(
        font, title,
        Vector2{popup.x + (popup.width - titleTextSize.x) / 2.0f, popup.y + 25.0f},
        titleSize, 2.0f, WHITE);

    Fighter *attackerFighter = combatSystem.getAttacker();
    Fighter *defenderFighter = combatSystem.getDefender();

    int attackValue = combatSystem.getPendingAttackValue();
    int defenceValue = combatSystem.getPendingDefenceValue();
    int damage = combatSystem.getLastDamage();
    bool attackerWon = combatSystem.didAttackerWin();

    std::string attackLine = "Attack Value : " + std::to_string(attackValue);
    std::string defenceLine = "Defense Value : " + std::to_string(defenceValue);
    std::string damageLine =
        (defenderFighter != nullptr ? defenderFighter->getName() : "Defender") +
        std::string(" took ") + std::to_string(damage) + " damage";
    std::string winnerLine =
        attackerWon
            ? ((attackerFighter != nullptr ? attackerFighter->getName() : "Attacker") + std::string(" WINS the combat!"))
            : ((defenderFighter != nullptr ? defenderFighter->getName() : "Defender") + std::string(" WINS the combat!"));

    const float lineFontSize = 24.0f;
    const float lineSpacing = 1.5f;
    const float lineHeight = 42.0f;

    float lineY = popup.y + 100.0f;
    float lineX = popup.x + 40.0f;

    DrawTextEx(font, attackLine.c_str(), Vector2{lineX, lineY}, lineFontSize, lineSpacing, WHITE);
    lineY += lineHeight;

    DrawTextEx(font, defenceLine.c_str(), Vector2{lineX, lineY}, lineFontSize, lineSpacing, WHITE);
    lineY += lineHeight;

    DrawTextEx(font, damageLine.c_str(), Vector2{lineX, lineY}, lineFontSize, lineSpacing, WHITE);
    lineY += lineHeight;

    DrawTextEx(font, winnerLine.c_str(), Vector2{lineX, lineY}, lineFontSize, lineSpacing,
               attackerWon ? Color{120, 220, 120, 255} : Color{220, 120, 120, 255});

    const float backWidth = 160.0f;
    const float backHeight = 50.0f;

    resultBackButton = Rectangle{
        popup.x + (popup.width - backWidth) / 2.0f,
        popup.y + popup.height - 70.0f,
        backWidth,
        backHeight};

    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, resultBackButton);

    Color backColor = hovered ? Color{75, 75, 75, 255} : Color{40, 40, 40, 255};

    DrawRectangleRounded(resultBackButton, 1.0f, 20, backColor);

    const char *backText = "BACK";
    Vector2 backTextSize = MeasureTextEx(font, backText, 24.0f, 1.5f);

    DrawTextEx(
        font, backText,
        Vector2{
            resultBackButton.x + (resultBackButton.width - backTextSize.x) / 2.0f,
            resultBackButton.y + (resultBackButton.height - backTextSize.y) / 2.0f},
        24.0f, 1.5f, WHITE);
}

void GameScreen::drawDeckEmptyPopup()
{
    if (!deckEmptyPopupOpen || assets == nullptr)
    {
        return;
    }

    Font font = assets->getGameFont();

    const char *message = "DECK IS EMPTY :< YOUR FIGHTERS TAKE 2 DAMAGE.";
    const float fontSize = 24.0f;
    const float spacing = 1.5f;

    Vector2 textSize = MeasureTextEx(font, message, fontSize, spacing);

    const float buttonWidth = 140.0f;
    const float buttonHeight = 48.0f;

    // فاصله از پایین صفحه، درست جایی که قبلاً دکمه‌های اکشن بودن
    float textY = GetScreenHeight() - 130.0f;
    float textX = (GetScreenWidth() - textSize.x) / 2.0f;

    Rectangle background{
        textX - 20.0f,
        textY - 10.0f,
        textSize.x + 40.0f,
        textSize.y + 20.0f};

    DrawRectangleRounded(background, 0.2f, 12, Color{80, 20, 20, 220});
    DrawRectangleRoundedLines(background, 0.2f, 12, Color{220, 100, 100, 255});

    DrawTextEx(font, message, Vector2{textX, textY}, fontSize, spacing, WHITE);

    deckEmptyOkButton = Rectangle{
        (GetScreenWidth() - buttonWidth) / 2.0f,
        textY + textSize.y + 25.0f,
        buttonWidth,
        buttonHeight};

    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, deckEmptyOkButton);

    Color buttonColor = hovered ? Color{75, 75, 75, 245} : Color{35, 35, 35, 235};

    DrawRectangleRounded(deckEmptyOkButton, 1.0f, 20, buttonColor);
    DrawRectangleRoundedLines(deckEmptyOkButton, 1.0f, 20, hovered ? WHITE : Color{150, 150, 150, 255});

    const char *okText = "OK";
    Vector2 okTextSize = MeasureTextEx(font, okText, 22.0f, 1.5f);

    DrawTextEx(
        font, okText,
        Vector2{
            deckEmptyOkButton.x + (deckEmptyOkButton.width - okTextSize.x) / 2.0f,
            deckEmptyOkButton.y + (deckEmptyOkButton.height - okTextSize.y) / 2.0f},
        22.0f, 1.5f, WHITE);
}

void GameScreen::openHandLimitPopup(Player *player)
{
    handLimitPlayer = player;
    handLimitCards.clear();
    handLimitCardBoxes.clear();
    handLimitSelectedIndex = -1;

    if (player == nullptr)
    {
        handLimitPopupOpen = false;
        return;
    }

    for (Card *card : player->getHand().getCards())
    {
        if (card != nullptr)
        {
            handLimitCards.push_back(card);
        }
    }

    const int maxPerRow = 4;
    const float boxWidth = 200.0f;
    const float boxHeight = 290.0f;
    const float gapX = 20.0f;
    const float gapY = 20.0f;
    const float startY = 100.0f;

    int total = static_cast<int>(handLimitCards.size());
    int rowCount = (total + maxPerRow - 1) / maxPerRow;
    int index = 0;

    for (int row = 0; row < rowCount; row++)
    {
        int cardsInRow = std::min(maxPerRow, total - index);

        float rowWidth = cardsInRow * boxWidth + (cardsInRow - 1) * gapX;
        float rowStartX = (GetScreenWidth() - rowWidth) / 2.0f;
        float rowY = startY + row * (boxHeight + gapY);

        for (int col = 0; col < cardsInRow; col++)
        {
            Rectangle box{
                rowStartX + col * (boxWidth + gapX),
                rowY,
                boxWidth,
                boxHeight};

            handLimitCardBoxes.push_back(box);
            index++;
        }
    }

    const float burnWidth = 200.0f;
    const float burnHeight = 55.0f;
    float lastRowBottom = startY + rowCount * (boxHeight + gapY);

    handLimitBurnButton = Rectangle{
        (GetScreenWidth() - burnWidth) / 2.0f,
        lastRowBottom + 15.0f,
        burnWidth,
        burnHeight};

    handLimitPopupOpen = true;
}

void GameScreen::checkHandLimit()
{
    if (handLimitPopupOpen || deckEmptyPopupOpen || game == nullptr)
    {
        return;
    }

    const std::vector<Player *> &players = game->getPlayers();

    for (Player *player : players)
    {
        if (player != nullptr && player->getHand().getSize() > 7)
        {
            openHandLimitPopup(player);
            return;
        }
    }
}

void GameScreen::drawHandLimitPopup()
{
    if (!handLimitPopupOpen || assets == nullptr)
    {
        return;
    }

    Font font = assets->getGameFont();
    Vector2 mouse = GetMousePosition();

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{0, 0, 0, 190});

    const char *title = "HAND LIMIT EXCEEDED - CHOOSE A CARD TO DISCARD";
    const float titleSize = 30.0f;

    Vector2 titleTextSize = MeasureTextEx(font, title, titleSize, 2.0f);

    DrawTextEx(
        font, title,
        Vector2{(GetScreenWidth() - titleTextSize.x) / 2.0f, 40.0f},
        titleSize, 2.0f, WHITE);

    std::string heroName =
        (handLimitPlayer != nullptr && handLimitPlayer->getHero() != nullptr)
            ? handLimitPlayer->getHero()->getName()
            : "";

    for (size_t i = 0; i < handLimitCards.size(); i++)
    {
        Card *card = handLimitCards[i];
        Rectangle box = handLimitCardBoxes[i];

        bool hovered = CheckCollisionPointRec(mouse, box);
        bool selected = (static_cast<int>(i) == handLimitSelectedIndex);

        Color boxColor;
        if (selected)
            boxColor = Color{120, 85, 40, 245};
        else if (hovered)
            boxColor = Color{75, 75, 75, 245};
        else
            boxColor = Color{35, 35, 35, 235};

        DrawRectangleRounded(box, 0.08f, 20, boxColor);
        DrawRectangleRoundedLines(box, 0.08f, 20, (hovered || selected) ? WHITE : Color{150, 150, 150, 255});

        std::string textureKey = getCardTextureKeyForHandLimit(card, heroName);

        if (!textureKey.empty())
        {
            Texture2D texture = assets->getCard(textureKey);

            if (texture.id != 0)
            {
                const float padding = 12.0f;

                Rectangle source{0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height)};
                Rectangle destination{box.x + padding, box.y + padding, box.width - 2.0f * padding, box.height - 60.0f};

                DrawTexturePro(texture, source, destination, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
            }
        }

        std::string name = card->getName();
        Vector2 nameSize = MeasureTextEx(font, name.c_str(), 18.0f, 1.0f);

        DrawTextEx(
            font, name.c_str(),
            Vector2{box.x + (box.width - nameSize.x) / 2.0f, box.y + box.height - 35.0f},
            18.0f, 1.0f, WHITE);
    }

    bool burnEnabled = (handLimitSelectedIndex != -1);
    bool burnHovered = CheckCollisionPointRec(mouse, handLimitBurnButton);

    Color burnColor;
    if (!burnEnabled)
        burnColor = Color{30, 30, 30, 120};
    else if (burnHovered)
        burnColor = Color{140, 40, 40, 245};
    else
        burnColor = Color{100, 30, 30, 235};

    DrawRectangleRounded(handLimitBurnButton, 1.0f, 20, burnColor);

    const char *burnText = "BURN";
    Vector2 burnTextSize = MeasureTextEx(font, burnText, 26.0f, 1.5f);

    DrawTextEx(
        font, burnText,
        Vector2{
            handLimitBurnButton.x + (handLimitBurnButton.width - burnTextSize.x) / 2.0f,
            handLimitBurnButton.y + (handLimitBurnButton.height - burnTextSize.y) / 2.0f},
        26.0f, 1.5f,
        burnEnabled ? WHITE : Color{150, 150, 150, 150});
}

void GameScreen::refreshSaveFiles()
{
    saveFiles = saveManager.getSaveFiles();
}

void GameScreen::updateSaveMenu()
{
    if (!saveMenuOpen)
    {
        return;
    }

    Vector2 mousePosition = GetMousePosition();

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        return;
    }

    // =========================================
    // Existing Save Files
    // =========================================

    for (size_t i = 0; i < saveButtons.size(); i++)
    {
        if (CheckCollisionPointRec(mousePosition, saveButtons[i]))
        {
            try
            {
                // انتخاب Save قبلی = overwrite
                saveManager.saveGame(game, saveFiles[i]);

                std::cout
                    << "\n[+] Save overwritten: "
                    << saveFiles[i]
                    << std::endl;

                saveMenuOpen = false;
            }
            catch (const std::exception &e)
            {
                std::cerr
                    << "\n[!] Save failed: "
                    << e.what()
                    << std::endl;
            }

            return;
        }
    }

    // =========================================
    // NEW SAVE
    // =========================================

    if (CheckCollisionPointRec(mousePosition, newSaveButton))
    {
        try
        {
            std::string newSaveName =
                saveManager.createNewSaveName();

            saveManager.saveGame(game, newSaveName);

            std::cout
                << "\n[+] New save created: "
                << newSaveName
                << std::endl;

            saveMenuOpen = false;
        }
        catch (const std::exception &e)
        {
            std::cerr
                << "\n[!] Save failed: "
                << e.what()
                << std::endl;
        }

        return;
    }

    // =========================================
    // BACK
    // =========================================

    if (CheckCollisionPointRec(mousePosition, saveMenuBackButton))
    {
        saveMenuOpen = false;
        return;
    }
}

void GameScreen::drawSaveMenu()
{
    if (!saveMenuOpen || assets == nullptr)
    {
        return;
    }

    Font font = assets->getGameFont();

    // =========================================
    // Dark Overlay
    // =========================================

    DrawRectangle(
        0,
        0,
        GetScreenWidth(),
        GetScreenHeight(),
        Color{0, 0, 0, 190});

    // =========================================
    // Popup
    // =========================================

    const float popupWidth = 650.0f;
    const float popupHeight = 600.0f;

    Rectangle popup{
        (GetScreenWidth() - popupWidth) / 2.0f,
        (GetScreenHeight() - popupHeight) / 2.0f,
        popupWidth,
        popupHeight};

    DrawRectangleRounded(
        popup,
        0.05f,
        20,
        Color{25, 20, 18, 250});

    DrawRectangleRoundedLines(
        popup,
        0.05f,
        20,
        Color{180, 160, 130, 255});

    // =========================================
    // Title
    // =========================================

    const char *title = "SAVE GAME";

    const float titleSize = 34.0f;

    Vector2 titleSizeVec =
        MeasureTextEx(
            font,
            title,
            titleSize,
            2.0f);

    DrawTextEx(
        font,
        title,
        Vector2{
            popup.x + (popup.width - titleSizeVec.x) / 2.0f,
            popup.y + 30.0f},
        titleSize,
        2.0f,
        WHITE);

    // =========================================
    // Save Buttons
    // =========================================

    saveButtons.clear();

    const float buttonWidth = 420.0f;
    const float buttonHeight = 55.0f;
    const float buttonGap = 15.0f;

    float startX =
        popup.x + (popup.width - buttonWidth) / 2.0f;

    float startY = popup.y + 100.0f;

    Vector2 mousePosition = GetMousePosition();

    for (size_t i = 0; i < saveFiles.size(); i++)
    {
        Rectangle button{
            startX,
            startY + i * (buttonHeight + buttonGap),
            buttonWidth,
            buttonHeight};

        saveButtons.push_back(button);

        bool hovered =
            CheckCollisionPointRec(
                mousePosition,
                button);

        Color buttonColor =
            hovered
                ? Color{75, 75, 75, 245}
                : Color{35, 35, 35, 235};

        DrawRectangleRounded(
            button,
            0.1f,
            20,
            buttonColor);

        DrawRectangleRoundedLines(
            button,
            0.1f,
            20,
            hovered
                ? WHITE
                : Color{150, 150, 150, 255});

        std::string text =
            saveFiles[i];

        Vector2 textSize =
            MeasureTextEx(
                font,
                text.c_str(),
                23.0f,
                1.5f);

        DrawTextEx(
            font,
            text.c_str(),
            Vector2{
                button.x +
                    (button.width - textSize.x) / 2.0f,
                button.y +
                    (button.height - textSize.y) / 2.0f},
            23.0f,
            1.5f,
            WHITE);
    }

    // =========================================
    // NEW SAVE BUTTON
    // =========================================

    float newSaveY =
        startY +
        saveFiles.size() *
            (buttonHeight + buttonGap);

    newSaveButton = Rectangle{
        startX,
        newSaveY,
        buttonWidth,
        buttonHeight};

    bool newSaveHovered =
        CheckCollisionPointRec(
            mousePosition,
            newSaveButton);

    DrawRectangleRounded(
        newSaveButton,
        0.1f,
        20,
        newSaveHovered
            ? Color{80, 110, 70, 245}
            : Color{45, 80, 40, 235});

    DrawRectangleRoundedLines(
        newSaveButton,
        0.1f,
        20,
        newSaveHovered
            ? WHITE
            : Color{130, 180, 130, 255});

    const char *newSaveText = "+ NEW SAVE";

    Vector2 newSaveTextSize =
        MeasureTextEx(font,
                      newSaveText,
                      23.0f,
                      1.5f);

    DrawTextEx(
        font,
        newSaveText,
        Vector2{
            newSaveButton.x +
                (newSaveButton.width -
                 newSaveTextSize.x) /
                    2.0f,
            newSaveButton.y +
                (newSaveButton.height -
                 newSaveTextSize.y) /
                    2.0f},
        23.0f,
        1.5f,
        WHITE);

    // =========================================
    // BACK BUTTON
    // =========================================

    saveMenuBackButton = Rectangle{
        popup.x + (popup.width - 150.0f) / 2.0f,
        popup.y + popup.height - 75.0f,
        150.0f,
        48.0f};

    bool backHovered =
        CheckCollisionPointRec(
            mousePosition,
            saveMenuBackButton);

    DrawRectangleRounded(
        saveMenuBackButton,
        0.1f,
        20,
        backHovered
            ? Color{75, 75, 75, 245}
            : Color{35, 35, 35, 235});

    const char *backText = "BACK";

    Vector2 backTextSize =
        MeasureTextEx(
            font,
            backText,
            22.0f,
            1.5f);

    DrawTextEx(
        font,
        backText,
        Vector2{
            saveMenuBackButton.x +
                (saveMenuBackButton.width -
                 backTextSize.x) /
                    2.0f,
            saveMenuBackButton.y +
                (saveMenuBackButton.height -
                 backTextSize.y) /
                    2.0f},
        22.0f,
        1.5f,
        WHITE);
}
void GameScreen::drawFeintBlockedPopup()
{
    if (!feintBlockedPopupOpen || assets == nullptr)
    {
        return;
    }

    Font font = assets->getGameFont();

    const float fontSize = 22.0f;
    const float spacing = 1.5f;

    Vector2 textSize =
        MeasureTextEx(font, feintBlockedMessage.c_str(), fontSize, spacing);

    // بالای صفحه با کمی فاصله
    float textY = 70.0f;
    float textX = (GetScreenWidth() - textSize.x) / 2.0f;

    Rectangle background{
        textX - 20.0f,
        textY - 10.0f,
        textSize.x + 40.0f,
        textSize.y + 20.0f};

    DrawRectangleRounded(
        background,
        0.2f,
        12,
        Color{80, 20, 20, 220});

    DrawRectangleRoundedLines(
        background,
        0.2f,
        12,
        Color{220, 100, 100, 255});

    DrawTextEx(
        font,
        feintBlockedMessage.c_str(),
        Vector2{textX, textY},
        fontSize,
        spacing,
        WHITE);
}