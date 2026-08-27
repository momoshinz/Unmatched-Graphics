#include "graphics/PlayerPanel.h"
#include "graphics/AssetManager.h"
#include "player/Player.h"
#include "fighter/Hero.h"
#include "fighter/Sidekick.h"
#include <raylib.h>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

PlayerPanel::PlayerPanel(AssetManager *assets)
    : assets(assets), bounds{0, 0, 0, 0}, textAnimationTimer(0.0f), textAnimationFinished(false),
      charRevealDelay(0.03f)
{
}

static void DrawGothicPanel(Rectangle bounds)
{
    const Color shadowColor = {0, 0, 0, 165};
    const Color panelColor = {18, 14, 14, 175};
    const Color darkGold = {92, 67, 35, 255};
    const Color gold = {155, 116, 60, 255};
    const Color brightGold = {205, 165, 95, 255};
    const Color deepFrame = {65, 47, 30, 255};

    Rectangle shadow = {bounds.x + 7.0f, bounds.y + 7.0f, bounds.width, bounds.height};

    DrawRectangle(
        static_cast<int>(shadow.x),
        static_cast<int>(shadow.y),
        static_cast<int>(shadow.width),
        static_cast<int>(shadow.height),
        shadowColor);

    DrawRectangle(
        static_cast<int>(bounds.x),
        static_cast<int>(bounds.y),
        static_cast<int>(bounds.width),
        static_cast<int>(bounds.height),
        panelColor);

    DrawRectangleLinesEx(bounds, 5.0f, darkGold);

    Rectangle frame1 = {bounds.x + 5.0f, bounds.y + 5.0f, bounds.width - 10.0f, bounds.height - 10.0f};
    DrawRectangleLinesEx(frame1, 2.5f, gold);

    Rectangle frame2 = {bounds.x + 11.0f, bounds.y + 11.0f, bounds.width - 22.0f, bounds.height - 22.0f};
    DrawRectangleLinesEx(frame2, 1.5f, deepFrame);

    Rectangle frame3 = {bounds.x + 17.0f, bounds.y + 17.0f, bounds.width - 34.0f, bounds.height - 34.0f};
    DrawRectangleLinesEx(frame3, 1.0f, darkGold);

    const float c = 38.0f;

    Vector2 tl = {bounds.x, bounds.y};
    DrawLineEx(Vector2{tl.x, tl.y + c}, Vector2{tl.x + c, tl.y}, 4.0f, gold);

    DrawLineEx(Vector2{tl.x + 6.0f, tl.y + c - 6.0f}, Vector2{tl.x + c - 6.0f, tl.y + 6.0f}, 2.0f, brightGold);

    DrawLineEx(Vector2{tl.x + 8.0f, tl.y + 4.0f}, Vector2{tl.x + 8.0f, tl.y + 19.0f}, 2.0f, gold);

    DrawLineEx(Vector2{tl.x + 4.0f, tl.y + 8.0f}, Vector2{tl.x + 19.0f, tl.y + 8.0f}, 2.0f, gold);

    DrawLineEx(Vector2{tl.x + 2.0f, tl.y + 26.0f}, Vector2{tl.x + 26.0f, tl.y + 2.0f}, 1.5f, darkGold);

    DrawPoly(Vector2{tl.x + 15.0f, tl.y + 15.0f}, 4, 8.0f, 45.0f, brightGold);

    DrawPoly(Vector2{tl.x + 15.0f, tl.y + 15.0f}, 4, 4.0f, 45.0f, darkGold);

    Vector2 tr = {bounds.x + bounds.width, bounds.y};

    DrawLineEx(Vector2{tr.x - c, tr.y}, Vector2{tr.x, tr.y + c}, 4.0f, gold);

    DrawLineEx(Vector2{tr.x - c + 6.0f, tr.y + 6.0f}, Vector2{tr.x - 6.0f, tr.y + c - 6.0f}, 2.0f, brightGold);

    DrawLineEx(Vector2{tr.x - 8.0f, tr.y + 4.0f}, Vector2{tr.x - 8.0f, tr.y + 19.0f}, 2.0f, gold);

    DrawLineEx(Vector2{tr.x - 4.0f, tr.y + 8.0f}, Vector2{tr.x - 19.0f, tr.y + 8.0f}, 2.0f, gold);

    DrawLineEx(Vector2{tr.x - 2.0f, tr.y + 26.0f}, Vector2{tr.x - 26.0f, tr.y + 2.0f}, 1.5f, darkGold);

    DrawPoly(Vector2{tr.x - 15.0f, tr.y + 15.0f}, 4, 8.0f, 45.0f, brightGold);

    DrawPoly(Vector2{tr.x - 15.0f, tr.y + 15.0f}, 4, 4.0f, 45.0f, darkGold);

    Vector2 bl = {bounds.x, bounds.y + bounds.height};

    DrawLineEx(Vector2{bl.x, bl.y - c}, Vector2{bl.x + c, bl.y}, 4.0f, gold);

    DrawLineEx(Vector2{bl.x + 6.0f, bl.y - c + 6.0f}, Vector2{bl.x + c - 6.0f, bl.y - 6.0f},
               2.0f,
               brightGold);

    DrawLineEx(Vector2{bl.x + 8.0f, bl.y - 4.0f}, Vector2{bl.x + 8.0f, bl.y - 19.0f},
               2.0f,
               gold);

    DrawLineEx(Vector2{bl.x + 4.0f, bl.y - 8.0f}, Vector2{bl.x + 19.0f, bl.y - 8.0f},
               2.0f,
               gold);

    DrawLineEx(Vector2{bl.x + 2.0f, bl.y - 26.0f}, Vector2{bl.x + 26.0f, bl.y - 2.0f},
               1.5f,
               darkGold);

    DrawPoly(Vector2{bl.x + 15.0f, bl.y - 15.0f}, 4, 8.0f, 45.0f, brightGold);

    DrawPoly(Vector2{bl.x + 15.0f, bl.y - 15.0f}, 4, 4.0f, 45.0f, darkGold);

    Vector2 br = {bounds.x + bounds.width, bounds.y + bounds.height};

    DrawLineEx(Vector2{br.x - c, br.y}, Vector2{br.x, br.y - c}, 4.0f, gold);

    DrawLineEx(Vector2{br.x - c + 6.0f, br.y - 6.0f}, Vector2{br.x - 6.0f, br.y - c + 6.0f}, 2.0f, brightGold);

    DrawLineEx(Vector2{br.x - 8.0f, br.y - 4.0f}, Vector2{br.x - 8.0f, br.y - 19.0f}, 2.0f, gold);

    DrawLineEx(Vector2{br.x - 4.0f, br.y - 8.0f}, Vector2{br.x - 19.0f, br.y - 8.0f}, 2.0f, gold);

    DrawLineEx(Vector2{br.x - 2.0f, br.y - 26.0f}, Vector2{br.x - 26.0f, br.y - 2.0f}, 1.5f, darkGold);

    DrawPoly(Vector2{br.x - 15.0f, br.y - 15.0f}, 4, 8.0f, 45.0f, brightGold);

    DrawPoly(Vector2{br.x - 15.0f, br.y - 15.0f}, 4, 4.0f, 45.0f, darkGold);

    Vector2 topCenter = {bounds.x + bounds.width / 2.0f, bounds.y};

    DrawLineEx(Vector2{topCenter.x - 38.0f, topCenter.y}, Vector2{topCenter.x - 13.0f, topCenter.y},
               2.0f,
               gold);

    DrawLineEx(Vector2{topCenter.x + 13.0f, topCenter.y}, Vector2{topCenter.x + 38.0f, topCenter.y},
               2.0f,
               gold);

    DrawPoly(Vector2{topCenter.x, topCenter.y + 8.0f}, 4, 12.0f, 45.0f, gold);

    DrawPoly(Vector2{topCenter.x, topCenter.y + 8.0f}, 4, 6.0f, 45.0f, brightGold);

    DrawTriangle(Vector2{topCenter.x - 22.0f, topCenter.y + 2.0f},
                 Vector2{topCenter.x - 13.0f, topCenter.y + 12.0f},
                 Vector2{topCenter.x - 4.0f, topCenter.y + 2.0f},
                 darkGold);

    DrawTriangle(Vector2{topCenter.x + 22.0f, topCenter.y + 2.0f},
                 Vector2{topCenter.x + 13.0f, topCenter.y + 12.0f},
                 Vector2{topCenter.x + 4.0f, topCenter.y + 2.0f},
                 darkGold);

    Vector2 bottomCenter = {bounds.x + bounds.width / 2.0f, bounds.y + bounds.height};

    DrawLineEx(Vector2{bottomCenter.x - 38.0f, bottomCenter.y},
               Vector2{bottomCenter.x - 13.0f, bottomCenter.y}, 2.0f, gold);

    DrawLineEx(Vector2{bottomCenter.x + 13.0f, bottomCenter.y},
               Vector2{bottomCenter.x + 38.0f, bottomCenter.y}, 2.0f, gold);

    DrawPoly(Vector2{bottomCenter.x, bottomCenter.y - 8.0f}, 4, 12.0f, 45.0f, gold);

    DrawPoly(Vector2{bottomCenter.x, bottomCenter.y - 8.0f}, 4, 6.0f, 45.0f, brightGold);

    DrawTriangle(Vector2{bottomCenter.x - 22.0f, bottomCenter.y - 2.0f},
                 Vector2{bottomCenter.x - 13.0f, bottomCenter.y - 12.0f},
                 Vector2{bottomCenter.x - 4.0f, bottomCenter.y - 2.0f},
                 darkGold);

    DrawTriangle(Vector2{bottomCenter.x + 22.0f, bottomCenter.y - 2.0f},
                 Vector2{bottomCenter.x + 13.0f, bottomCenter.y - 12.0f},
                 Vector2{bottomCenter.x + 4.0f, bottomCenter.y - 2.0f},
                 darkGold);
}

