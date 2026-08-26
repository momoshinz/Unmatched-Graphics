#include "graphics/WinnerUI.h"
#include "graphics/AssetManager.h"
#include "fighter/Hero.h"

#include <string>

WinnerUI::WinnerUI(AssetManager *assets)
    : assets(assets)
{
}

// ============================================================
// GET HERO TEXTURE
// ============================================================

Texture2D WinnerUI::getHeroTextureForWinnerUI(
    AssetManager *assets,
    Hero *hero)
{
    if (assets == nullptr || hero == nullptr)
    {
        return {};
    }

    std::string name = hero->getName();

    if (name == "DRACULA")
    {
        return assets->getCharacter("dracula");
    }

    if (name == "SHERLOCK HOLMES")
    {
        return assets->getCharacter("sherlock_transparent");
    }

    if (name == "INVISIBLE MAN")
    {
        return assets->getCharacter("invisible_man");
    }

    return {};
}

// ============================================================
// OPEN
// ============================================================

void WinnerUI::open(Hero *winnerHero)
{
    this->winnerHero = winnerHero;

    open_ = true;
    returnRequested = false;

    // Reset button bounds
    returnTextBounds = Rectangle{};
}

// ============================================================
// IS OPEN
// ============================================================

bool WinnerUI::isOpen() const
{
    return open_;
}

// ============================================================
// UPDATE
// ============================================================

void WinnerUI::update()
{
    if (!open_)
    {
        return;
    }

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        return;
    }

    Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, returnTextBounds))
    {
        open_ = false;
        returnRequested = true;
    }
}

// ============================================================
// CONSUME RETURN REQUEST
// ============================================================

bool WinnerUI::consumeReturnToMenu()
{
    if (!returnRequested)
    {
        return false;
    }

    returnRequested = false;

    return true;
}

// ============================================================
// DRAW
// ============================================================

void WinnerUI::draw()
{
    if (!open_ ||
        assets == nullptr ||
        winnerHero == nullptr)
    {
        return;
    }

    Font font = assets->getGameFont();
    Font titleFont = assets->getTitleFont();

    // ========================================================
    // WINNER POPUP
    // ========================================================

    const float popupWidth = 520.0f;
    const float popupHeight = 590.0f;

    Rectangle popup{
        (GetScreenWidth() - popupWidth) / 2.0f,
        (GetScreenHeight() - popupHeight) / 2.0f - 30.0f,
        popupWidth,
        popupHeight};

    // Background
    DrawRectangleRounded(
        popup,
        0.06f,
        20,
        Color{25, 20, 18, 245});

    // Border
    DrawRectangleRoundedLines(
        popup,
        0.06f,
        20,
        Color{200, 170, 90, 255});

    // ========================================================
    // HERO IMAGE
    // ========================================================

    Texture2D texture =
        getHeroTextureForWinnerUI(assets, winnerHero);

    const float imagePadding = 30.0f;

    // Maximum area available for the hero image.
    const float maxImageWidth =
        popup.width - 2.0f * imagePadding;

    const float maxImageHeight = 330.0f;

    if (texture.id != 0 &&
        texture.width > 0 &&
        texture.height > 0)
    {
        const float textureWidth =
            static_cast<float>(texture.width);

        const float textureHeight =
            static_cast<float>(texture.height);

        // ----------------------------------------------------
        // KEEP ORIGINAL ASPECT RATIO
        // ----------------------------------------------------

        float scaleX =
            maxImageWidth / textureWidth;

        float scaleY =
            maxImageHeight / textureHeight;

        float scale =
            (scaleX < scaleY) ? scaleX : scaleY;

        float drawWidth =
            textureWidth * scale;

        float drawHeight =
            textureHeight * scale;

        // ----------------------------------------------------
        // CENTER IMAGE
        // ----------------------------------------------------

        float imageAreaX =
            popup.x + imagePadding;

        float imageAreaY =
            popup.y + imagePadding;

        float drawX =
            imageAreaX +
            (maxImageWidth - drawWidth) / 2.0f;

        float drawY =
            imageAreaY +
            (maxImageHeight - drawHeight) / 2.0f;

        Rectangle source{
            0.0f,
            0.0f,
            textureWidth,
            textureHeight};

        Rectangle destination{
            drawX,
            drawY,
            drawWidth,
            drawHeight};

        DrawTexturePro(
            texture,
            source,
            destination,
            Vector2{0.0f, 0.0f},
            0.0f,
            WHITE);
    }

    // ========================================================
    // HERO NAME
    // ========================================================

    std::string heroName =
        winnerHero->getName();

    const float nameFontSize = 34.0f;
    const float nameSpacing = 2.0f;

    Vector2 nameSize =
        MeasureTextEx(
            titleFont,
            heroName.c_str(),
            nameFontSize,
            nameSpacing);

    float nameY =
        popup.y +
        imagePadding +
        maxImageHeight +
        25.0f;

    DrawTextEx(
        titleFont,
        heroName.c_str(),
        Vector2{
            popup.x +
                (popup.width - nameSize.x) / 2.0f,
            nameY},
        nameFontSize,
        nameSpacing,
        Color{255, 215, 120, 255});

    // ========================================================
    // "IS THE WINNER"
    // ========================================================

    const char *subtitle =
        "IS THE WINNER";

    const float subtitleFontSize = 24.0f;
    const float subtitleSpacing = 1.5f;

    Vector2 subtitleSize =
        MeasureTextEx(
            font,
            subtitle,
            subtitleFontSize,
            subtitleSpacing);

    float subtitleY =
        nameY +
        nameSize.y +
        15.0f;

    DrawTextEx(
        font,
        subtitle,
        Vector2{
            popup.x +
                (popup.width - subtitleSize.x) / 2.0f,
            subtitleY},
        subtitleFontSize,
        subtitleSpacing,
        WHITE);

    // ========================================================
    // RETURN TO MAIN MENU
    // ========================================================

    const char *returnText =
        "RETURN TO MAIN MENU";

    const float returnFontSize = 25.0f;
    const float returnSpacing = 1.5f;

    Vector2 returnTextSize =
        MeasureTextEx(
            font,
            returnText,
            returnFontSize,
            returnSpacing);

    float returnX =
        (GetScreenWidth() - returnTextSize.x) / 2.0f;

    float returnY =
        popup.y +
        popup.height +
        25.0f;

    returnTextBounds = Rectangle{
        returnX - 10.0f,
        returnY - 8.0f,
        returnTextSize.x + 20.0f,
        returnTextSize.y + 16.0f};

    Vector2 mouse =
        GetMousePosition();

    bool hovered =
        CheckCollisionPointRec(
            mouse,
            returnTextBounds);

    Color textColor =
        hovered
            ? Color{255, 215, 120, 255}
            : Color{200, 200, 200, 255};

    DrawTextEx(
        font,
        returnText,
        Vector2{returnX, returnY},
        returnFontSize,
        returnSpacing,
        textColor);
}