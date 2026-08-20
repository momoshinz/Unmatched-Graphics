#include "graphics/ManeuverUI.h"
#include "graphics/AssetManager.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "fighter/Hero.h"
#include "fighter/Sidekick.h"
#include "card/Hand.h"
#include "card/Card.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>

ManeuverUI::ManeuverUI(AssetManager *assets)
    : assets(assets)
{
}

// ============================================================
// اسم نمایشی کارت -> کلید تکسچر در AssetManager (تمام کارت‌ها)
// ============================================================

std::string ManeuverUI::getCardTextureKey(const Card *card, const std::string &heroName)
{
    if (card == nullptr)
    {
        return "";
    }

    const std::string &name = card->getName();

    // "Feint" بین دراکولا و شرلوک مشترکه؛ با هیرو تفکیک می‌کنیم
    if (name == "Feint")
    {
        return (heroName == "DRACULA") ? "FeintDracula" : "FeintSherlock";
    }

    static const std::unordered_map<std::string, std::string> nameToKey = {
        {"Feeding Frenzy", "FeedingFrenzy"},
        {"Beastform", "BeastForm"},
        {"Ambush", "Ambush"},
        {"Thirst For Sustenance", "ThirstForSustenance"},
        {"Look Into My Eyes", "LookIntoMyEyes"},
        {"Dash", "Dash"},
        {"Exploit", "Exploit"},
        {"Mistform", "MistForm"},
        {"Baptism Of Blood", "BaptismOfBlood"},
        {"Prey Upon", "PreyUpon"},
        {"Ravening Seduction", "RaveningSeduction"},
        {"The Game Is Afoot", "TheGameIsAfoot"},
        {"Service Revolver", "ServiceRevolver"},
        {"Counter Punch", "CounterPunch"},
        {"Deduce Strategy", "DeduceStrategy"},
        {"Education Never Ends", "EducationNeverEnds"},
        {"Fixed Point", "FixedPoint"},
        {"Study Methods", "StudyMethods"},
        {"Administer Aid", "AdministerAid"},
        {"Eliminate The Impossible", "EliminateTheImpossible"},
        {"Master Of Disguise", "MasterOfDisguise"},
        {"Coded Notes", "CodedNotes"},
        {"Dreaming Of Revenge", "DreamingOfRevange"},
        {"Emerge From Mist", "EmergeFromMist"},
        {"Impossible To See", "ImpossibleToSee"},
        {"Into Thin Air", "IntoThinAir"},
        {"Lurking", "Lurking"},
        {"Reign Of Terror", "ReignOfTerror"},
        {"Rolling Fog", "RollingFog"},
        {"Slip Away", "SlipAway"},
        {"Step Lightly", "StepLightly"}};

    auto it = nameToKey.find(name);

    if (it == nameToKey.end())
    {
        return "";
    }

    return it->second;
}

// ============================================================
// OPEN
// ============================================================

void ManeuverUI::open(Player *player)
{
    this->player = player;

    selectableFighters.clear();
    fighterBoxes.clear();
    selectedFighter = nullptr;

    burnableCards.clear();
    burnCardBoxes.clear();
    burnSelectedIndex = -1;
    boostAmount = 0;

    availableMoves.clear();
    waitingForMoves = false;
    readyToFinalize = false;

    if (player == nullptr)
    {
        state = ManeuverState::CLOSED;
        return;
    }

    state = ManeuverState::ASK_MOVE;
    layoutAskButtons();
}

// ============================================================
// LAYOUT HELPERS
// ============================================================

void ManeuverUI::layoutAskButtons()
{
    const float buttonWidth = 160.0f;
    const float buttonHeight = 55.0f;
    const float gap = 30.0f;

    const float totalWidth = 2.0f * buttonWidth + gap;
    const float startX = (GetScreenWidth() - totalWidth) / 2.0f;
    const float buttonY = GetScreenHeight() - 90.0f;

    yesButton = Rectangle{startX, buttonY, buttonWidth, buttonHeight};
    noButton = Rectangle{startX + buttonWidth + gap, buttonY, buttonWidth, buttonHeight};
}

