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
#include <sstream>

GameScreen::GameScreen(
    AssetManager *assets,
    Game *game)
    : assets(assets),
      game(game),
      leftPlayerPanel(assets),
      rightPlayerPanel(assets),
      attackUI(assets),
      schemeUI(assets),
      maneuverUI(assets),
      effectUI(assets)
{
}

int GameScreen::update()
{
    if (assets == nullptr)
    {
        return 0;
    }

    // =========================================
    // Button dimensions
    // =========================================

    const float buttonWidth = 145.0f;
    const float buttonHeight = 48.0f;
    const float buttonGap = 15.0f;

    const float totalWidth =
        3.0f * buttonWidth +
        2.0f * buttonGap;

    const float startX =
        (GetScreenWidth() - totalWidth) / 2.0f;

    const float buttonY = 12.0f;

    Rectangle exitButton{
        startX,
        buttonY,
        buttonWidth,
        buttonHeight};

    Rectangle saveButton{
        startX + buttonWidth + buttonGap,
        buttonY,
        buttonWidth,
        buttonHeight};

    Rectangle guideButton{
        startX +
            2.0f * (buttonWidth + buttonGap),
        buttonY,
        buttonWidth,
        buttonHeight};

    Vector2 mousePosition = GetMousePosition();

    // =========================================
    // GUIDE POPUP
    // =========================================

    if (guideOpen)
    {
        const float popupWidth = 900.0f;
        const float popupHeight = 720.0f;

        const float popupX =
            (GetScreenWidth() - popupWidth) / 2.0f;

        const float popupY =
            (GetScreenHeight() - popupHeight) / 2.0f;

        const float backWidth = 130.0f;
        const float backHeight = 45.0f;

        Rectangle backButton{
            (GetScreenWidth() - backWidth) / 2.0f,
            popupY + popupHeight - 65.0f,
            backWidth,
            backHeight};

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(
                    mousePosition,
                    backButton))
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
        // =========================================
        // EXIT
        // =========================================

        if (CheckCollisionPointRec(
                mousePosition,
                exitButton))
        {
            return 1;
        }

        // =========================================
        // SAVE GAME
        // =========================================

        if (CheckCollisionPointRec(
                mousePosition,
                saveButton))
        {
            // TODO:
            // Save game logic
            return 0;
        }

        // =========================================
        // GUIDE
        // =========================================

        if (CheckCollisionPointRec(
                mousePosition,
                guideButton))
        {
            guideOpen = true;
            return 0;
        }

        // =========================================
        // ACTION BUTTONS (فقط وقتی مانور باز نیست)
        // =========================================

        if (!maneuverUI.isOpen())
        {
            if (CheckCollisionPointRec(
                    mousePosition,
                    attackButton))
            {
                selectedAction =
                    ActionChoice::ATTACK;

                std::cout
                    << "Selected Action: ATTACK"
                    << std::endl;

                Player *currentPlayer =
                    game->getTurnManager().getCurrentPlayer();

                if (currentPlayer != nullptr)
                {
                    std::vector<Fighter *> fighters;

                    Hero *hero =
                        currentPlayer->getHero();

                    if (hero != nullptr &&
                        hero->isAlive())
                    {
                        fighters.push_back(hero);
                    }

                    std::vector<Sidekick *> sidekicks =
                        currentPlayer->getSideKicks();

                    for (Sidekick *sidekick : sidekicks)
                    {
                        if (sidekick != nullptr &&
                            sidekick->isAlive())
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
                selectedAction =
                    ActionChoice::MANEUVER;

                std::cout
                    << "Selected Action: MANEUVER"
                    << std::endl;

                Player *currentPlayer =
                    game->getTurnManager().getCurrentPlayer();

                if (currentPlayer != nullptr)
                {
                    maneuverUI.open(currentPlayer);
                }

                selectedAction = ActionChoice::NONE;
                return 0;
            }

            if (CheckCollisionPointRec(
                    mousePosition,
                    schemeButton))
            {
                selectedAction =
                    ActionChoice::SCHEME;

                std::cout
                    << "Selected Action: SCHEME"
                    << std::endl;

                Player *currentPlayer =
                    game->getTurnManager().getCurrentPlayer();

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

            effectUI.open(game, combatSystem.getPendingEffect(),
                          combatSystem.getPendingUser(),
                          combatSystem.getPendingTarget());

            combatEffectRequested = true;
            combatShowLookButton = false;

            return 0;
        }

        if (combatInProgress &&
            !combatResultPopupOpen &&
            game->getCombatSystem().isAwaitingResultReveal() &&
            CheckCollisionPointRec(mousePosition, resultRevealButton))
        {
            combatResultPopupOpen = true;
            return 0;
        }

        if (combatResultPopupOpen &&
            CheckCollisionPointRec(mousePosition, resultBackButton))
        {
            combatResultPopupOpen = false;
            game->getCombatSystem().acknowledgeResult();
            return 0;
        }

        // =========================================
        // MAP SPACE CLICK
        // =========================================

        int clickedSpaceId =
            getClickedSpaceId();

        if (clickedSpaceId != -1)
        {
            std::cout
                << "Clicked Space: "
                << clickedSpaceId
                << std::endl;

            if (maneuverUI.isSelectingSpace())
            {
                for (Space *space : maneuverUI.getAvailableMoves())
                {
                    if (space != nullptr &&
                        space->getId() == clickedSpaceId)
                    {
                        game->getBoard().moveFighter(
                            maneuverUI.getSelectedFighter(),
                            space);

                        maneuverUI.finishAfterMove();
                        break;
                    }
                }
            }

            if (effectUI.isChoosingSpace())
            {
                Space *clickedSpace =
                    game->getBoard().getSpace(clickedSpaceId);

                if (clickedSpace != nullptr)
                {
                    std::cout
                        << "[+] Effect destination clicked: Space "
                        << clickedSpace->getId()
                        << std::endl;

                    effectUI.selectSpace(clickedSpace);

                    if (effectUI.isReady() &&
                        combatInProgress &&
                        combatEffectRequested)
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
                effectUI.open(game, combatSystem.getPendingEffect(),
                              combatSystem.getPendingUser(),
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
        Player *currentPlayer =
            game->getTurnManager().getCurrentPlayer();

        Fighter *chosenAttacker =
            attackUI.getSelectedAttacker();

        Fighter *chosenTarget =
            attackUI.getSelectedTarget();

        Card *chosenAttackCard =
            attackUI.getSelectedAttackCard();

        Card *chosenDefenseCard =
            attackUI.getSelectedDefenseCard();

        if (chosenAttacker != nullptr &&
            chosenTarget != nullptr &&
            chosenAttackCard != nullptr)
        {
            Player *defenderPlayer =
                chosenTarget->getOwner();

            Card *removedAttackCard = nullptr;
            Card *removedDefenceCard = nullptr;

            if (currentPlayer != nullptr)
            {
                Hand &attackerHand =
                    currentPlayer->getHand();

                const std::vector<Card *> &attackerCards =
                    attackerHand.getCards();

                for (int i = 0;
                     i < static_cast<int>(attackerCards.size());
                     i++)
                {
                    if (attackerCards[i] == chosenAttackCard)
                    {
                        removedAttackCard =
                            attackerHand.removeCard(i);
                        break;
                    }
                }
            }

            if (chosenDefenseCard != nullptr &&
                defenderPlayer != nullptr)
            {
                Hand &defenderHand =
                    defenderPlayer->getHand();

                const std::vector<Card *> &defenderCards =
                    defenderHand.getCards();

                for (int i = 0;
                     i < static_cast<int>(defenderCards.size());
                     i++)
                {
                    if (defenderCards[i] == chosenDefenseCard)
                    {
                        removedDefenceCard =
                            defenderHand.removeCard(i);
                        break;
                    }
                }
            }

            if (removedAttackCard != nullptr)
            {
                game->getCombatSystem().beginCombat(
                    *game,
                    *chosenAttacker,
                    *chosenTarget,
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
            Player *currentPlayer =
                game->getTurnManager().getCurrentPlayer();

            if (currentPlayer != nullptr)
            {
                Effect *effect = playedCard->getEffect();

                if (effect == nullptr ||
                    effect->getInputKind() == EffectInputKind::None)
                {
                    if (effect != nullptr)
                    {
                        EffectChoice emptyChoice;

                        effect->apply(
                            *game,
                            *currentPlayer->getHero(),
                            *currentPlayer->getHero(),
                            *playedCard,
                            nullptr,
                            false,
                            emptyChoice);
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
                    // تعاملی: EffectUI رو باز کن، فعلاً کارت رو نگه دار
                    pendingSchemeCard = playedCard;

                    Fighter *actingFighter = currentPlayer->getHero();

                    if (playedCard->getOwnerType() == OwnerType::Sidekick)
                    {
                        actingFighter = currentPlayer->getWatson();

                        if (actingFighter == nullptr && !currentPlayer->getSideKicks().empty())
                        {
                            actingFighter = currentPlayer->getSideKicks()[0];
                        }
                    }

                    effectUI.open(game, effect, actingFighter, actingFighter);
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

            std::vector<Space *> moves =
                game->getBoard().getAvailableMoves(
                    fighter,
                    movementBudget);

            if (moves.empty())
            {
                std::cout << "\n[!] No available moves for "
                          << fighter->getName()
                          << std::endl;

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

            if (pendingSchemeCard->getOwnerType() == OwnerType::Sidekick)
            {
                actingFighter = currentPlayer->getWatson();
            }

            if (effect != nullptr && actingFighter != nullptr)
            {
                effect->apply(*game, *actingFighter, *actingFighter,
                              *pendingSchemeCard, nullptr, false,
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
    return 0;
}

void GameScreen::draw()
{
    if (assets == nullptr)
    {
        return;
    }

    Texture2D background =
        assets->getMainPanelBackground();

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
        ClearBackground(
            Color{
                45,
                30,
                20,
                255});
    }

    drawMap();
    drawPlayerPanels();
    drawTopButtons();
    drawTurnIndicator();
    drawCombatEffectText();
    drawResultRevealButton();
    drawLookButton();

    if (!maneuverUI.isOpen() && !combatInProgress)
    {
        drawActionButtons();
    }

    if (guideOpen)
    {
        drawGuidePopup();
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
}

void GameScreen::drawMap()
{
    Texture2D map =
        assets->getGameMap();

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

    calculateMapTransform(
        mapX,
        mapY,
        scale,
        mapWidth,
        mapHeight);

    Rectangle source{
        0.0f,
        0.0f,
        static_cast<float>(map.width),
        static_cast<float>(map.height)};

    Rectangle destination{
        mapX,
        mapY,
        mapWidth,
        mapHeight};

    DrawTexturePro(
        map,
        source,
        destination,
        Vector2{0.0f, 0.0f},
        0.0f,
        WHITE);

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

    const float panelHeight =
        GetScreenHeight() -
        2.0f * margin;

    leftPlayerPanel.setBounds(
        Rectangle{
            margin,
            margin,
            panelWidth,
            panelHeight});

    rightPlayerPanel.setBounds(
        Rectangle{
            GetScreenWidth() -
                panelWidth -
                margin,

            margin,
            panelWidth,
            panelHeight});

    const auto &players =
        game->getPlayers();

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
    Font font =
        assets->getGameFont();

    // =========================================
    // Button dimensions
    // =========================================

    const float buttonWidth = 145.0f;
    const float buttonHeight = 48.0f;

    const float buttonGap = 15.0f;

    const float totalWidth =
        3.0f * buttonWidth +
        2.0f * buttonGap;

    const float startX =
        (GetScreenWidth() - totalWidth) / 2.0f;

    const float buttonY = 12.0f;

    // =========================================
    // Button rectangles
    // =========================================

    Rectangle exitButton{
        startX,
        buttonY,
        buttonWidth,
        buttonHeight};

    Rectangle saveButton{
        startX +
            buttonWidth +
            buttonGap,

        buttonY,

        buttonWidth,
        buttonHeight};

    Rectangle guideButton{
        startX +
            2.0f *
                (buttonWidth + buttonGap),

        buttonY,

        buttonWidth,
        buttonHeight};

    // =========================================
    // Mouse
    // =========================================

    Vector2 mousePosition =
        GetMousePosition();

    // =========================================
    // Colors
    // =========================================

    Color normalColor{
        25,
        25,
        25,
        190};

    Color hoverColor{
        65,
        65,
        65,
        210};

    // =========================================
    // EXIT
    // =========================================

    Color exitColor =
        CheckCollisionPointRec(
            mousePosition,
            exitButton)
            ? hoverColor
            : normalColor;

    DrawRectangleRounded(
        exitButton,
        1.0f,
        32,
        exitColor);

    // =========================================
    // SAVE GAME
    // =========================================

    Color saveColor =
        CheckCollisionPointRec(
            mousePosition,
            saveButton)
            ? hoverColor
            : normalColor;

    DrawRectangleRounded(
        saveButton,
        1.0f,
        32,
        saveColor);

    // =========================================
    // GUIDE
    // =========================================

    Color guideColor =
        CheckCollisionPointRec(
            mousePosition,
            guideButton)
            ? hoverColor
            : normalColor;

    DrawRectangleRounded(
        guideButton,
        1.0,
        32,
        guideColor);

    // =========================================
    // Text
    // =========================================

    const float fontSize = 26.0f;
    const float spacing = 2.0f;

    const char *exitText =
        "EXIT";

    const char *saveText =
        "SAVE GAME";

    const char *guideText =
        "GUIDE";

    // =========================================
    // Measure text
    // =========================================

    Vector2 exitTextSize =
        MeasureTextEx(
            font,
            exitText,
            fontSize,
            spacing);

    Vector2 saveTextSize =
        MeasureTextEx(
            font,
            saveText,
            fontSize,
            spacing);

    Vector2 guideTextSize =
        MeasureTextEx(
            font,
            guideText,
            fontSize,
            spacing);

    // =========================================
    // EXIT text
    // =========================================

    DrawTextEx(
        font,
        exitText,

        Vector2{
            exitButton.x +
                (exitButton.width -
                 exitTextSize.x) /
                    2.0f,

            exitButton.y +
                (exitButton.height -
                 exitTextSize.y) /
                    2.0f},

        fontSize,
        spacing,
        WHITE);

    // =========================================
    // SAVE GAME text
    // =========================================

    DrawTextEx(
        font,
        saveText,

        Vector2{
            saveButton.x +
                (saveButton.width -
                 saveTextSize.x) /
                    2.0f,

            saveButton.y +
                (saveButton.height -
                 saveTextSize.y) /
                    2.0f},

        fontSize,
        spacing,
        WHITE);

    // =========================================
    // GUIDE text
    // =========================================

    DrawTextEx(
        font,
        guideText,

        Vector2{
            guideButton.x +
                (guideButton.width -
                 guideTextSize.x) /
                    2.0f,

            guideButton.y +
                (guideButton.height -
                 guideTextSize.y) /
                    2.0f},

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

    DrawRectangle(
        0,
        0,
        GetScreenWidth(),
        GetScreenHeight(),
        Color{0, 0, 0, 120});

    // =========================================
    // Popup size
    // =========================================

    const float popupWidth = 900.0f;
    const float popupHeight = 730.0f;

    Rectangle popup{
        (GetScreenWidth() - popupWidth) / 2.0f,
        (GetScreenHeight() - popupHeight) / 2.0f,
        popupWidth,
        popupHeight};

    // =========================================
    // Popup background
    // =========================================

    DrawRectangleRounded(
        popup,
        0.04f,
        20,
        Color{25, 20, 18, 245});

    // =========================================
    // Popup border
    // =========================================

    DrawRectangleRoundedLines(
        popup,
        0.04f,
        20,
        Color{180, 160, 130, 255});

    // =========================================
    // Title
    // =========================================

    const char *title = "INSTRUCTIONS";

    const float titleSize = 38.0f;

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
            popup.x +
                (popup.width - titleSizeVec.x) / 2.0f,

            popup.y + 25.0f},

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

    DrawTextEx(
        font,
        "[ ACTIONS ]",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "Attack   ->  Attack an enemy fighter.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "Maneuver ->  Move a fighter and draw a card.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "Scheme   ->  Play a Scheme card.",
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
        "[o] Each turn you have 2 actions.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "[o] Every Hero card carries a unique effect,",
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
        "[o] Your hand must never exceed 7 cards.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font, "[o] Heroes enter the battlefield alongside their",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font, "    Sidekicks, NEVER ALONE.",
        Vector2{x, y},
        textSize,
        spacing,
        WHITE);

    y += lineHeight;

    DrawTextEx(
        font,
        "[o] Through tactical moves in each turn, your goal",
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

    y += lineHeight;

    DrawTextEx(
        font,
        "[o] The younger player chooses the fighter and steps",
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
    if (game == nullptr ||
        assets == nullptr)
    {
        return;
    }

    Font font =
        assets->getGameFont();

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

        Hero *hero =
            player->getHero();

        if (hero != nullptr)
        {
            Space *space =
                hero->getPosition();

            if (space != nullptr)
            {
                int spaceId =
                    space->getId();

                if (spaceId >= 1 &&
                    spaceId <= 32)
                {
                    Vector2 center =
                        mapImageToScreen(
                            SPACE_GRAPHICS[spaceId - 1].center);

                    DrawCircleV(
                        center,
                        22.0f,
                        Color{
                            180,
                            40,
                            40,
                            230});

                    std::string heroName =
                        hero->getName();

                    Vector2 textSize =
                        MeasureTextEx(
                            font,
                            heroName.c_str(),
                            16.0f,
                            1.0f);

                    DrawTextEx(
                        font,
                        heroName.c_str(),

                        Vector2{
                            center.x -
                                textSize.x / 2.0f,

                            center.y -
                                35.0f},

                        16.0f,
                        1.0f,
                        WHITE);
                }
            }
        }

        // =====================================
        // SIDEKICKS
        // =====================================

        std::vector<Sidekick *> sidekicks =
            player->getSideKicks();

        for (Sidekick *sidekick : sidekicks)
        {
            if (sidekick == nullptr)
            {
                continue;
            }

            Space *space =
                sidekick->getPosition();

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

            Vector2 center =
                mapImageToScreen(
                    SPACE_GRAPHICS[spaceId - 1].center);

            DrawCircleV(
                center,
                15.0f,
                Color{
                    60,
                    120,
                    200,
                    230});

            std::string sidekickName =
                sidekick->getName();

            Vector2 textSize =
                MeasureTextEx(
                    font,
                    sidekickName.c_str(),
                    12.0f,
                    1.0f);

            DrawTextEx(
                font,
                sidekickName.c_str(),

                Vector2{
                    center.x -
                        textSize.x / 2.0f,

                    center.y -
                        28.0f},

                12.0f,
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
        GetScreenHeight() - 175.0f;

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
        mapY + mapHeight + 70.0f, // کمی پایین‌تر از نوار "Turn"
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
        fontSize, 1.5f, WHITE);
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
        mapY + mapHeight + 140.0f,
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