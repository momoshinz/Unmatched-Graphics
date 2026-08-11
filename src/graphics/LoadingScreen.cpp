#include "graphics/LoadingScreen.h"
#include <iostream>

LoadingScreen::LoadingScreen(float duration)
    : background{},
      font{},
      elapsedTime(0.0f),
      duration(duration)
{
}

bool LoadingScreen::load()
{
    std::cout << "Loading loading screen...\n";

    // -----------------------------
    // Loading background
    // -----------------------------

    background =
        LoadTexture("Unmatched_Assets/loading.png");

    // -----------------------------
    // Sweet Magic font
    // -----------------------------

    font =
        LoadFont("Unmatched_Assets/fonts/Sweet Magic.ttf");

    // -----------------------------
    // Check assets
    // -----------------------------

    if (background.id == 0)
    {
        std::cout
            << "Failed to load loading.png\n";

        unload();
        return false;
    }

    if (font.texture.id == 0)
    {
        std::cout
            << "Failed to load Sweet Magic.ttf\n";

        unload();
        return false;
    }

    std::cout
        << "LoadingScreen assets loaded successfully.\n";

    return true;
}

void LoadingScreen::unload()
{
    if (background.id != 0)
    {
        UnloadTexture(background);
        background = {};
    }

    if (font.texture.id != 0)
    {
        UnloadFont(font);
        font = {};
    }
}

void LoadingScreen::update()
{
    elapsedTime += GetFrameTime();
}

void LoadingScreen::draw()
{
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
                (float)background.width,
                (float)background.height
            },

            Rectangle{
                0,
                0,
                (float)GetScreenWidth(),
                (float)GetScreenHeight()
            },

            Vector2{
                0,
                0
            },

            0.0f,

            WHITE
        );
    }

    // =========================================
    // LOADING...
    // =========================================

    const char *text = "Loading...";

    float fontSize = 45.0f;
    float spacing = 2.0f;

    Vector2 textSize =
        MeasureTextEx(
            font,
            text,
            fontSize,
            spacing
        );

    float textX =
        (GetScreenWidth() - textSize.x) / 2.0f;

    float textY =
        (GetScreenHeight() - textSize.y) / 2.0f;

    DrawTextEx(
        font,
        text,

        Vector2{
            textX,
            textY
        },

        fontSize,
        spacing,

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

LoadingScreen::~LoadingScreen()
{
    unload();
}