#include "graphics/LoadingScreen.h"
#include <iostream>

LoadingScreen::LoadingScreen(
    AssetManager &assets,
    float duration)
    : assets(assets),
      elapsedTime(0.0f),
      duration(duration)
{
}

void LoadingScreen::update()
{
    elapsedTime += GetFrameTime();
}

void LoadingScreen::draw()
{
    // =========================================
    // Loading background
    // =========================================

    Texture2D background =
        assets.getLoadingBackground();

    if (background.id != 0)
    {
        DrawTexturePro(
            background,

            Rectangle{
                0.0f,
                0.0f,
                static_cast<float>(background.width),
                static_cast<float>(background.height)},

            Rectangle{
                0.0f,
                0.0f,
                static_cast<float>(GetScreenWidth()),
                static_cast<float>(GetScreenHeight())},

            Vector2{
                0.0f,
                0.0f},

            0.0f,

            WHITE);
    }

    // =========================================
    // Loading text
    // =========================================

    const char *text = "Loading . . .";

    Font font =
        assets.getLoadingFont();

    float fontSize = 50.0f;
    float spacing = 2.0f;

    
    Vector2 textSize =
        MeasureTextEx(
            font,
            text,
            fontSize,
            spacing);

    float textX =
        (GetScreenWidth() - textSize.x) / 2.0f;

    float textY =
        (GetScreenHeight() - textSize.y) / 1.5f;

    DrawTextEx(
        font,
        text,

        Vector2{
            textX,
            textY},

        fontSize,
        spacing,

        WHITE);
}

bool LoadingScreen::isFinished() const
{
    return elapsedTime >= duration;
}

void LoadingScreen::reset()
{
    elapsedTime = 0.0f;
}