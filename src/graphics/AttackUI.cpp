#include "graphics/AttackUI.h"

#include "graphics/AssetManager.h"
#include "fighter/Fighter.h"

#include <iostream>
#include <string>

AttackUI::AttackUI(AssetManager *assets)
    : assets(assets)
{
}

// ============================================================
// OPEN ATTACK
// ============================================================

void AttackUI::openAttack(
    const std::vector<Fighter *> &fighters)
{
    selectableFighters.clear();
    fighterBoxes.clear();

    selectedAttacker = nullptr;
    selectedTarget = nullptr;

    for (Fighter *fighter : fighters)
    {
        if (fighter != nullptr &&
            fighter->isAlive())
        {
            selectableFighters.push_back(fighter);
        }
    }

    open = true;
}

// ============================================================
// UPDATE
// ============================================================

void AttackUI::update()
{
    if (!open)
    {
        return;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mouse =
            GetMousePosition();

        for (size_t i = 0;
             i < fighterBoxes.size();
             ++i)
        {
            if (CheckCollisionPointRec(
                    mouse,
                    fighterBoxes[i]))
            {
                selectedAttacker =
                    selectableFighters[i];

                std::cout
                    << "Attacker selected : "
                    << selectedAttacker->getName()
                    << std::endl;

                open = false;

                return;
            }
        }
    }
}

// ============================================================
// DRAW
// ============================================================

void AttackUI::draw()
{
    if (!open ||
        assets == nullptr)
    {
        return;
    }

    // ========================================================
    // DARK OVERLAY
    // ========================================================

    DrawRectangle(
        0,
        0,
        GetScreenWidth(),
        GetScreenHeight(),
        Color{0, 0, 0, 180});

    // ========================================================
    // POPUP
    // ========================================================

    const float popupWidth = 900.0f;
    const float popupHeight = 600.0f;

    Rectangle popup{
        (GetScreenWidth() - popupWidth) / 2.0f,
        (GetScreenHeight() - popupHeight) / 2.0f,
        popupWidth,
        popupHeight};

    DrawRectangleRounded(
        popup,
        0.04f,
        20,
        Color{25, 20, 18, 250});

    DrawRectangleRoundedLines(
        popup,
        0.04f,
        20,
        Color{180, 160, 130, 255});

    // ========================================================
    // TITLE
    // ========================================================

    Font font =
        assets->getGameFont();

    const char *title =
        "CHOOSE YOUR ATTACKER";

    const float titleSize = 32.0f;

    Vector2 titleMeasure =
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
                (popup.width -
                 titleMeasure.x) / 2.0f,

            popup.y + 25.0f},

        titleSize,
        2.0f,
        WHITE);

    // ========================================================
    // FIGHTER BOXES
    // ========================================================

    fighterBoxes.clear();

    const float boxWidth = 180.0f;
    const float boxHeight = 260.0f;

    const float gap = 25.0f;

    const int fighterCount =
        static_cast<int>(
            selectableFighters.size());

    if (fighterCount == 0)
    {
        const char *text =
            "No living fighters.";

        Vector2 textSize =
            MeasureTextEx(
                font,
                text,
                24.0f,
                1.0f);

        DrawTextEx(
            font,
            text,

            Vector2{
                popup.x +(popup.width -
                     textSize.x) / 2.0f,

                popup.y +
                    popup.height / 2.0f},

            24.0f,
            1.0f,
            WHITE);

        return;
    }

    const float totalWidth =
        fighterCount * boxWidth +
        (fighterCount - 1) * gap;

    float startX =
        popup.x +
        (popup.width - totalWidth) / 2.0f;

    const float boxY =
        popup.y + 130.0f;

    for (int i = 0;
         i < fighterCount;
         ++i)
    {
        Fighter *fighter =
            selectableFighters[i];

        if (fighter == nullptr)
        {
            continue;
        }

        Rectangle box{
            startX +
                i * (boxWidth + gap),

            boxY,

            boxWidth,
            boxHeight};

        fighterBoxes.push_back(box);

        // ====================================================
        // BOX
        // ====================================================

        Vector2 mouse =
            GetMousePosition();

        bool hovered =
            CheckCollisionPointRec(
                mouse,
                box);

        Color boxColor =
            hovered
                ? Color{75, 75, 75, 255}
                : Color{45, 40, 38, 255};

        DrawRectangleRounded(
            box,
            0.08f,
            20,
            boxColor);

        DrawRectangleRoundedLines(
            box,
            0.08f,
            20,
            Color{150, 130, 110, 255});

        // ====================================================
        // FIGHTER NAME
        // ====================================================

        std::string name =
            fighter->getName();

        Vector2 nameSize =
            MeasureTextEx(
                font,
                name.c_str(),
                20.0f,
                1.0f);

        DrawTextEx(
            font,
            name.c_str(),

            Vector2{
                box.x +
                    (box.width -
                     nameSize.x) / 2.0f,

                box.y + 15.0f},

            20.0f,
            1.0f,
            WHITE);

        // ====================================================
        // IMAGE
        // ====================================================

        // فعلاً این قسمت را خالی گذاشته‌ایم تا
        // AssetManager را با ساختار واقعی پروژه‌ات
        // وصل کنیم.
        //
        // بعد از اینکه getterهای Texture2D مربوط به
        // Dracula / Sisters / Sherlock / Watson /
        // Invisible Man را ببینیم، دقیقاً همینجا
        // عکس هر Fighter قرار می‌گیرد.

        DrawRectangle(
            static_cast<int>(box.x + 20),
            static_cast<int>(box.y + 55),
            static_cast<int>(box.width - 40),
            150,
            Color{30, 30, 30, 255});

        const char *selectText =
            "CLICK TO SELECT";

        Vector2 selectSize =
            MeasureTextEx(
                font,
                selectText,
                14.0f,
                1.0f);

        DrawTextEx(
            font,
            selectText,

            Vector2{
                box.x +
                    (box.width -
                     selectSize.x) / 2.0f,

                box.y + 220.0f},

            14.0f,
            1.0f,
            WHITE);
    }
}

// ============================================================
// IS OPEN
// ============================================================

bool AttackUI::isOpen() const
{
    return open;
}

// ============================================================
// GET SELECTED ATTACKER
// ============================================================

Fighter *AttackUI::getSelectedAttacker() const
{
    return selectedAttacker;
}

// ============================================================
// GET SELECTED TARGET
// ============================================================

Fighter *AttackUI::getSelectedTarget() const
{
    return selectedTarget;
}