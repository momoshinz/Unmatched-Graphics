#include "graphics/PlayerPanel.h"
#include "graphics/AssetManager.h"
#include "player/Player.h"
#include "fighter/Hero.h"
#include "fighter/Sidekick.h"
#include <string>
#include <vector>

// ============================================================
// GOTHIC PANEL
// ============================================================

static void DrawGothicPanel(Rectangle bounds)
{
    // ========================================================
    // OUTER SHADOW
    // ========================================================

    Rectangle shadow = {
        bounds.x + 6.0f,
        bounds.y + 6.0f,
        bounds.width,
        bounds.height};

    DrawRectangle(
        static_cast<int>(shadow.x),
        static_cast<int>(shadow.y),
        static_cast<int>(shadow.width),
        static_cast<int>(shadow.height),
        Color{0, 0, 0, 150});

    // ========================================================
    // OUTER PANEL
    // ========================================================

    DrawRectangle(
        static_cast<int>(bounds.x),
        static_cast<int>(bounds.y),
        static_cast<int>(bounds.width),
        static_cast<int>(bounds.height),
        Color{18, 14, 14, 170});

    // ========================================================
    // OUTER FRAME
    // ========================================================

    DrawRectangleLinesEx(
        bounds,
        4.0f,
        Color{105, 82, 48, 255});

    // ========================================================
    // SECOND FRAME
    // ========================================================

    Rectangle innerFrame = {
        bounds.x + 10.0f,
        bounds.y + 10.0f,
        bounds.width - 20.0f,
        bounds.height - 20.0f};

    DrawRectangleLinesEx(
        innerFrame,
        2.0f,
        Color{160, 125, 70, 220});

    // ========================================================
    // INNER DARK AREA
    // ========================================================

    Rectangle contentFrame = {
        bounds.x + 17.0f,
        bounds.y + 17.0f,
        bounds.width - 34.0f,
        bounds.height - 34.0f};

    DrawRectangleLinesEx(
        contentFrame,
        1.5f,
        Color{75, 58, 40, 220});

    // ========================================================
    // GOTHIC CORNER SIZE
    // ========================================================

    const float c = 28.0f;

    // ========================================================
    // TOP LEFT
    // ========================================================

    DrawLineEx(
        Vector2{bounds.x, bounds.y + c},
        Vector2{bounds.x + c, bounds.y},
        4.0f,
        Color{160, 125, 70, 255});

    DrawLineEx(
        Vector2{bounds.x + 7, bounds.y + c - 5},
        Vector2{bounds.x + c - 5, bounds.y + 7},
        2.0f,
        Color{90, 70, 45, 255});

    // ========================================================
    // TOP RIGHT
    // ========================================================

    DrawLineEx(
        Vector2{bounds.x + bounds.width - c, bounds.y},
        Vector2{bounds.x + bounds.width, bounds.y + c},
        4.0f,
        Color{160, 125, 70, 255});

    DrawLineEx(
        Vector2{
            bounds.x + bounds.width - c + 5,
            bounds.y + 7},
        Vector2{
            bounds.x + bounds.width - 7,
            bounds.y + c - 5},
        2.0f,
        Color{90, 70, 45, 255});

    // ========================================================
    // BOTTOM LEFT
    // ========================================================

    DrawLineEx(
        Vector2{
            bounds.x,
            bounds.y + bounds.height - c},
        Vector2{
            bounds.x + c,
            bounds.y + bounds.height},
        4.0f,
        Color{160, 125, 70, 255});

    DrawLineEx(
        Vector2{
            bounds.x + 7,
            bounds.y + bounds.height - c + 5},
        Vector2{
            bounds.x + c - 5,
            bounds.y + bounds.height - 7},
        2.0f,
        Color{90, 70, 45, 255});

    // ========================================================
    // BOTTOM RIGHT
    // ========================================================

    DrawLineEx(
        Vector2{
            bounds.x + bounds.width - c,
            bounds.y + bounds.height},
        Vector2{
            bounds.x + bounds.width,
            bounds.y + bounds.height - c},
        4.0f,
        Color{160, 125, 70, 255});

    DrawLineEx(
        Vector2{
            bounds.x + bounds.width - c + 5,
            bounds.y + bounds.height - 7},
        Vector2{
            bounds.x + bounds.width - 7,
            bounds.y + bounds.height - c + 5},
        2.0f,
        Color{90, 70, 45, 255});

    // ========================================================
    // TOP CENTER ORNAMENT
    // ========================================================

    Vector2 topCenter = {
        bounds.x + bounds.width / 2.0f,
        bounds.y};

    DrawTriangle(
        Vector2{
            topCenter.x,
            topCenter.y + 2.0f},

        Vector2{
            topCenter.x - 15.0f,
            topCenter.y + 18.0f},

        Vector2{
            topCenter.x + 15.0f,
            topCenter.y + 18.0f},

        Color{105, 82, 48, 255});

    DrawTriangle(
        Vector2{
            topCenter.x,
            topCenter.y + 5.0f},

        Vector2{
            topCenter.x - 7.0f,
            topCenter.y + 13.0f},

        Vector2{
            topCenter.x + 7.0f,
            topCenter.y + 13.0f},

        Color{190, 150, 85, 255});

    // ========================================================
    // BOTTOM CENTER ORNAMENT
    // ========================================================

    Vector2 bottomCenter = {
        bounds.x + bounds.width / 2.0f,
        bounds.y + bounds.height};

    DrawTriangle(
        Vector2{
            bottomCenter.x,
            bottomCenter.y - 2.0f},

        Vector2{
            bottomCenter.x - 15.0f,
            bottomCenter.y - 18.0f},

        Vector2{
            bottomCenter.x + 15.0f,
            bottomCenter.y - 18.0f},

        Color{105, 82, 48, 255});

    DrawTriangle(
        Vector2{
            bottomCenter.x,
            bottomCenter.y - 5.0f},

        Vector2{
            bottomCenter.x - 7.0f,
            bottomCenter.y - 13.0f},

        Vector2{
            bottomCenter.x + 7.0f,
            bottomCenter.y - 13.0f},

        Color{190, 150, 85, 255});
}

