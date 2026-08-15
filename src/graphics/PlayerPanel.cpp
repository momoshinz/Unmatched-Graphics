#include "graphics/PlayerPanel.h"
#include "graphics/AssetManager.h"
#include "player/Player.h"
#include "fighter/Hero.h"
#include "fighter/Sidekick.h"

#include <string>
#include <vector>

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
    const float abilityTitleSize = 24.0f;
    const float abilityTextSize = 22.0f;

    // Sidekicks - bigger
    const float sidekickTitleSize = 24.0f;
    const float sidekickNameSize = 22.0f;
    const float sidekickInfoSize = 20.0f;

    const float spacing = 0.8f;

    // ========================================================
    // GENERAL PANEL SETTINGS
    // ========================================================

    const float padding = 22.0f;

    const float contentX =
        bounds.x + padding;

    const float contentWidth =
        bounds.width - 2.0f * padding;

    const float bottom =
        bounds.y + bounds.height - padding;

    // ========================================================
    // PANEL
    // ========================================================

    DrawRectangleRounded(
        bounds,
        0.12f,
        30,
        Color{
            0,
            0,
            0,
            180});

    // ========================================================
    // CURRENT Y
    // ========================================================

    float currentY =
        bounds.y + 15.0f;

    // ========================================================
    // HERO
    // ========================================================

    Hero *hero =
        player->getHero();

    if (hero != nullptr)
    {
        // ====================================================
        // HERO IMAGE
        // ====================================================

        std::string heroTextureKey =
            getHeroTextureKey(hero);

        Texture2D heroTexture =
            assets->getCharacter(
                heroTextureKey);

        if (heroTexture.id != 0)
        {
            const float heroImageHeight = 165.0f;

            Rectangle source{
                0.0f,
                0.0f,
                static_cast<float>(
                    heroTexture.width),
                static_cast<float>(
                    heroTexture.height)};

            Rectangle destination{
                bounds.x + 65.0f,
                currentY,
                bounds.width - 130.0f,
                heroImageHeight};

            DrawTexturePro(
                heroTexture,
                source,
                destination,
                Vector2{
                    0.0f,
                    0.0f},
                0.0f,
                WHITE);

            currentY +=
                heroImageHeight + 7.0f;
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
            heroInfoSize + 15.0f;

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
        currentY += 15.0f;
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
    // SIDEKICK IMAGE
    // ========================================================

    std::string sidekickTextureKey =
        getSidekickTextureKey(player);

    Texture2D sidekickTexture =
        assets->getCharacter(
            sidekickTextureKey);

    if (sidekickTexture.id != 0)
    {
        const float sidekickImageHeight = 85.0f;

        Rectangle source{
            0.0f,
            0.0f,
            static_cast<float>(
                sidekickTexture.width),
            static_cast<float>(
                sidekickTexture.height)};

        Rectangle destination{
            bounds.x + 55.0f,
            currentY,
            bounds.width - 110.0f,
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
            sidekickImageHeight + 8.0f;
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
            sidekick-> getName();

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
            sidekickNameSize + 5.0f;

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

    return "";
}