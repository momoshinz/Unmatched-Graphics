#include "graphics/LoadingScreen.h"
#include "graphics/AssetManager.h"

LoadingScreen::LoadingScreen(AssetManager *assets, float duration)
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
    if (assets == nullptr)
        return;

    Texture2D loading = assets->getLoadingBackground();

    if (loading.id == 0)
        return;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // -----------------------------
    // Loading Background
    // -----------------------------

    DrawTexturePro(
        loading,

        Rectangle{
            0,
            0,
            (float)loading.width,
            (float)loading.height
        },

        Rectangle{
            0,
            0,
            (float)screenWidth,
            (float)screenHeight
        },

        Vector2{0, 0},
        0.0f,
        WHITE
    );

    // -----------------------------
    // Loading Text
    // -----------------------------

    const char *text = "LOADING...";

    int fontSize = 35;

    int textWidth = MeasureText(text, fontSize);

    float textX =
        (screenWidth - textWidth) / 2.0f;

    float textY =
        screenHeight * 0.85f;

    DrawText(
        text,
        (int)textX,
        (int)textY,
        fontSize,
        WHITE
    );
}

bool LoadingScreen::isFinished() const
{
    return elapsedTime >= duration;
}

void LoadingScreen::reset()
{
    elapsedTime = 0.0f;
}