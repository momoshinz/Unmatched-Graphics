#include "graphics/MainMenu.h"

MainMenu::MainMenu(AssetManager *assets)
    : assets(assets)
{
}

// =========================================
// UPDATE
// =========================================

void MainMenu::update()
{
}

// =========================================
// DRAW
// =========================================

void MainMenu::draw()
{
    if (assets == nullptr)
    {
        return;
    }

    Texture2D background =
        assets->getMainMenuBackground();

    Font font =
        assets->getGameFont();

    Font titleFont = assets->getTitleFont();

    

    // =========================================
    // Background
    // =========================================

    if (background.id != 0)
    {
        DrawTexturePro(
            background,

            Rectangle{
                0,
                0,
                static_cast<float>(background.width),
                static_cast<float>(background.height)},

            Rectangle{
                0,
                0,
                static_cast<float>(GetScreenWidth()),
                static_cast<float>(GetScreenHeight())},

            Vector2{0, 0},

            0.0f,

            WHITE);
    }

    // =========================================
    // Game Title
    // =========================================

    const char *title = "UNMATCHED";

    const float titleSize = 60.0f;
    const float titleSpacing = 2.0f;

    Vector2 titleTextSize =
        MeasureTextEx(
            titleFont,
            title,
            titleSize,
            titleSpacing);

    const float titleX =
        (GetScreenWidth() - titleTextSize.x) / 2.0f;

    const float titleY = 100.0f;

    DrawTextEx(
        titleFont,
        title,

        Vector2{
            titleX,
            titleY},

        titleSize,
        titleSpacing,

        WHITE);

    // =========================================
    // Buttons
    // =========================================

    const float buttonWidth = 360.0f;
    const float buttonHeight = 75.0f;

    const float buttonX =
        (GetScreenWidth() - buttonWidth) / 2.0f;

    const float firstButtonY = 500.0f;
    const float buttonGap = 25.0f;

    Rectangle newGameButton{
        buttonX,
        firstButtonY,
        buttonWidth,
        buttonHeight};

    Rectangle loadGameButton{
        buttonX,
        firstButtonY + buttonHeight + buttonGap,
        buttonWidth,
        buttonHeight};

    Rectangle exitButton{
        buttonX,
        firstButtonY + 2.0f * (buttonHeight + buttonGap),
        buttonWidth,
        buttonHeight};

    // =========================================
    // Mouse position
    // =========================================

    Vector2 mousePosition =
        GetMousePosition();

    // =========================================
    // Button colors
    // =========================================

    Color normalColor{
        30,
        30,
        30,
        220};

    Color hoverColor{
        60,
        60,
        60,
        235};

    // =========================================
    // Button drawing
    // =========================================

    Color newGameColor =
        CheckCollisionPointRec(
            mousePosition,
            newGameButton)
            ? hoverColor
            : normalColor;

    Color loadGameColor =
        CheckCollisionPointRec(
            mousePosition,
            loadGameButton)
            ? hoverColor
            : normalColor;

    Color exitColor =
        CheckCollisionPointRec(
            mousePosition,
            exitButton)
            ? hoverColor
            : normalColor;

    // کپسول اول
    DrawRectangleRounded(
        newGameButton,
        1.5f,
        32,
        newGameColor);

    // کپسول دوم
    DrawRectangleRounded(
        loadGameButton,
        1.5f,
        32,
        loadGameColor);

    // کپسول سوم
    DrawRectangleRounded(
        exitButton,
        1.5f,
        32,
        exitColor);

    // =========================================
    // Button text
    // =========================================

    const float buttonFontSize = 35.0f;
    const float buttonSpacing = 2.0f;

    const char *newGameText = "NEW GAME";
    const char *loadGameText = "LOAD GAME";
    const char *exitText = "EXIT";

    Vector2 newGameTextSize =
        MeasureTextEx(
            font,
            newGameText,
            buttonFontSize,
            buttonSpacing);

    Vector2 loadGameTextSize =
        MeasureTextEx(
            font,
            loadGameText,
            buttonFontSize,
            buttonSpacing);

    Vector2 exitTextSize =
        MeasureTextEx(
            font,
            exitText,
            buttonFontSize,
            buttonSpacing);

    // NEW GAME
    DrawTextEx(
        font,
        newGameText,

        Vector2{
            newGameButton.x +
                (newGameButton.width - newGameTextSize.x) / 2.0f,

            newGameButton.y +
                (newGameButton.height - newGameTextSize.y) / 2.0f},

        buttonFontSize,
        buttonSpacing,

        WHITE);

    // LOAD GAME
    DrawTextEx(
        font,
        loadGameText,

        Vector2{
            loadGameButton.x +
                (loadGameButton.width - loadGameTextSize.x) / 2.0f,

            loadGameButton.y +
                (loadGameButton.height - loadGameTextSize.y) / 2.0f},

        buttonFontSize,
        buttonSpacing,

        WHITE);

    // EXIT
    DrawTextEx(
        font,
        exitText,

        Vector2{
            exitButton.x +
                (exitButton.width - exitTextSize.x) / 2.0f,

            exitButton.y +
                (exitButton.height - exitTextSize.y) / 2.0f},

        buttonFontSize,
        buttonSpacing,

        WHITE);
}

// =========================================
// HANDLE INPUT
// =========================================

int MainMenu::handleInput() const
{
    if (assets == nullptr)
    {
        return 0;
    }

    const float buttonWidth = 360.0f;
    const float buttonHeight = 75.0f;

    const float buttonX =
        (GetScreenWidth() - buttonWidth) / 2.0f;

    const float firstButtonY = 500.0f;
    const float buttonGap = 25.0f;

    // =========================================
    // Button areas
    // =========================================

    Rectangle newGameButton{
        buttonX,
        firstButtonY,
        buttonWidth,
        buttonHeight};

    Rectangle loadGameButton{
        buttonX,
        firstButtonY + buttonHeight + buttonGap,
        buttonWidth,
        buttonHeight};

    Rectangle exitButton{
        buttonX,
        firstButtonY + 2.0f * (buttonHeight + buttonGap),
        buttonWidth,
        buttonHeight};

    // =========================================
    // Mouse
    // =========================================

    Vector2 mousePosition =
        GetMousePosition();

    // فقط کلیک داخل محدوده‌ی دکمه قبول می‌شود
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (CheckCollisionPointRec(
                mousePosition,
                newGameButton))
        {
            return 1;
        }

        if (CheckCollisionPointRec(
                mousePosition,
                loadGameButton))
        {
            return 2;
        }

        if (CheckCollisionPointRec(
                mousePosition,
                exitButton))
        {
            return 3;
        }
    }

    return 0;
}