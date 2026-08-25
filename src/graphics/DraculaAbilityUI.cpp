#include "graphics/DraculaAbilityUI.h"
#include "graphics/AssetManager.h"
#include "game/Game.h"
#include "player/Player.h"
#include "fighter/Fighter.h"
#include "fighter/Hero.h"
#include "board/Board.h"
#include "board/Space.h"

DraculaAbilityUI::DraculaAbilityUI(AssetManager *assets)
    : assets(assets)
{
}

static Texture2D getFighterTextureForDraculaUI(AssetManager *assets, Fighter *fighter)
{
    if (assets == nullptr || fighter == nullptr)
    {
        return {};
    }

    std::string name = fighter->getName();

    if (name == "DRACULA")
        return assets->getCharacter("dracula");
    if (name == "* Sister 1")
        return assets->getCharacter("sister1");
    if (name == "* Sister 2")
        return assets->getCharacter("sister2");
    if (name == "* Sister 3")
        return assets->getCharacter("sister3");
    if (name == "SHERLOCK HOLMES")
        return assets->getCharacter("sherlock_art");
    if (name == "* Dr. Watson")
        return assets->getCharacter("drwatson");
    if (name == "INVISIBLE MAN")
        return assets->getCharacter("invisible_man");

    return {};
}

void DraculaAbilityUI::open(Game *game, Hero *dracula, Player *player)
{
    this->game = game;
    this->dracula = dracula;
    this->player = player;

    finished = false;
    selectedIndex = -1;
    candidateFighters.clear();
    fighterBoxes.clear();

    if (game == nullptr || dracula == nullptr || player == nullptr)
    {
        state = DraculaAbilityState::CLOSED;
        finished = true;
        return;
    }

    state = DraculaAbilityState::ASK;
    layoutAskButtons();
}

void DraculaAbilityUI::layoutAskButtons()
{
    const float buttonWidth = 160.0f;
    const float buttonHeight = 55.0f;
    const float gap = 30.0f;

    const float totalWidth = 2.0f * buttonWidth + gap;
    const float startX = (GetScreenWidth() - totalWidth) / 2.0f;
    const float buttonY = GetScreenHeight() - 115.0f;

    yesButton = Rectangle{startX, buttonY, buttonWidth, buttonHeight};
    noButton = Rectangle{startX + buttonWidth + gap, buttonY, buttonWidth, buttonHeight};
}

void DraculaAbilityUI::layoutNoTargetMessage()
{
    const float buttonWidth = 140.0f;
    const float buttonHeight = 48.0f;

    backButton = Rectangle{(GetScreenWidth() - buttonWidth) / 2.0f, GetScreenHeight() - 80.0f,
                           buttonWidth,
                           buttonHeight};
}

void DraculaAbilityUI::layoutTargetSelection()
{
    fighterBoxes.clear();

    const float boxWidth = 220.0f;
    const float boxHeight = 320.0f;
    const float gapX = 25.0f;

    int count = static_cast<int>(candidateFighters.size());

    const float totalWidth = count * boxWidth + (count > 0 ? (count - 1) : 0) * gapX;
    const float startX = (GetScreenWidth() - totalWidth) / 2.0f;
    const float startY = 170.0f;

    for (int i = 0; i < count; i++)
    {
        Rectangle box{startX + i * (boxWidth + gapX), startY, boxWidth, boxHeight};
        fighterBoxes.push_back(box);
    }

    const float confirmWidth = 220.0f;
    const float confirmHeight = 60.0f;

    confirmButton = Rectangle{(GetScreenWidth() - confirmWidth) / 2.0f, startY + boxHeight + 25.0f,
                              confirmWidth,
                              confirmHeight};
}