PlayerPanel::PlayerPanel(AssetManager *assets)
    : assets(assets),
      bounds{0, 0, 0, 0}
{
}

// ============================================================
// SET BOUNDS
// ============================================================

void PlayerPanel::setBounds(Rectangle bounds)
{
    this->bounds = bounds;
}

// ============================================================
// HELPER
// ============================================================

static std::string getAttackTypeText(AttackType type)
{
    if (type == AttackType::Melee)
    {
        return "ATTACK TYPE: MELEE";
    }

    return "ATTACK TYPE: RANGED";
}

// ============================================================
// DRAW
// ============================================================

void PlayerPanel::draw(const Player *player) const
{
    if (assets == nullptr || player == nullptr)
    {
        return;
    }

    Font font = assets->getGameFont();

    // ========================================================
    // TEXT SIZES
    // ========================================================

    const float heroNameSize = 26.0f;

    // Hero information - bigger
    const float heroInfoSize = 23.0f;

    // Special ability - bigger
    const float abilityTitleSize = 25.0f;
    const float abilityTextSize = 24.0f;

    // Sidekicks - bigger
    const float sidekickTitleSize = 24.0f;
    const float sidekickNameSize = 22.0f;
    const float sidekickInfoSize = 20.0f;

    const float spacing = 1.2f;

    // ========================================================
    // GENERAL PANEL SETTINGS
    // ========================================================

    const float padding = 28.0f;

    const float contentX =
        bounds.x + padding;

    const float contentWidth =
        bounds.width - 2.0f * padding;

    const float bottom =
        bounds.y + bounds.height - padding;

    // ========================================================
    // PANEL
    // ========================================================

    DrawGothicPanel(bounds);

    // ========================================================
    // CURRENT Y
    // ========================================================

    float currentY =
        bounds.y + 30.0f;

    // ========================================================
    // HERO
    // ========================================================

    Hero *hero =
        player->getHero();

    if (hero != nullptr)
    {
        // ====================================================
        // HERO AND SIDEKICK IMAGE
        // ====================================================

        std::string sidekickTextureKey =
            getSidekickTextureKey(player);

        Texture2D sidekickTexture =
            assets->getCharacter(
                sidekickTextureKey);

        if (sidekickTexture.id != 0)
        {
            const float sidekickImageHeight = 190.0f;

            Rectangle source{
                0.0f,
                0.0f,
                static_cast<float>(
                    sidekickTexture.width),
                static_cast<float>(
                    sidekickTexture.height)};

            Rectangle destination{
                bounds.x + 45.0f,
                currentY,
                bounds.width - 90.0f,
                sidekickImageHeight};

            DrawTexturePro(
                sidekickTexture,
                source,
                destination,
                Vector2{
                    0.0f,
                    0.0f},
                0.0f,
                WHITE);

            currentY +=
                sidekickImageHeight + 7.0f;
        }

        // ====================================================
        // HERO NAME
        // ====================================================

        std::string heroName =
            hero->getName();

        Vector2 heroNameMeasure =
            MeasureTextEx(
                font,
                heroName.c_str(),
                heroNameSize,
                spacing);

        DrawTextEx(
            font,
            heroName.c_str(),

            Vector2{
                bounds.x +
                    (bounds.width -
                     heroNameMeasure.x) /
                        2.0f,

                currentY},

            heroNameSize,
            spacing,
            WHITE);

        currentY +=
            heroNameSize + 10.0f;

        // ====================================================
        // HERO HAND
        // ====================================================

        std::string heroHandText =
            "CARDS: " +
            std::to_string(
                player->getHand().getSize());

        DrawTextEx(
            font,
            heroHandText.c_str(),

            Vector2{
                contentX,
                currentY},

            heroInfoSize,
            spacing,
            WHITE);

        currentY +=
            heroInfoSize + 8.0f;

        // ====================================================
        // HERO HP
        // ====================================================

        std::string heroHealthText =
            "HP: " +
            std::to_string(
                hero->getHealth()) +
            " / " +
            std::to_string(
                hero->getMaxHealth());

        DrawTextEx(
            font,
            heroHealthText.c_str(),

            Vector2{
                contentX,
                currentY},

            heroInfoSize,
            spacing,
            WHITE);

        currentY +=
            heroInfoSize + 5.0f;

        // ====================================================
        // HERO ATTACK TYPE
        // ====================================================

        std::string heroAttackTypeText =
            getAttackTypeText(
                hero->getAttackType());

        DrawTextEx(
            font,
            heroAttackTypeText.c_str(),

            Vector2{
                contentX,
                currentY},

            heroInfoSize,
            spacing,
            WHITE);

        currentY +=
            heroInfoSize + 5.0f;

        // ====================================================
        // HERO MOVEMENT
        // ====================================================

        std::string heroMovementText =
            "MOVEMENT: " +
            std::to_string(
                hero->getMovement());

        DrawTextEx(
            font,
            heroMovementText.c_str(),

            Vector2{
                contentX,
                currentY},

            heroInfoSize,
            spacing,
            WHITE);

        // Extra space between MOVEMENT
        // and SPECIAL ABILITY
        currentY +=
            heroInfoSize + 20.0f;

        // ====================================================
        // SPECIAL ABILITY TITLE
        // ====================================================

        DrawTextEx(
            font,
            "SPECIAL ABILITY:",

            Vector2{
                contentX,
                currentY},

            abilityTitleSize,
            spacing,
            WHITE);

        currentY +=
            abilityTitleSize + 5.0f;

        // ====================================================
        // SPECIAL ABILITY TEXT
        // ====================================================

        std::string abilityText =
            hero->getAbilityDescription();

        if (!abilityText.empty())
        {
            const float maxAbilityWidth =
                contentWidth;

            const float lineHeight =
                20.0f;

            std::string currentLine;
            std::string currentWord;

            int lineCount = 0;

            // Maximum number of lines
            // so Sidekicks still have room.
            const int maxLines = 4;

            for (size_t i = 0;
                 i <= abilityText.length();
                 ++i)
            {
                if (i < abilityText.length() &&
                    abilityText[i] != ' ')
                {
                    currentWord +=
                        abilityText[i];

                    continue;
                }

                if (!currentWord.empty())
                {
                    std::string testLine;

                    if (currentLine.empty())
                    {
                        testLine =
                            currentWord;
                    }
                    else
                    {
                        testLine =
                            currentLine +
                            " " +
                            currentWord;
                    }

                    Vector2 testSize =
                        MeasureTextEx(
                            font,
                            testLine.c_str(),
                            abilityTextSize,
                            spacing);

                    if (testSize.x >
                            maxAbilityWidth &&
                        !currentLine.empty())
                    {
                        if (lineCount < maxLines)
                        {
                            DrawTextEx(
                                font,
                                currentLine.c_str(),

                                Vector2{
                                    contentX,
                                    currentY},

                                abilityTextSize,
                                spacing,
                                WHITE);

                            currentY +=
                                lineHeight;

                            lineCount++;
                        }

                        currentLine =
                            currentWord;
                    }
                    else
                    {
                        currentLine =
                            testLine;
                    }

                    currentWord.clear();
                }
            }

            // Draw final line
            if (!currentLine.empty() &&
                lineCount < maxLines)
            {
                DrawTextEx(
                    font,
                    currentLine.c_str(),

                    Vector2{
                        contentX,
                        currentY},

                    abilityTextSize,
                    spacing,
                    WHITE);

                currentY +=
                    lineHeight;

                lineCount++;
            }
        }

        // Extra space between
        // SPECIAL ABILITY and SIDEKICKS
        currentY += 20.0f;
    }

    // ========================================================
    // SIDEKICKS
    // ========================================================

    const std::vector<Sidekick *> &sidekicks =
        player->getSideKicks();

    // ========================================================
    // SIDEKICK TITLE
    // ========================================================

    DrawTextEx(
        font,
        "SIDEKICKS",

        Vector2{
            contentX,
            currentY},

        sidekickTitleSize,
        spacing,
        WHITE);

    currentY +=
        sidekickTitleSize + 6.0f;

    // ========================================================
    // NO SIDEKICKS
    // ========================================================

    if (sidekicks.empty())
    {
        if (currentY + sidekickInfoSize <= bottom)
        {
            DrawTextEx(
                font,
                "NO SIDEKICKS",

                Vector2{
                    contentX,
                    currentY},

                sidekickInfoSize,
                spacing,
                WHITE);
        }

        return;
    }

    // ========================================================
    // SIDEKICK INFORMATION
    // ========================================================

    for (size_t i = 0;
         i < sidekicks.size();
         ++i)
    {
        Sidekick *sidekick =
            sidekicks[i];

        if (sidekick == nullptr)
        {
            continue;
        }

        // ====================================================
        // REQUIRED HEIGHT
        // ====================================================

        const float requiredHeight =
            sidekickNameSize +
            5.0f +
            sidekickInfoSize +
            5.0f +
            sidekickInfoSize +
            5.0f +
            sidekickInfoSize +
            8.0f;

        if (currentY + requiredHeight >
            bottom)
        {
            break;
        }

        // ====================================================
        // SIDEKICK NAME
        // ====================================================

        std::string sidekickName =
            sidekick->getName();

        DrawTextEx(
            font,
            sidekickName.c_str(),

            Vector2{
                contentX,
                currentY},

            sidekickNameSize,
            spacing,
            WHITE);

        currentY +=
            sidekickNameSize + 10.0f;

        // ====================================================
        // SIDEKICK HP
        // ====================================================

        std::string sidekickHealthText =
            "HP: " +
            std::to_string(
                sidekick->getHealth()) +
            " / " +
            std::to_string(
                sidekick->getMaxHealth());

        DrawTextEx(
            font,
            sidekickHealthText.c_str(),

            Vector2{
                contentX,
                currentY},

            sidekickInfoSize,
            spacing,
            WHITE);

        currentY +=
            sidekickInfoSize + 5.0f;

        // ====================================================
        // SIDEKICK ATTACK TYPE
        // ====================================================

        std::string sidekickAttackTypeText =
            getAttackTypeText(
                sidekick->getAttackType());

        DrawTextEx(
            font,
            sidekickAttackTypeText.c_str(),

            Vector2{
                contentX,
                currentY},

            sidekickInfoSize,
            spacing,
            WHITE);

        currentY +=
            sidekickInfoSize + 5.0f;

        // ====================================================
        // SIDEKICK MOVEMENT
        // ====================================================

        std::string sidekickMovementText =
            "MOVEMENT: " +
            std::to_string(
                sidekick->getMovement());

        DrawTextEx(
            font,
            sidekickMovementText.c_str(),

            Vector2{
                contentX,
                currentY},

            sidekickInfoSize,
            spacing,
            WHITE);

        currentY +=
            sidekickInfoSize + 8.0f;
    }
}

// ============================================================
// HERO TEXTURE KEY
// ============================================================

std::string PlayerPanel::getHeroTextureKey(
    const Hero *hero) const
{
    if (hero == nullptr)
    {
        return "";
    }

    const std::string name =
        hero->getName();

    if (name == "DRACULA")
    {
        return "dracula";
    }

    if (name == "SHERLOCK HOLMES")
    {
        return "sherlock";
    }

    if (name == "INVISIBLE MAN")
    {
        return "invisible_man";
    }

    return "";
}

// ============================================================
// SIDEKICK TEXTURE KEY
// ============================================================

std::string PlayerPanel::getSidekickTextureKey(
    const Player *player) const
{
    if (player == nullptr ||
        player->getHero() == nullptr)
    {
        return "";
    }

    const std::string heroName =
        player->getHero()->getName();

    if (heroName == "DRACULA")
    {
        return "sisters";
    }

    if (heroName == "SHERLOCK HOLMES")
    {
        return "watson";
    }

    if (heroName == "INVISIBLE MAN")
    {
        return "foggy";
    }

    return "";
}