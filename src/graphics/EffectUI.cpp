#include "graphics/EffectUI.h"
#include "graphics/AssetManager.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "fighter/Hero.h"
#include "fighter/Sidekick.h"
#include "board/Board.h"
#include "board/Space.h"
#include "card/Hand.h"
#include "card/Card.h"

#include <iostream>
#include <algorithm>
#include <unordered_map>

EffectUI::EffectUI(AssetManager *assets)
    : assets(assets)
{
}

// ============================================================
// اسم کارت -> کلید تکسچر (کل مجموعه کارت‌ها)
// ============================================================

std::string EffectUI::getCardTextureKey(const Card *card, const std::string &heroName)
{
    if (card == nullptr)
    {
        return "";
    }

    const std::string &name = card->getName();

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

void EffectUI::open(Game *game, Effect *effect, Fighter *fighter, Fighter *target)
{
    this->game = game;
    this->effect = effect;
    this->fighter = fighter;
    this->target = target;

    choice = EffectChoice{};

    candidateSpaces.clear();
    candidateCards.clear();
    cardBoxes.clear();
    selectedCardIndex = -1;

    candidateFighters.clear();
    fighterBoxes.clear();

    ready = false;

    if (effect == nullptr)
    {
        open_ = false;
        ready = true;
        return;
    }

    inputKind = effect->getInputKind();

    switch (inputKind)
    {
    case EffectInputKind::None:
        open_ = false;
        ready = true;
        return;

    case EffectInputKind::ChooseAdjacentEmptySpace:
        setupChooseAdjacentEmptySpace();
        break;

    case EffectInputKind::ChooseReachableSpace:
        setupChooseReachableSpace();
        break;

    case EffectInputKind::ChooseOpponentCardToBurn:
        setupChooseOpponentCardToBurn();
        break;

    case EffectInputKind::ChooseEnemyFighter:
        setupChooseEnemyFighter();
        break;
    }

    open_ = true;
}

// ============================================================
// SETUP HELPERS
// ============================================================

void EffectUI::setupChooseAdjacentEmptySpace()
{
    candidateSpaces.clear();

    if (fighter == nullptr || game == nullptr)
    {
        return;
    }

    Space *center = fighter->getPosition();

    if (center == nullptr)
    {
        return;
    }

    for (Space *neighbor : center->getNeighbors())
    {
        if (neighbor != nullptr && !neighbor->isOccupied())
        {
            candidateSpaces.push_back(neighbor);
        }
    }

    if (candidateSpaces.empty())
    {
        std::cout << "[!] No empty adjacent home available." << std::endl;
    }
}

void EffectUI::setupChooseReachableSpace()
{
    candidateSpaces.clear();

    if (fighter == nullptr || game == nullptr || effect == nullptr)
    {
        return;
    }

    int range = effect->getMoveRange();

    candidateSpaces = game->getBoard().getAvailableMoves(fighter, range);

    if (candidateSpaces.empty())
    {
        std::cout << "[!] No reachable empty home available." << std::endl;
    }
}

void EffectUI::setupChooseOpponentCardToBurn()
{
    candidateCards.clear();
    cardBoxes.clear();
    selectedCardIndex = -1;

    if (fighter == nullptr || game == nullptr)
    {
        return;
    }

    Player *owner = fighter->getOwner();

    if (owner == nullptr)
    {
        return;
    }

    Player *opponent = game->selectOpponent(*owner);

    if (opponent == nullptr)
    {
        return;
    }

    for (Card *card : opponent->getHand().getCards())
    {
        if (card != nullptr)
        {
            candidateCards.push_back(card);
        }

        if (candidateCards.size() >= 7)
        {
            break;
        }
    }

    if (candidateCards.empty())
    {
        std::cout << "[!] Opponent has no cards in hand." << std::endl;
        return;
    }

    layoutCardWindow();
}

void EffectUI::setupChooseEnemyFighter()
{
    candidateFighters.clear();
    fighterBoxes.clear();

    if (fighter == nullptr || game == nullptr)
    {
        return;
    }

    Player *owner = fighter->getOwner();

    if (owner == nullptr)
    {
        return;
    }

    Player *opponent = game->selectOpponent(*owner);

    if (opponent == nullptr)
    {
        return;
    }

    Hero *enemyHero = opponent->getHero();

    if (enemyHero != nullptr && enemyHero->isAlive())
    {
        candidateFighters.push_back(enemyHero);
    }

    for (Sidekick *sidekick : opponent->getSideKicks())
    {
        if (sidekick != nullptr && sidekick->isAlive())
        {
            candidateFighters.push_back(sidekick);
        }
    }

    if (candidateFighters.empty())
    {
        std::cout << "[!] Opponent has no available fighters." << std::endl;
        return;
    }

    layoutFighterWindow();
}

// ============================================================
// LAYOUT
// ============================================================

void EffectUI::layoutCardWindow()
{
    cardBoxes.clear();

    const int maxPerRow = 3;
    const float boxWidth = 220.0f;
    const float boxHeight = 320.0f;
    const float gapX = 25.0f;
    const float gapY = 25.0f;
    const float startY = 130.0f;

    int total = static_cast<int>(candidateCards.size());
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

            cardBoxes.push_back(box);
            index++;
        }
    }

    const float confirmWidth = 220.0f;
    const float confirmHeight = 55.0f;
    float lastRowBottom = startY + rowCount * (boxHeight + gapY);

    confirmButton = Rectangle{
        (GetScreenWidth() - confirmWidth) / 2.0f,
        lastRowBottom + 15.0f,
        confirmWidth,
        confirmHeight};
}