void DraculaAbilityUI::update()
{
    if (state == DraculaAbilityState::CLOSED)
    {
        return;
    }

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        return;
    }

    Vector2 mouse = GetMousePosition();

    if (state == DraculaAbilityState::ASK)
    {
        if (CheckCollisionPointRec(mouse, yesButton))
        {
            candidateFighters.clear();

            if (game != nullptr && dracula != nullptr && dracula->getPosition() != nullptr)
            {
                Board &board = game->getBoard();

                for (Space *space : board.getAdjacentSpaces(dracula->getPosition()))
                {
                    if (space == nullptr)
                        continue;

                    Fighter *fighter = space->getFighter();

                    if (fighter != nullptr && fighter->isAlive())
                    {
                        candidateFighters.push_back(fighter);
                    }
                }
            }

            if (candidateFighters.empty())
            {
                state = DraculaAbilityState::NO_TARGET_MESSAGE;
                layoutNoTargetMessage();
            }
            else
            {
                state = DraculaAbilityState::SELECT_TARGET;
                layoutTargetSelection();
            }
        }
        else if (CheckCollisionPointRec(mouse, noButton))
        {
            state = DraculaAbilityState::CLOSED;
            finished = true;
        }

        return;
    }

    if (state == DraculaAbilityState::NO_TARGET_MESSAGE)
    {
        if (CheckCollisionPointRec(mouse, backButton))
        {
            state = DraculaAbilityState::CLOSED;
            finished = true;
        }

        return;
    }

    if (state == DraculaAbilityState::SELECT_TARGET)
    {
        for (size_t i = 0; i < fighterBoxes.size(); i++)
        {
            if (CheckCollisionPointRec(mouse, fighterBoxes[i]))
            {
                selectedIndex = static_cast<int>(i);
                return;
            }
        }

        if (selectedIndex != -1 && CheckCollisionPointRec(mouse, confirmButton))
        {
            applyAbility(candidateFighters[selectedIndex]);
            state = DraculaAbilityState::CLOSED;
            finished = true;
        }

        return;
    }
}

void DraculaAbilityUI::applyAbility(Fighter *target)
{
    if (target == nullptr || player == nullptr)
    {
        return;
    }

    target->takeDamage(1);
    player->drawCardToHand();
}

