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

    // فقط فایترهای زنده را وارد لیست می‌کنیم
    for (Fighter *fighter : fighters)
    {
        if (fighter == nullptr)
        {
            continue;
        }

        if (!fighter->isAlive())
        {
            continue;
        }

        selectableFighters.push_back(fighter);
    }

    // اگر هیچ فایتر زنده‌ای وجود ندارد
    if (selectableFighters.empty())
    {
        open = false;
        return;
    }

    // ========================================================
    // محاسبه جای کارت‌های فایتر
    // ========================================================

    const float boxWidth = 220.0f;
    const float boxHeight = 300.0f;

    const float gap = 25.0f;

    const float totalWidth =
        selectableFighters.size() * boxWidth +
        (selectableFighters.size() - 1) * gap;

    const float startX =
        (GetScreenWidth() - totalWidth) / 2.0f;

    const float startY = 170.0f;

    for (size_t i = 0;
         i < selectableFighters.size();
         i++)
    {
        Rectangle box{
            startX +
                i * (boxWidth + gap),

            startY,

            boxWidth,
            boxHeight};

        fighterBoxes.push_back(box);
    }

    open = true;

    std::cout
        << "Attack UI opened."
        << std::endl;
}

// ============================================================
// GET CHARACTER TEXTURE
// ============================================================

static Texture2D getFighterTexture(
    AssetManager *assets,
    Fighter *fighter)
{
    if (assets == nullptr ||
        fighter == nullptr)
    {
        return {};
    }

    std::string name =
        fighter->getName();

    // -----------------------------
    // Dracula
    // -----------------------------

    if (name == "DRACULA")
    {
        return assets->getCharacter("dracula");
    }

    // -----------------------------
    // Dracula Sisters
    // -----------------------------

    if (name == "* Sister 1")
    {
        return assets->getCharacter("sister1");
    }

    if (name == "* Sister 2")
    {
        return assets->getCharacter("sister2");
    }

    if (name == "* Sister 3")
    {
        return assets->getCharacter("sister3");
    }

    // -----------------------------
    // Sherlock
    // -----------------------------

    if (name == "SHERLOCK HOLMES")
    {
        return assets->getCharacter("sherlock_art");
    }

    // -----------------------------
    // Doctor Watson
    // -----------------------------

    if (name == "* Dr. Watson")
    {
        return assets->getCharacter("watson");
    }

    // -----------------------------
    // Invisible Man
    // -----------------------------

    if (name == "INVISIBLE MAN")
    {
        return assets->getCharacter("invisible_man");
    }

    return {};
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

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        return;
    }

    Vector2 mouse =
        GetMousePosition();

    for (size_t i = 0;
         i < fighterBoxes.size();
         i++)
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

    Font font =
        assets->getGameFont();

    // ========================================================
    // Dark overlay
    // ========================================================

    DrawRectangle(
        0,
        0,
        GetScreenWidth(),
        GetScreenHeight(),
        Color{
            0,
            0,
            0,
            190});

    // ========================================================
    // Title
    // ========================================================

    const char *title =
        "CHOOSE YOUR ATTACKER";

    const float titleSize =
        38.0f;

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
            (GetScreenWidth() -
             titleMeasure.x) /
                2.0f,

            70.0f},

        titleSize,
        2.0f,
        WHITE);

    // ========================================================
    // Fighter boxes
    // ========================================================

    Vector2 mouse =
        GetMousePosition();

    for (size_t i = 0;
         i < selectableFighters.size();
         i++)
    {
        Fighter *fighter =
            selectableFighters[i];

        Rectangle box =
            fighterBoxes[i];

        bool hovered =
            CheckCollisionPointRec(
                mouse,
                box);

        // ----------------------------------------------------
        // Box
        // ----------------------------------------------------

        Color boxColor =
            hovered
                ? Color{75, 75, 75, 245}
                : Color{35, 35, 35, 235};

        DrawRectangleRounded(
            box,
            0.08f,
            20,
            boxColor);

        // ----------------------------------------------------
        // Border
        // ----------------------------------------------------

        DrawRectangleRoundedLines(
            box,
            0.08f,
            20,
            hovered
                ? WHITE
                : Color{150, 150, 150, 255});

        // ----------------------------------------------------
        // Fighter image
        // ----------------------------------------------------

        Texture2D texture =
            getFighterTexture(
                assets,
                fighter);

        if (texture.id != 0)
        {
            const float imagePadding =
                15.0f;

            Rectangle source{
                0.0f,
                0.0f,
                static_cast<float>(
                    texture.width),
                static_cast<float>(
                    texture.height)};

            Rectangle destination{
                box.x + imagePadding,
                box.y + imagePadding,

                box.width -
                    2.0f * imagePadding,

                220.0f};

            DrawTexturePro(
                texture,
                source,
                destination,
                Vector2{0.0f, 0.0f},
                0.0f,
                WHITE);
        }

        // ----------------------------------------------------
        // Fighter name
        // ----------------------------------------------------

        std::string name =
            fighter->getName();

        Vector2 nameSize =
            MeasureTextEx(
                font,
                name.c_str(),
                22.0f,
                1.0f);

        DrawTextEx(
            font,
            name.c_str(),

            Vector2{
                box.x +
                    (box.width -
                     nameSize.x) /
                        2.0f,

                box.y +
                    245.0f},

            22.0f,
            1.0f,
            WHITE);

        // ----------------------------------------------------
        // Health
        // ----------------------------------------------------

        std::string healthText =
            "HP: " +
            std::to_string(
                fighter->getHealth());

        Vector2 healthSize =
            MeasureTextEx(
                font,
                healthText.c_str(),
                18.0f,
                1.0f);

        DrawTextEx(
            font,
            healthText.c_str(),

            Vector2{
                box.x +
                    (box.width -
                     healthSize.x) /
                        2.0f,

                box.y +
                    270.0f},

            18.0f,
            1.0f,
            WHITE);
    }
}

// ============================================================
// GETTERS
// ============================================================

bool AttackUI::isOpen() const
{
    return open;
}

Fighter *AttackUI::getSelectedAttacker() const
{
    return selectedAttacker;
}

Fighter *AttackUI::getSelectedTarget() const
{
    return selectedTarget;
}