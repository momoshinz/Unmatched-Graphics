#include <raylib.h>
#include <iostream>

#include "graphics/AssetManager.h"

int main()
{
    // ساخت پنجره
    InitWindow(1600, 900, "Unmatched");

    // محدود کردن بازی به 60 فریم بر ثانیه
    SetTargetFPS(60);

    std::cout << "Window created.\n";

    // ساخت AssetManager
    AssetManager assets;

    std::cout << "AssetManager created.\n";

    // لود کردن Assetها
    if (!assets.load())
    {
        std::cout << "ASSET LOADING FAILED!\n";

        CloseWindow();
        return 1;
    }

    std::cout << "ALL ASSETS LOADED!\n";

    // -----------------------------
    // LOADING SCREEN
    // -----------------------------

    float loadingTime = 10.0f;
    float elapsedTime = 0.0f;

    while (!WindowShouldClose() &&
       elapsedTime < loadingTime)
{
    float deltaTime = GetFrameTime();

    elapsedTime += deltaTime;

    BeginDrawing();

    ClearBackground(BLACK);

    Texture2D loading =
        assets.getLoadingBackground();


    // -----------------------------
    // Background
    // -----------------------------

    float scaleX = 1600.0f / loading.width;
    float scaleY = 900.0f / loading.height;

    float scale = (scaleX > scaleY)
                    ? scaleX
                    : scaleY;


    float drawWidth =
        loading.width * scale;

    float drawHeight =
        loading.height * scale;


    float posX =
        (1600.0f - drawWidth) / 2.0f;

    float posY =
        (900.0f - drawHeight) / 2.0f;


    DrawTexturePro(
        loading,

        Rectangle{
            0,
            0,
            (float)loading.width,
            (float)loading.height
        },

        Rectangle{
            posX,
            posY,
            drawWidth,
            drawHeight
        },

        Vector2{
            0,
            0
        },

        0.0f,

        WHITE
    );


    // -----------------------------
    // LOADING TEXT
    // -----------------------------

    const char *text = "LOADING...";

    int fontSize = 45;

    int textWidth =
        MeasureText(text, fontSize);

    int textX =
        (1600 - textWidth) / 2;

    int textY =
        (900 - fontSize) / 2;


    DrawText(
        text,
        textX,
        textY,
        fontSize,
        WHITE
    );


    EndDrawing();
}

    // -----------------------------
    // TEST MAIN MENU
    // -----------------------------

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        Texture2D menu =
            assets.getMainMenuBackground();

        DrawTexturePro(
            menu,

            Rectangle{
                0,
                0,
                (float)menu.width,
                (float)menu.height},

            Rectangle{
                0,
                0,
                1280,
                720},

            Vector2{
                0,
                0},

            0.0f,

            WHITE);

        EndDrawing();
    }

    // آزاد کردن Assetها
    assets.unload();

    // بستن پنجره
    CloseWindow();

    return 0;
}