void EffectUI::layoutFighterWindow()
{
    fighterBoxes.clear();

    const float boxWidth = 220.0f;
    const float boxHeight = 320.0f;
    const float gapX = 25.0f;

    int count = static_cast<int>(candidateFighters.size());

    const float totalWidth =
        count * boxWidth + (count > 0 ? (count - 1) : 0) * gapX;

    const float startX = (GetScreenWidth() - totalWidth) / 2.0f;
    const float startY = 170.0f;

    for (int i = 0; i < count; i++)
    {
        Rectangle box{
            startX + i * (boxWidth + gapX),
            startY,
            boxWidth,
            boxHeight};

        fighterBoxes.push_back(box);
    }
}

// ============================================================
// UPDATE
// ============================================================

void EffectUI::update()
{
    if (!open_ || ready)
    {
        return;
    }

    // انتخاب خونه روی نقشه (ChooseAdjacentEmptySpace / ChooseReachableSpace)
    // اینجا کاری نمی‌کنیم؛ GameScreen خودش کلیک روی نقشه رو پردازش می‌کنه
    // و beginConfirmSpace(...) رو صدا می‌زنه.

    if (inputKind == EffectInputKind::ChooseAdjacentEmptySpace ||
        inputKind == EffectInputKind::ChooseReachableSpace)
    {
        return;
    }

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        return;
    }

    Vector2 mouse = GetMousePosition();

    if (inputKind == EffectInputKind::ChooseOpponentCardToBurn)
    {
        for (size_t i = 0; i < cardBoxes.size(); i++)
        {
            if (CheckCollisionPointRec(mouse, cardBoxes[i]))
            {
                selectedCardIndex = static_cast<int>(i);
                return;
            }
        }

        if (selectedCardIndex != -1 &&
            CheckCollisionPointRec(mouse, confirmButton))
        {
            choice.selectedCardIndex = selectedCardIndex;
            open_ = false;
            ready = true;
        }

        return;
    }

    if (inputKind == EffectInputKind::ChooseEnemyFighter)
    {
        for (size_t i = 0; i < fighterBoxes.size(); i++)
        {
            if (CheckCollisionPointRec(mouse, fighterBoxes[i]))
            {
                choice.selectedFighter = candidateFighters[i];
                open_ = false;
                ready = true;
                return;
            }
        }

        return;
    }
}

void EffectUI::selectSpace(Space *space)
{
    if (space == nullptr)
    {
        return;
    }

    for (Space *candidate : candidateSpaces)
    {
        if (candidate == space)
        {
            choice.selectedSpace = space;
            open_ = false;
            ready = true;
            return;
        }
    }
}

// ============================================================
// DRAW
// ============================================================