static std::vector<std::string> WrapText(Font font, const std::string &text,
                                         float fontSize,
                                         float spacing,
                                         float maxWidth)
{
    std::vector<std::string> lines;

    std::stringstream stream(text);

    std::string word;
    std::string currentLine;

    while (stream >> word)
    {
        std::string testLine;

        if (currentLine.empty())
        {
            testLine = word;
        }
        else
        {
            testLine = currentLine + " " + word;
        }

        Vector2 size = MeasureTextEx(font, testLine.c_str(), fontSize, spacing);

        if (size.x > maxWidth && !currentLine.empty())
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
    return lines;
}

static void DrawAnimatedText(Font font, const std::string &text,
                             Vector2 position,
                             float fontSize,
                             float spacing,
                             Color color,
                             int visibleCharacters)
{
    if (text.empty() || visibleCharacters <= 0)
    {
        return;
    }

    if (visibleCharacters > static_cast<int>(text.length()))
    {
        visibleCharacters = static_cast<int>(text.length());
    }

    std::string visibleText = text.substr(0, visibleCharacters);
    DrawTextEx(font, visibleText.c_str(), position, fontSize, spacing, color);
}

void PlayerPanel::setBounds(Rectangle bounds)
{
    this->bounds = bounds;
}

void PlayerPanel::resetTextAnimation()
{
    textAnimationTimer = 0.0f;
    textAnimationFinished = false;
}

bool PlayerPanel::isTextAnimationFinished() const
{
    return textAnimationFinished;
}

static std::string getAttackTypeText(AttackType type)
{
    if (type == AttackType::Melee)
    {
        return "ATTACK TYPE: MELEE";
    }
    return "ATTACK TYPE: RANGED";
}

void PlayerPanel::draw(const Player *player)
{
    if (assets == nullptr || player == nullptr)
    {
        return;
    }

    Font font = assets->getGameFont();

    if (!textAnimationFinished)
    {
        textAnimationTimer += GetFrameTime();
    }

    int visibleCharacters = static_cast<int>(textAnimationTimer / charRevealDelay);

    const float heroNameSize = 26.0f;
    const float heroInfoSize = 23.0f;

    const float abilityTitleSize = 25.0f;
    const float abilityTextSize = 24.0f;

    const float sidekickTitleSize = 24.0f;
    const float sidekickNameSize = 22.0f;
    const float sidekickInfoSize = 20.0f;

    const float spacing = 1.2f;

    const float padding = 28.0f;

    const float contentX = bounds.x + padding;
    const float contentWidth = bounds.width - 2.0f * padding;

    const float bottom = bounds.y + bounds.height - padding;

    DrawGothicPanel(bounds);

    float currentY = bounds.y + 30.0f;

    int consumedCharacters = 0;

    auto drawLine = [&](const std::string &text, Vector2 position, float fontSize)
    {
        int localVisible = visibleCharacters - consumedCharacters;
        if (localVisible > 0)
        {
            DrawAnimatedText(font, text, position, fontSize, spacing, WHITE, localVisible);
        }
        consumedCharacters += static_cast<int>(text.length());
    };

    Hero *hero = player->getHero();

    if (hero != nullptr)
    {
        std::string sidekickTextureKey = getSidekickTextureKey(player);
        Texture2D sidekickTexture = assets->getCharacter(sidekickTextureKey);

        if (sidekickTexture.id != 0)
        {
            const float maxImageWidth = bounds.width - 98.0f;
            float maxImageHeight = 168.0f;

            const float textureWidth = static_cast<float>(sidekickTexture.width);
            const float textureHeight = static_cast<float>(sidekickTexture.height);

            const float scaleX = maxImageWidth / textureWidth;
            const float scaleY = maxImageHeight / textureHeight;

            const float scale = std::min(scaleX, scaleY);

            const float imageWidth = textureWidth * scale;
            const float imageHeight = textureHeight * scale;

            const float imageX = bounds.x + (bounds.width - imageWidth) / 2.0f;
            const float imageY = currentY;

            Rectangle source{0.0f, 0.0f, textureWidth, textureHeight};
            Rectangle destination{imageX, imageY, imageWidth, imageHeight};

            DrawTexturePro(sidekickTexture, source, destination, Vector2{0.0f, 0.0f}, 0.0f, WHITE);

            currentY += imageHeight + 7.0f;
        }

        std::string heroName = hero->getName();

        int localHeroVisible = visibleCharacters - consumedCharacters;
        if (localHeroVisible > 0)
        {
            if (localHeroVisible > static_cast<int>(heroName.length()))
            {
                localHeroVisible = static_cast<int>(heroName.length());
            }

            std::string visibleName = heroName.substr(0, localHeroVisible);

            Vector2 visibleSize = MeasureTextEx(font, visibleName.c_str(), heroNameSize, spacing);

            DrawTextEx(font, visibleName.c_str(),
                       Vector2{bounds.x + (bounds.width - visibleSize.x) / 2.0f, currentY},
                       heroNameSize,
                       spacing,
                       WHITE);
        }
        consumedCharacters += static_cast<int>(heroName.length());
        currentY += heroNameSize + 10.0f;

        std::string heroHandText = "CARDS: " + std::to_string(player->getHand().getSize());
        drawLine(heroHandText, Vector2{contentX, currentY}, heroInfoSize);
        currentY += heroInfoSize + 8.0f;

        std::string heroHealthText = "HP: " + std::to_string(hero->getHealth()) +
                                     " / " +
                                     std::to_string(
                                         hero->getMaxHealth());

        drawLine(heroHealthText, Vector2{contentX, currentY}, heroInfoSize);
        currentY += heroInfoSize + 5.0f;

        std::string heroAttackTypeText = getAttackTypeText(hero->getAttackType());
        drawLine(heroAttackTypeText, Vector2{contentX, currentY}, heroInfoSize);
        currentY += heroInfoSize + 5.0f;

        std::string heroMovementText = "MOVEMENT: " + std::to_string(hero->getMovement());
        drawLine(heroMovementText, Vector2{contentX, currentY}, heroInfoSize);
        currentY += heroInfoSize + 20.0f;

        drawLine("SPECIAL ABILITY:", Vector2{contentX, currentY}, abilityTitleSize);
        currentY += abilityTitleSize + 5.0f;
        std::string abilityText = hero->getAbilityDescription();

        if (!abilityText.empty())
        {
            const float lineHeight = 20.0f;
            const int maxLines = 4;

            std::vector<std::string> lines = WrapText(
                font,
                abilityText,
                abilityTextSize,
                spacing,
                contentWidth);

            int linesDrawn = 0;

            for (const std::string &line : lines)
            {
                if (linesDrawn >= maxLines)
                {
                    break;
                }

                drawLine(line, Vector2{contentX, currentY}, abilityTextSize);

                currentY += lineHeight;
                linesDrawn++;
            }
        }
        currentY += 20.0f;
    }

    drawLine("SIDEKICKS", Vector2{contentX, currentY}, sidekickTitleSize);
    currentY += sidekickTitleSize + 6.0f;
    const std::vector<Sidekick *> &sidekicks = player->getSideKicks();

    if (sidekicks.empty())
    {
        if (currentY + sidekickInfoSize <= bottom)
        {
            drawLine("NO SIDEKICKS", Vector2{contentX, currentY}, sidekickInfoSize);
        }

        if (visibleCharacters >= consumedCharacters)
        {
            textAnimationFinished = true;
        }
        drawShowHandButton(player);
        return;
    }

    for (size_t i = 0; i < sidekicks.size(); ++i)
    {
        Sidekick *sidekick = sidekicks[i];

        if (sidekick == nullptr)
        {
            continue;
        }

        const float requiredHeight = sidekickNameSize + 5.0f + sidekickInfoSize +
                                     5.0f +
                                     sidekickInfoSize +
                                     5.0f +
                                     sidekickInfoSize +
                                     8.0f;

        if (currentY + requiredHeight > bottom)
        {
            break;
        }

        std::string sidekickName = sidekick->getName();
        drawLine(sidekickName, Vector2{contentX, currentY}, sidekickNameSize);
        currentY += sidekickNameSize + 10.0f;

        std::string sidekickHealthText = "HP: " + std::to_string(sidekick->getHealth()) +
                                         " / " +
                                         std::to_string(sidekick->getMaxHealth());

        drawLine(sidekickHealthText, Vector2{contentX, currentY}, sidekickInfoSize);
        currentY += sidekickInfoSize + 5.0f;

        std::string sidekickAttackTypeText = getAttackTypeText(sidekick->getAttackType());
        drawLine(sidekickAttackTypeText, Vector2{contentX, currentY}, sidekickInfoSize);
        currentY += sidekickInfoSize + 5.0f;

        std::string sidekickMovementText = "MOVEMENT: " + std::to_string(sidekick->getMovement());
        drawLine(sidekickMovementText, Vector2{contentX, currentY}, sidekickInfoSize);
        currentY += sidekickInfoSize + 8.0f;
    }

    if (visibleCharacters >= consumedCharacters)
    {
        textAnimationFinished = true;
    }
    drawShowHandButton(player);
}

std::string PlayerPanel::getHeroTextureKey(const Hero *hero) const
{
    if (hero == nullptr)
    {
        return "";
    }

    const std::string name = hero->getName();

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

std::string PlayerPanel::getSidekickTextureKey(const Player *player) const
{
    if (player == nullptr || player->getHero() == nullptr)
    {
        return "";
    }

    const std::string heroName = player->getHero()->getName();
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

void PlayerPanel::drawShowHandButton(const Player *player)
{
    if (assets == nullptr)
    {
        return;
    }

    Font font = assets->getGameFont();

    const float buttonWidth = 136.0f;
    const float buttonHeight = 34.0f;

    showHandButtonBounds = Rectangle{bounds.x + (bounds.width - buttonWidth) / 2.0f,
                                     bounds.y + bounds.height - buttonHeight - 20.0f,
                                     buttonWidth,
                                     buttonHeight};

    Vector2 mouse = GetMousePosition();

    bool hovered = CheckCollisionPointRec(mouse, showHandButtonBounds);

    Color buttonColor = hovered ? Color{75, 60, 40, 245} : Color{40, 32, 22, 235};

    DrawRectangleRounded(showHandButtonBounds, 1.0f, 12, buttonColor);
    DrawRectangleRoundedLines(showHandButtonBounds, 1.0f, 12,
                              hovered ? Color{205, 165, 95, 255}
                                      : Color{92, 67, 35, 255});

    const char *text = "SHOW HAND";
    const float fontSize = 22.0f;
    const float textSpacing = 1.0f;

    Vector2 textSize = MeasureTextEx(font, text, fontSize, textSpacing);

    DrawTextEx(font, text,
               Vector2{showHandButtonBounds.x + (showHandButtonBounds.width - textSize.x) / 2.0f,
                       showHandButtonBounds.y + (showHandButtonBounds.height - textSize.y) / 2.0f},
               fontSize,
               textSpacing,
               WHITE);
}

Rectangle PlayerPanel::getShowHandButtonBounds() const
{
    return showHandButtonBounds;
}