void ManeuverUI::layoutFighterSelection()
{
    selectableFighters.clear();
    fighterBoxes.clear();

    if (player == nullptr)
    {
        return;
    }

    Hero *hero = player->getHero();

    if (hero != nullptr && hero->isAlive())
    {
        selectableFighters.push_back(hero);
    }

    for (Sidekick *sidekick : player->getSideKicks())
    {
        if (sidekick != nullptr && sidekick->isAlive())
        {
            selectableFighters.push_back(sidekick);
        }
    }

    const float boxWidth = 180.0f;
    const float boxHeight = 70.0f;
    const float gap = 20.0f;

    int count = static_cast<int>(selectableFighters.size());

    const float totalWidth =
        count * boxWidth + (count > 0 ? (count - 1) : 0) * gap;

    const float startX = (GetScreenWidth() - totalWidth) / 2.0f;
    const float boxY = GetScreenHeight() - 110.0f;

    for (int i = 0; i < count; i++)
    {
        Rectangle box{
            startX + i * (boxWidth + gap),
            boxY,
            boxWidth,
            boxHeight};

        fighterBoxes.push_back(box);
    }
}

void ManeuverUI::layoutBurnCardWindow()
{
    burnableCards.clear();
    burnCardBoxes.clear();
    burnSelectedIndex = -1;

    if (player == nullptr)
    {
        return;
    }

    for (Card *card : player->getHand().getCards())
    {
        if (card != nullptr)
        {
            burnableCards.push_back(card);
        }

        if (burnableCards.size() >= 7)
        {
            break;
        }
    }

    const int maxPerRow = 4;
    const float boxWidth = 180.0f;
    const float boxHeight = 260.0f;
    const float gapX = 20.0f;
    const float gapY = 20.0f;
    const float startY = 130.0f;

    int total = static_cast<int>(burnableCards.size());
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

            burnCardBoxes.push_back(box);
            index++;
        }
    }

    const float burnWidth = 200.0f;
    const float burnHeight = 55.0f;
    float lastRowBottom = startY + rowCount * (boxHeight + gapY);

    burnButton = Rectangle{
        (GetScreenWidth() - burnWidth) / 2.0f,
        lastRowBottom + 15.0f,
        burnWidth,
        burnHeight};
}

// ============================================================
// UPDATE
// ============================================================

void ManeuverUI::update()
{
    if (state == ManeuverState::CLOSED ||
        state == ManeuverState::AWAITING_MOVES ||
        state == ManeuverState::SELECT_SPACE)
    {
        return;
    }

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        return;
    }

    Vector2 mouse = GetMousePosition();

    if (state == ManeuverState::ASK_MOVE)
    {
        if (CheckCollisionPointRec(mouse, yesButton))
        {
            state = ManeuverState::SELECT_FIGHTER;
            layoutFighterSelection();
        }
        else if (CheckCollisionPointRec(mouse, noButton))
        {
            state = ManeuverState::CLOSED;
            readyToFinalize = true;
        }

        return;
    }

    if (state == ManeuverState::SELECT_FIGHTER)
    {
        for (size_t i = 0; i < fighterBoxes.size(); i++)
        {
            if (CheckCollisionPointRec(mouse, fighterBoxes[i]))
            {
                selectedFighter = selectableFighters[i];

                std::cout << "Fighter selected for maneuver: "
                          << selectedFighter->getName()
                          << std::endl;

                state = ManeuverState::ASK_BOOST;
                layoutAskButtons();
                return;
            }
        }

        return;
    }

    if (state == ManeuverState::ASK_BOOST)
    {
        if (CheckCollisionPointRec(mouse, yesButton))
        {
            if (player != nullptr && player->getHand().isEmpty())
            {
                std::cout << "[!] No cards in hand to burn for boost." << std::endl;

                boostAmount = 0;
                state = ManeuverState::AWAITING_MOVES;
                waitingForMoves = true;
            }
            else
            {
                state = ManeuverState::BURN_CARD;
                layoutBurnCardWindow();
            }
        }
        else if (CheckCollisionPointRec(mouse, noButton))
        {
            boostAmount = 0;
            state = ManeuverState::AWAITING_MOVES;
            waitingForMoves = true;
        }

        return;
    }

    if (state == ManeuverState::BURN_CARD)
    {
        for (size_t i = 0; i < burnCardBoxes.size(); i++)
        {
            if (CheckCollisionPointRec(mouse, burnCardBoxes[i]))
            {
                burnSelectedIndex = static_cast<int>(i);

                std::cout << "Boost card selected: "
                          << burnableCards[i]->getName()
                          << std::endl;

                return;
            }
        }

        if (burnSelectedIndex != -1 &&
            CheckCollisionPointRec(mouse, burnButton))
        {
            burnSelectedCard();

            state = ManeuverState::AWAITING_MOVES;
            waitingForMoves = true;
        }

        return;
    }
}