void EffectUI::draw()
{
    if (!open_ || assets == nullptr)
    {
        return;
    }

    Font font = assets->getGameFont();
    Vector2 mouse = GetMousePosition();

    if (inputKind == EffectInputKind::ChooseAdjacentEmptySpace ||
        inputKind == EffectInputKind::ChooseReachableSpace)
    {
        const char *title = "CHOOSE A DESTINATION ON THE MAP";
        const float titleSize = 28.0f;

        Vector2 titleTextSize = MeasureTextEx(font, title, titleSize, 1.5f);

        DrawTextEx(
            font, title,
            Vector2{
                (GetScreenWidth() - titleTextSize.x) / 2.0f,
                20.0f},
            titleSize, 1.5f, WHITE);

        return;
    }

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{0, 0, 0, 190});

    if (inputKind == EffectInputKind::ChooseOpponentCardToBurn)
    {
        const char *title = "CHOOSE A CARD TO BURN";
        const float titleSize = 32.0f;

        Vector2 titleTextSize = MeasureTextEx(font, title, titleSize, 2.0f);

        DrawTextEx(
            font, title,
            Vector2{(GetScreenWidth() - titleTextSize.x) / 2.0f, 70.0f},
            titleSize, 2.0f, WHITE);

        std::string heroName =
            (fighter != nullptr && fighter->getOwner() != nullptr &&
             fighter->getOwner()->getHero() != nullptr)
                ? fighter->getOwner()->getHero()->getName()
                : "";

        for (size_t i = 0; i < candidateCards.size(); i++)
        {
            Card *card = candidateCards[i];
            Rectangle box = cardBoxes[i];

            bool hovered = CheckCollisionPointRec(mouse, box);
            bool selected = (static_cast<int>(i) == selectedCardIndex);

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
                    const float padding = 14.0f;

                    Rectangle source{
                        0.0f, 0.0f,
                        static_cast<float>(texture.width),
                        static_cast<float>(texture.height)};

                    Rectangle destination{
                        box.x + padding,
                        box.y + padding,
                        box.width - 2.0f * padding,
                        box.height - 65.0f};

                    DrawTexturePro(texture, source, destination,
                                   Vector2{0.0f, 0.0f}, 0.0f, WHITE);
                }
            }

            std::string name = card->getName();
            Vector2 nameSize = MeasureTextEx(font, name.c_str(), 20.0f, 1.0f);

            DrawTextEx(
                font, name.c_str(),
                Vector2{
                    box.x + (box.width - nameSize.x) / 2.0f,
                    box.y + box.height - 40.0f},
                20.0f, 1.0f, WHITE);
        }

        bool confirmEnabled = (selectedCardIndex != -1);
        bool confirmHovered = CheckCollisionPointRec(mouse, confirmButton);

        Color confirmColor;
        if (!confirmEnabled)
            confirmColor = Color{30, 30, 30, 120};
        else if (confirmHovered)
            confirmColor = Color{140, 40, 40, 245};
        else
            confirmColor = Color{100, 30, 30, 235};

        DrawRectangleRounded(confirmButton, 1.0f, 20, confirmColor);

        const char *confirmText = "BURN";
        Vector2 confirmTextSize = MeasureTextEx(font, confirmText, 26.0f, 1.5f);

        DrawTextEx(
            font, confirmText,
            Vector2{
                confirmButton.x + (confirmButton.width - confirmTextSize.x) / 2.0f,
                confirmButton.y + (confirmButton.height - confirmTextSize.y) / 2.0f},
            26.0f, 1.5f,
            confirmEnabled ? WHITE : Color{150, 150, 150, 150});

        return;
    }

    if (inputKind == EffectInputKind::ChooseEnemyFighter)
    {
        const char *title = "CHOOSE AN OPPONENT'S FIGHTER";
        const float titleSize = 38.0f;

        Vector2 titleTextSize = MeasureTextEx(font, title, titleSize, 2.0f);

        DrawTextEx(
            font, title,
            Vector2{(GetScreenWidth() - titleTextSize.x) / 2.0f, 70.0f},
            titleSize, 2.0f, WHITE);

        for (size_t i = 0; i < candidateFighters.size(); i++)
        {
            Fighter *candidate = candidateFighters[i];
            Rectangle box = fighterBoxes[i];

            bool hovered = CheckCollisionPointRec(mouse, box);

            DrawRectangleRounded(box, 0.08f, 20,
                                 hovered ? Color{75, 75, 75, 245} : Color{35, 35, 35, 235});

            DrawRectangleRoundedLines(box, 0.08f, 20,
                                      hovered ? WHITE : Color{150, 150, 150, 255});

            std::string name = candidate->getName();
            Vector2 nameSize = MeasureTextEx(font, name.c_str(), 22.0f, 1.0f);

            DrawTextEx(
                font, name.c_str(),
                Vector2{
                    box.x + (box.width - nameSize.x) / 2.0f,
                    box.y + 245.0f},
                22.0f, 1.0f, WHITE);

            std::string healthText = "HP: " + std::to_string(candidate->getHealth());
            Vector2 healthSize = MeasureTextEx(font, healthText.c_str(), 18.0f, 1.0f);

            DrawTextEx(
                font, healthText.c_str(),
                Vector2{
                    box.x + (box.width - healthSize.x) / 2.0f,
                    box.y + 270.0f},
                18.0f, 1.0f, WHITE);
        }

        return;
    }
}

// ============================================================
// GETTERS
// ============================================================

bool EffectUI::isOpen() const
{
    return open_;
}

bool EffectUI::isReady() const
{
    return ready;
}

const EffectChoice &EffectUI::getChoice() const
{
    return choice;
}

void EffectUI::reset()
{
    open_ = false;
    ready = false;
    choice = EffectChoice{};
    candidateSpaces.clear();
    candidateCards.clear();
    candidateFighters.clear();
}

const std::vector<Space *> &EffectUI::getCandidateSpaces() const
{
    return candidateSpaces;
}

bool EffectUI::isChoosingSpace() const
{
    return open_ &&
           (inputKind == EffectInputKind::ChooseAdjacentEmptySpace ||
            inputKind == EffectInputKind::ChooseReachableSpace);
}