void DraculaAbilityUI::draw()
{
    if (state == DraculaAbilityState::CLOSED || assets == nullptr)
    {
        return;
    }

    Font font = assets->getGameFont();
    Vector2 mouse = GetMousePosition();

    if (state == DraculaAbilityState::ASK)
    {
        const char *question = "Do you want to use Dracula's special ability?";
        const float questionSize = 30.0f;

        Vector2 questionTextSize = MeasureTextEx(font, question, questionSize, 1.5f);

        DrawTextEx(font, question,
                   Vector2{(GetScreenWidth() - questionTextSize.x) / 2.0f, GetScreenHeight() - 170.0f},
                   questionSize, 1.5f, WHITE);

        bool yesHovered = CheckCollisionPointRec(mouse, yesButton);
        DrawRectangleRounded(yesButton, 1.0f, 20, yesHovered ? Color{75, 75, 75, 245} : Color{35, 35, 35, 235});
        Vector2 yesSize = MeasureTextEx(font, "YES", 24.0f, 1.5f);
        DrawTextEx(font, "YES",
                   Vector2{yesButton.x + (yesButton.width - yesSize.x) / 2.0f, yesButton.y + (yesButton.height - yesSize.y) / 2.0f},
                   24.0f, 1.5f, WHITE);

        bool noHovered = CheckCollisionPointRec(mouse, noButton);
        DrawRectangleRounded(noButton, 1.0f, 20, noHovered ? Color{75, 75, 75, 245} : Color{35, 35, 35, 235});
        Vector2 noSize = MeasureTextEx(font, "NO", 24.0f, 1.5f);
        DrawTextEx(font, "NO",
                   Vector2{noButton.x + (noButton.width - noSize.x) / 2.0f, noButton.y + (noButton.height - noSize.y) / 2.0f},
                   24.0f, 1.5f, WHITE);

        return;
    }

    if (state == DraculaAbilityState::NO_TARGET_MESSAGE)
    {
        const char *message = "NO ADJACENT FIGHTER TO TARGET!";
        const float fontSize = 24.0f;
        const float spacing = 1.5f;

        Vector2 textSize = MeasureTextEx(font, message, fontSize, spacing);

        float textX = (GetScreenWidth() - textSize.x) / 2.0f;
        float textY = GetScreenHeight() - 135.0f;

        Rectangle background{textX - 20.0f, textY - 10.0f, textSize.x + 40.0f, textSize.y + 20.0f};

        DrawRectangleRounded(background, 0.2f, 12, Color{80, 20, 20, 220});
        DrawRectangleRoundedLines(background, 0.2f, 12, Color{220, 100, 100, 255});

        DrawTextEx(font, message, Vector2{textX, textY}, fontSize, spacing, WHITE);

        bool hovered = CheckCollisionPointRec(mouse, backButton);
        Color buttonColor = hovered ? Color{75, 75, 75, 245} : Color{35, 35, 35, 235};

        DrawRectangleRounded(backButton, 1.0f, 20, buttonColor);
        DrawRectangleRoundedLines(backButton, 1.0f, 20, hovered ? WHITE : Color{150, 150, 150, 255});

        const char *backText = "OK";
        Vector2 backTextSize = MeasureTextEx(font, backText, 22.0f, 1.5f);

        DrawTextEx(font, backText,
                   Vector2{
                       backButton.x + (backButton.width - backTextSize.x) / 2.0f,
                       backButton.y + (backButton.height - backTextSize.y) / 2.0f},
                   22.0f, 1.5f, WHITE);

        return;
    }

    if (state == DraculaAbilityState::SELECT_TARGET)
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{0, 0, 0, 190});

        const char *title = "CHOOSE A FIGHTER TO DAMAGE";
        const float titleSize = 38.0f;

        Vector2 titleTextSize = MeasureTextEx(font, title, titleSize, 2.0f);

        DrawTextEx(font, title,
                   Vector2{(GetScreenWidth() - titleTextSize.x) / 2.0f, 70.0f},
                   titleSize, 2.0f, WHITE);

        for (size_t i = 0; i < candidateFighters.size(); i++)
        {
            Fighter *candidate = candidateFighters[i];
            Rectangle box = fighterBoxes[i];

            bool hovered = CheckCollisionPointRec(mouse, box);
            bool selected = (static_cast<int>(i) == selectedIndex);

            Color boxColor;
            if (selected)
                boxColor = Color{120, 85, 40, 245};
            else if (hovered)
                boxColor = Color{75, 75, 75, 245};
            else
                boxColor = Color{35, 35, 35, 235};

            DrawRectangleRounded(box, 0.08f, 20, boxColor);
            DrawRectangleRoundedLines(box, 0.08f, 20, (hovered || selected) ? WHITE : Color{150, 150, 150, 255});

            Texture2D texture = getFighterTextureForDraculaUI(assets, candidate);

            if (texture.id != 0)
            {
                const float imagePadding = 15.0f;

                Rectangle source{0.0f, 0.0f,
                                 static_cast<float>(texture.width),
                                 static_cast<float>(texture.height)};

                Rectangle destination{box.x + imagePadding, box.y + imagePadding,
                                      box.width - 2.0f * imagePadding,
                                      220.0f};

                DrawTexturePro(texture, source, destination, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
            }

            std::string name = candidate->getName();
            Vector2 nameSize = MeasureTextEx(font, name.c_str(), 22.0f, 1.0f);

            DrawTextEx(font, name.c_str(),
                       Vector2{box.x + (box.width - nameSize.x) / 2.0f, box.y + 245.0f},
                       22.0f, 1.0f, WHITE);

            std::string hpText = "HP : " + std::to_string(candidate->getHealth());
            Vector2 hpSize = MeasureTextEx(font, hpText.c_str(), 18.0f, 1.0f);

            DrawTextEx(font, hpText.c_str(),
                       Vector2{box.x + (box.width - hpSize.x) / 2.0f, box.y + 270.0f},
                       18.0f, 1.0f, WHITE);
        }

        bool confirmEnabled = (selectedIndex != -1);
        bool confirmHovered = CheckCollisionPointRec(mouse, confirmButton);

        Color confirmColor;
        if (!confirmEnabled)
            confirmColor = Color{30, 30, 30, 120};
        else if (confirmHovered)
            confirmColor = Color{75, 75, 75, 245};
        else
            confirmColor = Color{35, 35, 35, 235};

        DrawRectangleRounded(confirmButton, 1.0f, 20, confirmColor);

        const char *confirmText = "CONFIRM";
        Vector2 confirmTextSize = MeasureTextEx(font, confirmText, 26.0f, 1.5f);

        DrawTextEx(font, confirmText,
                   Vector2{
                       confirmButton.x + (confirmButton.width - confirmTextSize.x) / 2.0f,
                       confirmButton.y + (confirmButton.height - confirmTextSize.y) / 2.0f},
                   26.0f, 1.5f, confirmEnabled ? WHITE : Color{150, 150, 150, 150});

        return;
    }
}

bool DraculaAbilityUI::isOpen() const
{
    return state != DraculaAbilityState::CLOSED;
}

bool DraculaAbilityUI::consumeFinished()
{
    if (!finished)
    {
        return false;
    }
    finished = false;
    return true;
}