void ManeuverUI::burnSelectedCard()
{
    if (player == nullptr ||
        burnSelectedIndex < 0 ||
        burnSelectedIndex >= static_cast<int>(burnableCards.size()))
    {
        return;
    }

    Card *chosen = burnableCards[burnSelectedIndex];

    Hand &hand = player->getHand();
    const std::vector<Card *> &cards = hand.getCards();

    for (int i = 0; i < static_cast<int>(cards.size()); i++)
    {
        if (cards[i] == chosen)
        {
            Card *removed = hand.removeCard(i);

            boostAmount = removed->getBoost();

            player->getDiscardPile().addCard(removed);

            std::cout << "[.] Boost card burned: "
                      << removed->getName()
                      << " (+" << boostAmount << " movement)"
                      << std::endl;

            break;
        }
    }
}

// ============================================================
// GETTERS / STATE TRANSITIONS
// ============================================================

bool ManeuverUI::isOpen() const
{
    return state != ManeuverState::CLOSED;
}

bool ManeuverUI::isSelectingSpace() const
{
    return state == ManeuverState::SELECT_SPACE;
}

bool ManeuverUI::needsAvailableMoves() const
{
    return state == ManeuverState::AWAITING_MOVES && waitingForMoves;
}

Fighter *ManeuverUI::getFighterNeedingMoves() const
{
    return selectedFighter;
}

int ManeuverUI::getMovementBudget() const
{
    if (selectedFighter == nullptr)
    {
        return 0;
    }

    return selectedFighter->getMovement() + boostAmount;
}

void ManeuverUI::beginSpaceSelection(const std::vector<Space *> &moves)
{
    availableMoves = moves;
    waitingForMoves = false;
    state = ManeuverState::SELECT_SPACE;
}

Fighter *ManeuverUI::getSelectedFighter() const
{
    return selectedFighter;
}

const std::vector<Space *> &ManeuverUI::getAvailableMoves() const
{
    return availableMoves;
}

void ManeuverUI::finishAfterMove()
{
    state = ManeuverState::CLOSED;
    readyToFinalize = true;
    availableMoves.clear();
    selectedFighter = nullptr;
}

bool ManeuverUI::consumeReadyToFinalize()
{
    if (!readyToFinalize)
    {
        return false;
    }

    readyToFinalize = false;
    return true;
}

// ============================================================
// DRAW
// ============================================================

