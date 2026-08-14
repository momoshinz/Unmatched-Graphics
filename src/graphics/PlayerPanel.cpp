#include "graphics/PlayerPanel.h"
#include "graphics/AssetManager.h"
#include "player/Player.h"
#include "fighter/Hero.h"
#include "fighter/Sidekick.h"
#include <string>
#include <vector>

PlayerPanel::PlayerPanel(
    AssetManager *assets)
    : assets(assets),
      bounds{0, 0, 0, 0}
{
}

// ============================================================
// SET BOUNDS
// ============================================================

void PlayerPanel::setBounds(
    Rectangle bounds)
{
    this->bounds = bounds;
}

// ============================================================
// DRAW
// ============================================================

void PlayerPanel::draw(
    const Player *player) const
{
    if (assets == nullptr ||
        player == nullptr)
    {
        return;
    }

    // ========================================================
    // TEXT SETTINGS
    // ========================================================

    const float heroNameFontSize = 28.0f;

    const float heroInfoFontSize = 21.0f;

    const float abilityTitleFontSize = 21.0f;

    const float abilityFontSize = 16.0f;

    const float sidekickTitleFontSize = 24.0f;

    const float sidekickNameFontSize = 20.0f;

    const float sidekickInfoFontSize = 17.0f;

    const float normalSpacing = 1.0f;

    const float heroNameSpacing = 1.5f;

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

    Font font =
        assets->getGameFont();

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
            Rectangle source{
                0.0f,
                0.0f,
                static_cast<float>(
                    heroTexture.width),
                static_cast<float>(
                    heroTexture.height)};

            Rectangle destination{
                bounds.x + 40.0f,
                bounds.y + 25.0f,
                bounds.width - 80.0f,
                210.0f};

            DrawTexturePro(
                heroTexture,
                source,
                destination,
                Vector2{
                    0.0f,
                    0.0f},
                0.0f,
                WHITE);
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
                heroNameFontSize,
                heroNameSpacing);

        DrawTextEx(
            font,
            heroName.c_str(),

            Vector2{
                bounds.x +
                    (bounds.width -
                     heroNameMeasure.x) /
                        2.0f,

                bounds.y + 245.0f},

            heroNameFontSize,
            heroNameSpacing,
            WHITE);

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
                bounds.x + 30.0f,
                bounds.y + 290.0f},

            heroInfoFontSize,
            normalSpacing,
            WHITE);

        // ====================================================
        // HERO ATTACK TYPE
        // ====================================================

        std::string heroAttackTypeText;

        if (hero->getAttackType() ==
            AttackType::Melee)
        {
            heroAttackTypeText =
                "ATTACK TYPE: MELEE";
        }
        else
        {
            heroAttackTypeText =
                "ATTACK TYPE: RANGED";
        }

        DrawTextEx(
            font,
            heroAttackTypeText.c_str(),

            Vector2{
                bounds.x + 30.0f,
                bounds.y + 325.0f},

            heroInfoFontSize,
            normalSpacing,
            WHITE);

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
                bounds.x + 30.0f,
                bounds.y + 360.0f},

            heroInfoFontSize,
            normalSpacing,
            WHITE);

        // ====================================================
        // SPECIAL ABILITY TITLE
        // ====================================================

        DrawTextEx(
            font,
            "SPECIAL ABILITY:",

            Vector2{
                bounds.x + 30.0f,
                bounds.y + 400.0f},

            abilityTitleFontSize,
            normalSpacing,
            WHITE);

        // ====================================================
        // SPECIAL ABILITY TEXT
        // ====================================================

        std::string abilityText =
            hero->getAbilityDescription();

        if (!abilityText.empty())
        {
            const float abilityX =
                bounds.x + 30.0f;

            const float abilityMaxWidth =
                bounds.width - 60.0f;

            float abilityY =
                bounds.y + 430.0f;

            const float abilityLineHeight =
                21.0f;

            std::string currentLine;
            std::string currentWord;

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
                            abilityFontSize,
                            normalSpacing);

                    if (testSize.x >
                            abilityMaxWidth &&
                        !currentLine.empty())
                    {
                        DrawTextEx(
                            font,
                            currentLine.c_str(),

                            Vector2{
                                abilityX,
                                abilityY},

                            abilityFontSize,
                            normalSpacing,
                            WHITE);

                        abilityY +=
                            abilityLineHeight;

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

            if (!currentLine.empty())
            {
                DrawTextEx(
                    font,
                    currentLine.c_str(),

                    Vector2{
                        abilityX,
                        abilityY},

                    abilityFontSize,
                    normalSpacing,
                    WHITE);
            }
        }
    }

    // ========================================================
    // SIDEKICKS
    // ========================================================

    const std::vector<Sidekick *> &sidekicks =
        player->getSideKicks();

    // ========================================================
    // SIDEKICKS TITLE
    // ========================================================

    DrawTextEx(
        font,
        "SIDEKICKS",

        Vector2{
            bounds.x + 30.0f,
            bounds.y + 510.0f},

        sidekickTitleFontSize,
        normalSpacing,
        WHITE);

    // ========================================================
    // NO SIDEKICKS
    // ========================================================

    if (sidekicks.empty())
    {
        DrawTextEx(
            font,
            "NO SIDEKICKS",

            Vector2{
                bounds.x + 30.0f,
                bounds.y + 550.0f},

            sidekickInfoFontSize,
            normalSpacing,
            WHITE);

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
        Rectangle source{
            0.0f,
            0.0f,
            static_cast<float>(
                sidekickTexture.width),
            static_cast<float>(
                sidekickTexture.height)};

        Rectangle destination{
            bounds.x + 40.0f,
            bounds.y + 545.0f,
            bounds.width - 80.0f,
            100.0f};

        DrawTexturePro(
            sidekickTexture,
            source,
            destination,
            Vector2{
                0.0f,
                0.0f},
            0.0f,
            WHITE);
    }

    // ========================================================
    // SIDEKICK INFORMATION
    // ========================================================

    float sidekickY =
        bounds.y + 660.0f;

    for (Sidekick *sidekick : sidekicks)
    {
        if (sidekick == nullptr)
        {
            continue;
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
                bounds.x + 30.0f,
                sidekickY},

            sidekickNameFontSize,
            normalSpacing,
            WHITE);

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
                bounds.x + 30.0f,
                sidekickY + 27.0f},

            sidekickInfoFontSize,
            normalSpacing,
            WHITE);

        // ====================================================
        // SIDEKICK ATTACK TYPE
        // ====================================================

        std::string sidekickAttackTypeText;

        if (sidekick->getAttackType() ==
            AttackType::Melee)
        {
            sidekickAttackTypeText =
                "ATTACK TYPE: MELEE";
        }
        else
        {
            sidekickAttackTypeText =
                "ATTACK TYPE: RANGED";
        }

        DrawTextEx(
            font,
            sidekickAttackTypeText.c_str(),

            Vector2{
                bounds.x + 30.0f,
                sidekickY + 52.0f},

            sidekickInfoFontSize,
            normalSpacing,
            WHITE);

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
                bounds.x + 30.0f,
                sidekickY + 77.0f},

            sidekickInfoFontSize,
            normalSpacing,
            WHITE);

        // ====================================================
        // NEXT SIDEKICK
        // ====================================================

        sidekickY += 115.0f;
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