void ManeuverUI::draw()
{
    if (assets == nullptr)
    {
        return;
    }

    Font font = assets->getGameFont();
    Vector2 mouse = GetMousePosition();

    if (state == ManeuverState::ASK_MOVE || state == ManeuverState::ASK_BOOST)
    {
        const char *question =
            (state == ManeuverState::ASK_MOVE)
                ? "Do you want to move a fighter?"
                : "Do you want to use a boost card?";

        const float questionSize = 26.0f;

        Vector2 questionTextSize =
            MeasureTextEx(font, question, questionSize, 1.5f);

        DrawTextEx(
            font, question,
            Vector2{
                (GetScreenWidth() - questionTextSize.x) / 2.0f,
                GetScreenHeight() - 145.0f},
            questionSize, 1.5f, WHITE);

        bool yesHovered = CheckCollisionPointRec(mouse, yesButton);
        DrawRectangleRounded(yesButton, 1.0f, 20,
                             yesHovered ? Color{75, 75, 75, 245} : Color{35, 35, 35, 235});

        Vector2 yesTextSize = MeasureTextEx(font, "YES", 24.0f, 1.5f);
        DrawTextEx(font, "YES",
                   Vector2{
                       yesButton.x + (yesButton.width - yesTextSize.x) / 2.0f,
                       yesButton.y + (yesButton.height - yesTextSize.y) / 2.0f},
                   24.0f, 1.5f, WHITE);

        bool noHovered = CheckCollisionPointRec(mouse, noButton);
        DrawRectangleRounded(noButton, 1.0f, 20,
                             noHovered ? Color{75, 75, 75, 245} : Color{35, 35, 35, 235});

        Vector2 noTextSize = MeasureTextEx(font, "NO", 24.0f, 1.5f);
        DrawTextEx(font, "NO",
                   Vector2{
                       noButton.x + (noButton.width - noTextSize.x) / 2.0f,
                       noButton.y + (noButton.height - noTextSize.y) / 2.0f},
                   24.0f, 1.5f, WHITE);

        return;
    }

    if (state == ManeuverState::SELECT_FIGHTER)
    {
        const char *title = "CHOOSE A FIGHTER TO MOVE";
        const float titleSize = 26.0f;

        Vector2 titleTextSize = MeasureTextEx(font, title, titleSize, 1.5f);

        DrawTextEx(
            font, title,
            Vector2{
                (GetScreenWidth() - titleTextSize.x) / 2.0f,
                GetScreenHeight() - 150.0f},
            titleSize, 1.5f, WHITE);

        for (size_t i = 0; i < selectableFighters.size(); i++)
        {
            Rectangle box = fighterBoxes[i];
            bool hovered = CheckCollisionPointRec(mouse, box);

            DrawRectangleRounded(box, 1.0f, 20,
                                 hovered ? Color{75, 75, 75, 245} : Color{35, 35, 35, 235});

            DrawRectangleRoundedLines(box, 1.0f, 20,
                                      hovered ? WHITE : Color{150, 150, 150, 255});

            std::string name = selectableFighters[i]->getName();

            Vector2 nameSize = MeasureTextEx(font, name.c_str(), 20.0f, 1.0f);

            DrawTextEx(
                font, name.c_str(),
                Vector2{
                    box.x + (box.width - nameSize.x) / 2.0f,
                    box.y + (box.height - nameSize.y) / 2.0f},
                20.0f, 1.0f, WHITE);
        }

        return;
    }

    if (state == ManeuverState::BURN_CARD)
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{0, 0, 0, 190});

        const char *title = "CHOOSE A CARD TO BURN FOR BOOST";
        const float titleSize = 32.0f;

        Vector2 titleTextSize = MeasureTextEx(font, title, titleSize, 2.0f);

        DrawTextEx(
            font, title,
            Vector2{(GetScreenWidth() - titleTextSize.x) / 2.0f, 70.0f},
            titleSize, 2.0f, WHITE);

        std::string heroName =
            (player != nullptr && player->getHero() != nullptr)
                ? player->getHero()->getName()
                : "";

        for (size_t i = 0; i < burnableCards.size(); i++)
        {
            Card *card = burnableCards[i];
            Rectangle box = burnCardBoxes[i];

            bool hovered = CheckCollisionPointRec(mouse, box);
            bool selected = (static_cast<int>(i) == burnSelectedIndex);

            Color boxColor;
            if (selected)
                boxColor = Color{120, 85, 40, 245};
            else if (hovered)
                boxColor = Color{75, 75, 75, 245};
            else
                boxColor = Color{35, 35, 35, 235};

            DrawRectangleRounded(box, 0.08f, 20, boxColor);
            DrawRectangleRoundedLines(box, 0.08f, 20,
                                      (hovered || selected) ? WHITE : Color{150, 150, 150, 255});

            std::string textureKey = getCardTextureKey(card, heroName);

            if (!textureKey.empty())
            {
                Texture2D texture = assets->getCard(textureKey);

                if (texture.id != 0)
                {
                    const float padding = 10.0f;

                    Rectangle source{
                        0.0f, 0.0f,
                        static_cast<float>(texture.width),
                        static_cast<float>(texture.height)};

                    Rectangle destination{
                        box.x + padding,
                        box.y + padding,
                        box.width - 2.0f * padding,
                        box.height - 55.0f};

                    DrawTexturePro(texture, source, destination,
                                   Vector2{0.0f, 0.0f}, 0.0f, WHITE);
                }
            }

            std::string name = card->getName();
            Vector2 nameSize = MeasureTextEx(font, name.c_str(), 18.0f, 1.0f);

            DrawTextEx(
                font, name.c_str(),
                Vector2{
                    box.x + (box.width - nameSize.x) / 2.0f,
                    box.y + box.height - 32.0f},
                18.0f, 1.0f, WHITE);
        }

        bool burnEnabled = (burnSelectedIndex != -1);
        bool burnHovered = CheckCollisionPointRec(mouse, burnButton);

        Color burnColor;
        if (!burnEnabled)
            burnColor = Color{30, 30, 30, 120};
        else if (burnHovered)
            burnColor = Color{140, 40, 40, 245};
        else
            burnColor = Color{100, 30, 30, 235};

        DrawRectangleRounded(burnButton, 1.0f, 20, burnColor);

        const char *burnText = "BURN";
        Vector2 burnTextSize = MeasureTextEx(font, burnText, 26.0f, 1.5f);

        DrawTextEx(
            font, burnText,
            Vector2{
                burnButton.x + (burnButton.width - burnTextSize.x) / 2.0f,
                burnButton.y + (burnButton.height - burnTextSize.y) / 2.0f},
            26.0f, 1.5f,
            burnEnabled ? WHITE : Color{150, 150, 150, 150});

        return;
    }
}